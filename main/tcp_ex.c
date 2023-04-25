#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#include "modules/FatFs/ff.h"
#include "modules/FatFs/diskio.h"
#include "modules/ntp_time/ntp_time.h"
#include "string.h"
#include "inttypes.h"

#define WIFI_SSID "your_SSID"
#define WIFI_PASSWORD "your_PASSWORD"
#define TCP_SERVER_IP "your_SERVER_IP"
#define TCP_PORT 5656
#define BUF_SIZE (TCP_MSS*2)
#define POLL_TIME_S 5

typedef struct TCP_CLIENT_T_
{
    struct tcp_pcb *tcp_pcb;
    ip_addr_t remote_addr;
    int sent_len;
     bool connected;
} TCP_CLIENT_T;

TCP_CLIENT_T *tcp_client=NULL;

err_t tcp_client_connected_cb(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {
        printf("connect callback error\n");
        return err;
    }
    TCP_CLIENT_T *tcp_client=(TCP_CLIENT_T *)arg;
    
    printf("connect to ip:%s\n", ip4addr_ntoa(&tcp_client->remote_addr));
    tcp_client->connected = true;
    return ERR_OK;
}

err_t tcp_client_sent_cb(void *arg, struct tcp_pcb *tpcb, u16_t len){ 
    err_t err = ERR_OK;
    //printf("tcp server received: %d\n", len);
    TCP_CLIENT_T *tcp_client = (TCP_CLIENT_T*)arg;
    tcp_client->sent_len -= len;
    return err;
    
}

err_t tcp_client_close(void *arg)
{
    TCP_CLIENT_T *tcp_client = (TCP_CLIENT_T *)arg;
    err_t err = ERR_OK;
    if (tcp_client->tcp_pcb != NULL)
    {
        tcp_arg(tcp_client->tcp_pcb, NULL);
        tcp_poll(tcp_client->tcp_pcb, NULL, 0);
        tcp_sent(tcp_client->tcp_pcb, NULL);
        tcp_recv(tcp_client->tcp_pcb, NULL);
        tcp_err(tcp_client->tcp_pcb, NULL);
        err = tcp_close(tcp_client->tcp_pcb);
        if (err != ERR_OK)
        {
            printf("close failed %d, calling abort\n", err);
            tcp_abort(tcp_client->tcp_pcb);
            err = ERR_ABRT;
        }
        tcp_client->tcp_pcb = NULL;
    }
    return err;
}

err_t tcp_client_recv_cb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        printf(" connect close\n");
        tcp_client_close(arg);
        return err;
    } 
    cyw43_arch_lwip_check();
    uint8_t *buf = p->payload;
    buf[p->tot_len]='\0';
    printf("received data:%s\n", buf);
    
    if(strcmp())

    tcp_recved(tpcb, p->tot_len);
    pbuf_free(p);  // * important
    return err;
}

void tcp_client_err_cb(void *arg, err_t err) {
    printf("tcp_client error: %d\n", err);
    tcp_client_close(arg);
}

err_t tcp_client_poll_cb(void *arg, struct tcp_pcb *tpcb) {
    printf("tcl_client_poll_cb\n");

    return ERR_OK;

}

bool tcp_client_connect_server(uint8_t *addr, uint16_t port) {
    tcp_client = (TCP_CLIENT_T*) calloc(1, sizeof(TCP_CLIENT_T));
    if (tcp_client == NULL) return false;
    if (! ip4addr_aton(addr, &tcp_client->remote_addr)) return false;
    tcp_client->tcp_pcb = tcp_new_ip_type(IP_GET_TYPE(&tcp_client->remote_addr));
    tcp_client->connected=false;
    tcp_client->sent_len=0;

    tcp_arg(tcp_client->tcp_pcb, tcp_client);
    tcp_sent(tcp_client->tcp_pcb, tcp_client_sent_cb);
    tcp_recv(tcp_client->tcp_pcb, tcp_client_recv_cb);
    tcp_err(tcp_client->tcp_pcb, tcp_client_err_cb);
    tcp_poll(tcp_client->tcp_pcb, tcp_client_poll_cb, POLL_TIME_S*2);

    absolute_time_t timeout = make_timeout_time_ms(10000);
    err_t err = tcp_connect(tcp_client->tcp_pcb, &tcp_client->remote_addr, port, tcp_client_connected_cb);
    if (err != ERR_OK) {
        printf("connect to server error!\n");
        return false;
    }
    while (absolute_time_diff_us(get_absolute_time(), timeout) > 0 && !tcp_client->connected) {
        sleep_ms(100);
    }
    if (!tcp_client->connected) {
        printf("connect time out\n");
        return false;
    }
    return true;
}
void upload_file(uint8_t *filename) {
    FIL fil;
    FRESULT res;
    char path[100];
    if (!tcp_client_connect_server(TCP_SERVER_IP, TCP_PORT)){
        return;
    }
    sprintf(path, "%s/%s", SDMMC_PATH, filename);
    res = f_open(&fil, path, FA_READ);
    if (res != FR_OK) {
        printf("open file error:\n");
        return;
    }

    uint8_t buff[BUF_SIZE];
    UINT br;
    
    //absolute_time_t t1 = get_absolute_time();
    do {
        res = f_read(&fil, buff, BUF_SIZE, &br);
        tcp_write(tcp_client->tcp_pcb, buff, br, TCP_WRITE_FLAG_COPY);
        tcp_client->sent_len += br;
        tcp_output(tcp_client->tcp_pcb);
       
        while(tcp_client->sent_len > 0) 
        {
            sleep_us(1);
        }

    } while(br >0);
    //printf("tcp upload time=%"PRIu64"\n", absolute_time_diff_us(t1, get_absolute_time()));
    res=f_close(&fil);
    tcp_client_close(tcp_client);
}

int main()
{
    stdio_init_all();
    gpio_init(5);
    gpio_init(6);
    gpio_set_dir(5, 1);
    gpio_set_dir(6,1);
    gpio_put(5,1);
    gpio_put(6,0);

    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("failed to connect\n");
        return 1;
    }
    ntp_time_init();
    get_ntp_time();
    gpio_put(5,0);
    gpio_put(6,1);
    FIL fil;
    FATFS fs;
    FRESULT res;

  
    
    res = f_mount(&fs, SDMMC_PATH, 1);
    if (res != FR_OK)
    {
        printf(" mount error\n");
        return 0;
    }
    uint8_t fname[15];
    for (int j=0;j < 2;j++)
    for (int i=1; i <=8; i++) {
        sprintf(fname, "pic%d.jpg",i);
        printf("%s\n", fname);
        upload_file(fname);
    }
    gpio_put(6,0);
    printf("Finish\n");
    if (tcp_client) free(tcp_client);
    return 0;
}