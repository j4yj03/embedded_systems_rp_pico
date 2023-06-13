#ifndef SERVO_PWM_H
#define	SERVO_PWM_H


// define clock constants
#define _XTAL_FREQ 125e6
#define _PWM_FREQ 50


// define time constants
//#define DEBOUNCE_MS 50
#define SLEEP_MS 200
#define TIMER_DELAY_MS 200
#define TIMER_CONST_MS 2


// define GPIOS

#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define PWM_OUT 16
#define SERVO_BUTTON_L 18
#define SERVO_BUTTON_R 19

//#define NGPIO          2

// define servo constants
#define SERVO_MIN 1638
#define SERVO_INIT 4096
#define SERVO_MAX 8192
#define SERVO_STEP 16

// uart defines
#define UART_ID uart0
#define BAUD_RATE 57600 //115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// dma define
#define DMA_CHANNEL 0

#define DMA_RING_SIZE 4
#define MAX_STRING_LEN (1 << DMA_RING_SIZE)



// define new type: servo
typedef struct {
    unsigned int gpio;
    unsigned int pwm_slice_num;
    //unsigned int pwm_wrap;
    //float pwm_devider;

    unsigned int duty;
} SERVO;


#endif