#ifndef SERVO_PWM_H
#define	SERVO_PWM_H



#define _XTAL_FREQ 125e6
#define _PWM_FREQ 50



//#define DEBOUNCE_MS 50
#define SLEEP_MS 200
#define TIMER_DELAY_MS 300
#define TIMER_CONST_MS 2



#define PWM_OUT 16
#define SERVO_BUTTON_L 18
#define SERVO_BUTTON_R 19
// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

//#define NGPIO          2


#define SERVO_MIN 1638
#define SERVO_INIT 4096
#define SERVO_MAX 8192
#define SERVO_STEP 16


#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE



typedef struct {
    unsigned int gpio;
    unsigned int pwm_slice_num;
    //unsigned int pwm_wrap;
    //float pwm_devider;

    unsigned int duty;
} SERVO;


#endif