/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "pico/multicore.h"
#include "hardware/sync.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#include "encoder.pio.h"
#include "motor_control.c"

#define PINA(X) (2+2*X)
#define PINB(X) (3+2*X)
#define PWM_PIN(X) (X)
#define ENC_PIN(X) (14 + 2*(X-1))


#define DEBUGG "nigger"
#define WIFI_SSID "Holo"
#define WIFI_PASSWORD "Q1w2e3r4t5y6!"
#define TCP_SERVER_IP "10.147.2.233"
#define TCP_PORT 4242
#define DEBUG_printf printf
#define BUF_SIZE 32
#define RESP_BUF_SIZE 1

#define TEST_ITERATIONS 10
#define POLL_TIME_S 5

uint8_t motor_positions[] = {0,0,0,0};
int spin_num;
spin_lock_t *spinlock;

typedef struct TCP_CLIENT_T_ {
    struct tcp_pcb *tcp_pcb;
    ip_addr_t remote_addr;
    uint8_t buffer[BUF_SIZE];
    int buffer_len;
    int sent_len;
    int run_count;
    bool complete;
    bool connected;
} TCP_CLIENT_T;

TCP_CLIENT_T *pico_client = NULL;

static uint32_t construct(const char *number){
    uint32_t retVal = 0;
    uint8_t i = 0;
    while(*(number + i)!= '\0'){
        i++;
    }
    int max = i;
    i=0;
    while(i<max){
        retVal = retVal + (*(number+i)-48)*pow(10,max - i-1);
        i++;
    }
    return retVal;
}

static inline void deserialize(const char *serialized){
    
    uint32_t message = construct(serialized);
    for(uint8_t word=0 ;word < 4; word++ ){
        uint8_t command=0;
        for (uint8_t bit = 8; bit > 0; bit--){
            bool local_bit = message & (1 << (word*8+(bit-1)));
            command = command + (1<<(bit-1))*local_bit;
            motor_positions[word] = command;
        } 
    }
}

static err_t tcp_client_close(void *arg) {
    err_t err = ERR_OK;
    if (pico_client->tcp_pcb != NULL) {
        tcp_arg(pico_client->tcp_pcb, NULL);
        tcp_poll(pico_client->tcp_pcb, NULL, 0);
        tcp_sent(pico_client->tcp_pcb, NULL);
        tcp_recv(pico_client->tcp_pcb, NULL);
        tcp_err(pico_client->tcp_pcb, NULL);
        err = tcp_close(pico_client->tcp_pcb);
        if (err != ERR_OK) {
            DEBUG_printf("close failed %d, calling abort\n", err);
            tcp_abort(pico_client->tcp_pcb);
            err = ERR_ABRT;
        }
        pico_client->tcp_pcb = NULL;
    }
}

// Called with results of operation
static err_t tcp_result(void *arg, int state) {
    if (pico_client == 0) {
        DEBUG_printf("test success\n");
    } else {
        DEBUG_printf("test failed %d\n", pico_client);
    }
    pico_client->complete = true;
    return tcp_client_close(arg);
}

static err_t tcp_client_connected_cb(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {
        printf("connect failed %d\n", err);
        return tcp_result(arg, err);
    }
    pico_client->connected = true;
    DEBUG_printf("Waiting for buffer from server\n");
    return ERR_OK;
}

static void tcp_client_err_cb(void *arg, err_t err) {
    if (err != ERR_ABRT) {
        DEBUG_printf("tcp_client_err %d\n", err);
        tcp_result(arg, err);
    }
}

err_t tcp_client_recv_cb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    TCP_CLIENT_T *pico_client = (TCP_CLIENT_T*)arg;
    if (!p) {
        tcp_client_close(arg);
        return err;
    } 
    cyw43_arch_lwip_check();
    char *buf = p->payload;
    *(buf+(p->tot_len))='\0';
 
    if(strcmp(buf,"1"))
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    else if (strcmp(buf,"0"))
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    spin_lock_unsafe_blocking(spinlock);
    deserialize(buf);
    spin_unlock_unsafe(spinlock);

    tcp_recved(tpcb, p->tot_len);
    pbuf_free(p);

    uint8_t resp_buff[RESP_BUF_SIZE];
    resp_buff[0] = 1;
    err = tcp_write(tpcb, resp_buff, 1, TCP_WRITE_FLAG_COPY);

    return err;
}

err_t tcp_client_sent_cb(void *arg, struct tcp_pcb *tpcb, u16_t len){ 
    err_t err = ERR_OK;
    printf("tcp server received: %d\n", len);
    TCP_CLIENT_T *pico_client = (TCP_CLIENT_T*)arg;
    pico_client->sent_len -= len;
    return err;
    
}

// Initialize the tcp client and connect to the socket server using the server address
bool tcp_client_connect(uint8_t *addr){
    pico_client = (TCP_CLIENT_T*) calloc(1, sizeof(TCP_CLIENT_T));
    if (!ip4addr_aton(addr, &pico_client->remote_addr)) return false;
    pico_client->tcp_pcb = tcp_new_ip_type(IP_GET_TYPE(&pico_client->remote_addr));
    if (!pico_client->tcp_pcb) return false;
    pico_client->sent_len = 0;
    pico_client->buffer_len = 0;

    tcp_arg(pico_client->tcp_pcb, pico_client);
    tcp_sent(pico_client->tcp_pcb, tcp_client_sent_cb);
    tcp_recv(pico_client->tcp_pcb, tcp_client_recv_cb);
    tcp_err(pico_client->tcp_pcb, tcp_client_err_cb);

    err_t err = tcp_connect(pico_client->tcp_pcb, &pico_client->remote_addr, TCP_PORT, tcp_client_connected_cb);

    return err == ERR_OK;
}

void core1_entry(){
    if (cyw43_arch_init()) {
        DEBUG_printf("failed to initialise\n");
        return ;
    }
    cyw43_arch_enable_sta_mode();
    
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(250);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(250);

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return ;
    } else {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        printf("Connected.\n");
    }

    if (!tcp_client_connect(TCP_SERVER_IP)) {
        tcp_result(pico_client, -1);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        return;
    }

    while(!pico_client->complete) {
        sleep_ms(1);
    }

    free(pico_client);
    cyw43_arch_deinit();
}

int main() {
    sleep_ms(250);
    stdio_init_all();

    spin_num = spin_lock_claim_unused(true);
    spinlock = spin_lock_init(spin_num);
    bool available_message = false;
    multicore_launch_core1(core1_entry);

    int new_value1, delta, old_value1 = 0;
    int new_value2, delta2, old_value2 = 0;
    int new_value3, delta3, old_value3 = 0;
    int new_value4, delta4, old_value4 = 0;

    // Set encoder pins and state machines
    // Base pin to connect the A phase of the encoder.
    // The B phase must be connected to the next pin
    const uint PIN_AB = 12;
    const uint PIN_AB2 = 14;
    const uint PIN_AB3 = 16;
    const uint PIN_AB4 = 18; 

    PIO  pio2 = pio1;
    const uint sm = 0;
    const uint sm2 = 1;
    const uint sm3 = 2;
    const uint sm4 = 3;

    uint offset2 = pio_add_program(pio2, &quadrature_encoder_program);
    quadrature_encoder_program_init(pio2, sm, offset2, PIN_AB, 0);
    quadrature_encoder_program_init(pio2,sm2, offset2, PIN_AB2, 0);
    quadrature_encoder_program_init(pio2, sm3, offset2, PIN_AB3, 0);
    quadrature_encoder_program_init(pio2,sm4, offset2, PIN_AB4, 0);  
    /*
    // Set PWM pins and channels for motor drivers
    gpio_set_function(0, GPIO_FUNC_PWM);
    gpio_set_function(1, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(0);
    uint channel = pwm_gpio_to_channel(0);
    uint channel2 = pwm_gpio_to_channel(1);
    pwm_set_clkdiv(slice_num, 256.0f);
    pwm_set_wrap(slice_num, 9804);
    pwm_set_chan_level(slice_num, channel, 9804/2);
    pwm_set_chan_level(slice_num, channel2, 9804/3);
    pwm_set_enabled(slice_num, true);  

    gpio_set_function(2, GPIO_FUNC_PWM);
    gpio_set_function(3, GPIO_FUNC_PWM);
    uint slice_num2 = pwm_gpio_to_slice_num(2);
    uint channel3 = pwm_gpio_to_channel(2);
    uint channel4 = pwm_gpio_to_channel(3);
    pwm_set_clkdiv(slice_num2, 256.0f);
    pwm_set_wrap(slice_num2, 9804);
    pwm_set_chan_level(slice_num2, channel3, 9804/2);
    pwm_set_chan_level(slice_num2, channel4, 9804/3);
    pwm_set_enabled(slice_num2, true); 

    pwm_set_gpio_level(0, 9804/2);
    pwm_set_gpio_level(1, 9804/2);
    pwm_set_gpio_level(2, 9804/2);
    pwm_set_gpio_level(3, 9804/2);
    */
    // Set GPIO pins for motor driver inputs
    const uint PIN_IN1A = 4;
    const uint PIN_IN1B = 5;
    const uint PIN_IN2A = 6;
    const uint PIN_IN2B = 7; 
    const uint PIN_IN3A = 8;
    const uint PIN_IN3B = 9;
    const uint PIN_IN4A = 10;
    const uint PIN_IN4B = 11; 

    struct Motor motor = init_motor(PINA(2),PINB(2),ENC_PIN(2));
    init_motor_pwm(&motor,0,9804/2);
    init_PID(&motor,1000,30000);

    while(1){
        if(is_spin_locked(spinlock)){
            available_message = true;
        }
        else if(available_message == true){
            spin_lock_unsafe_blocking(spinlock);
            //printf("core 0 activity");
            
            spin_unlock_unsafe(spinlock);
            available_message = false;
            sleep_ms(1);
        }

        new_value3 = quadrature_encoder_get_count(pio2, sm3);
        //new_value2 = quadrature_encoder_get_count(pio2, sm2);
        //new_value1 = quadrature_encoder_get_count(pio2, sm);
        /*
        delta = new_value1 - old_value1;
        old_value1 = new_value1;
        v
        delta2 = new_value2 - old_value2;
        old_value2 = new_value2;
        
        delta3 = new_value3 - old_value3;
        old_value3 = new_value3;
        new_value4 = quadrature_encoder_get_count(pio2, sm4);
        delta4 = new_value4 - old_value4;
        old_value4 = new_value4;
        */
       long int fututi_mortii_pizdii_pulii_pizdii_ma_tii_lu_ma_ta = 3500;
       printf("%d pizda masii 1 \n",fututi_mortii_pizdii_pulii_pizdii_ma_tii_lu_ma_ta);
        move_motor_abs(&motor,fututi_mortii_pizdii_pulii_pizdii_ma_tii_lu_ma_ta, new_value3);

        printf("___________________\nposition 1 %8d|\n", new_value3);
        /*
        printf("___________________\nposition 2 %8d| delta 2 %6d|\n", new_value2, delta2);
        printf("___________________\nposition 3 %8d| delta 3 %6d|\n", new_value3, delta3);
        printf("___________________\nposition 4 %8d| delta 4 %6d|\n", new_value4, delta4);
 */   
    }
    return 0;
}