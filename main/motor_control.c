#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"

#define CLKDIV 256.0f
#define PWM_WRAP 9804
#define MIN_POS_DELTA 100
struct PWM{
    uint pin;
    uint slice_num;
    uint channel;
};
struct PID{
    uint8_t Kp;
    uint32_t minSpeed;
    long int current_pos;
    long int pos_error;
    long int pos_setpoint;
    uint32_t duty_cycle;
    uint32_t critical_delta;
    uint32_t limit;
};
struct Motor{
    int fwdPin;
    int bwdPin;
    int encoderAB;
    struct PWM pwm;
    struct PID pid; 
};


/**
 * @brief motor initialization functiuon
 * @param fwdPin forward pin to driver 
 * @param bwdPin backwards pin to driver
 * @param encoderAB first encoder pin (the second pin must be sequential)
 * @param pwm_pin pin for pwm to driver, pins are in pairs doe to slices
 * @return motor structure
*/
struct Motor init_motor(const int fwdPin,const int bwdPin,const int encoderAB){
   
    // initialize motor components
    struct Motor motor;
    motor.fwdPin = fwdPin;
    motor.bwdPin = bwdPin;
    motor.encoderAB = encoderAB;

    // initialize pins
    gpio_init(fwdPin);
    gpio_init(bwdPin);
    gpio_set_dir(fwdPin, true);
    gpio_set_dir(bwdPin, true);

    return motor;
}

/**
 * @brief initialize PID parameters for a defined motor
 * @param motor passed by pointer, modified motor parameters
 * @param critical_delta critical error, at wich the PID will slope down
 * @param limit rotational limit of the motor
*/
void init_PID(struct Motor *motor,const int critical_delta, const int limit){
    motor->pid.critical_delta = critical_delta;
    motor->pid.limit = limit;
}

/**
 * @brief initializes the motor pwm, sets the slices and channels, stores them into the motor structure, and sets the pwm frequency
 * @param motor the motor in question
 * @param pin the pwm pin (must be in pairs) that leads to the driver
 * @param frequency the frequency of the pwm duty cycle [0 - 9804]
*/
void init_motor_pwm(struct Motor *motor,const int pin, const int frequency){

    motor->pwm.pin = pin;                                   
    gpio_set_function(motor->pwm.pin, GPIO_FUNC_PWM);                           // set pin function

    motor->pwm.slice_num = pwm_gpio_to_slice_num(pin);               // get pwm slice from gpio
    motor->pwm.channel = pwm_gpio_to_channel(pin);                   // get pwm channel of the pin (1/2)

    pwm_set_clkdiv(motor->pwm.slice_num, CLKDIV);                               // set clock div
    pwm_set_wrap(motor->pwm.slice_num, PWM_WRAP);                               // set pwm wrap

    pwm_set_chan_level(motor->pwm.slice_num, motor->pwm.channel, frequency);    // set channel level
    pwm_set_enabled(motor->pwm.slice_num, true);                                // enable slice

}
/**
 * @brief inline function used by a pinter function, to move the motors, at a set speed taken from the motors struct
 * @param motor the motor that will move
*/
inline static void forward(const struct Motor *motor){
    printf("moving fwd\n");
    gpio_put(motor->fwdPin,1);
    gpio_put(motor->bwdPin,0);
    pwm_set_chan_level(motor->pwm.slice_num, motor->pwm.channel, motor->pid.duty_cycle);
}
/**
 * @brief inline static function used by a function pointer, that reverses the motors
 * @param motor the motor in question
*/
inline static void backrward(const struct Motor *motor){
    printf("movin bwd\n");
    gpio_put(motor->fwdPin,0);
    gpio_put(motor->bwdPin,1);
    pwm_set_chan_level(motor->pwm.slice_num, motor->pwm.channel, motor->pid.duty_cycle);
}
/**
 * @brief inline static function used by function pointer, that stops the motor
 * @param motor the motor inb question
*/
inline static void stop(const struct Motor *motor){
    gpio_put(motor->fwdPin,0);
    gpio_put(motor->bwdPin,0);
    pwm_set_chan_level(motor->pwm.slice_num,motor->pwm.channel,0);
    printf("im on stop\n");
}
/**
 * @brief update function, needs to be called from main methon for each motor, it requires encoder ticks
 * @param motor motor in question
 * @param encoder the position of the motor given by encoder ticks
*/
void update_error(struct Motor *motor,const int encoder){
    motor->pid.current_pos = encoder;
    motor->pid.pos_error = motor->pid.pos_setpoint - encoder;
}

/**
 * @brief static inline function, that sets the setpoint in the motor structure
 * @param motor the motor who's setpoint will be modified
*/
static void set_motor_sp(struct Motor *motor,const long int SP){
    motor->pid.pos_setpoint = SP;
}

/**
 * @brief static function that computes the duty cycle for a given error, taking the critical_Delta into consideration
 * @param motor is the motor in question
*/
inline static void compute_duty(struct Motor *motor){
    if(motor->pid.pos_error > motor->pid.critical_delta) 
        motor->pid.duty_cycle = 100; //max speed in %
    else
        motor->pid.duty_cycle = motor->pid.minSpeed + ((abs(motor->pid.pos_error)/motor->pid.critical_delta)*(100));
    motor->pid.duty_cycle = motor->pid.duty_cycle/100 * 65535;    
}
/**
 * @brief function to detect motor stop conditions
 * @param motor the motor struct variable
 * @return true if motor is in stop condition 
*/
bool motor_stop_cond(const struct Motor motor){
    return (motor.pid.pos_error < MIN_POS_DELTA) || (motor.pid.current_pos >= motor.pid.limit) || (motor.pid.current_pos <= (-1)*MIN_POS_DELTA);
}
/**
 * @brief function to move the motors by delta amount of ticks  
 * @param motor the motor struct 
 * @param posotion_delta position setpoint for the motor, passed by pointer
 * @param action function pointer 
 * @tparam forward()  - action function
 * @tparam backward()  - action function 
 * @tparam stop()  - action function
 * @param encoder encoder ticks from PIO
 * 
*/
void move_motor_inc(struct Motor *motor,uint32_t *position_delta,  void (*action)(struct Motor), uint encoder){
    
    compute_duty(motor);
    (*action)(*motor);
    if(abs(motor->pid.pos_error)> *position_delta){ 
        stop(motor); 
        set_motor_sp(motor,motor->pid.current_pos);
        *position_delta = 0;
    }
    else update_error(motor, encoder);     
}
/**
 * @brief absolute control for the motors
 * @param motor mtor strcuture variable
 * @param abs_pos absolute position [ticks]
 * @param encoder encoder ticks[ticks]
*/
void move_motor_abs(struct Motor *motor, long int abs_pos,int encoder){
    set_motor_sp(motor,abs_pos);
    update_error(motor,encoder);
    compute_duty(motor);
    if(motor->pid.pos_error > MIN_POS_DELTA){
        forward(motor);
        printf("%d ticks, %d err, %d sp\n ", encoder, motor->pid.pos_error, motor->pid.pos_setpoint);
    }
    else if (motor->pid.pos_error < -MIN_POS_DELTA) backrward(motor);
    else stop(motor);
    printf("%d \n", motor->pid.duty_cycle);
    
}