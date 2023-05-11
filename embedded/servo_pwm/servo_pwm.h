#ifndef SERVO_PWM_H
#define	SERVO_PWM_H



#define _XTAL_FREQ 125e6
#define _PWM_FREQ 50



#define DEBOUNCE_MS 50
#define SLEEP_MS 200


// define gpios
#define PWM_OUT 16
#define SERVO_BUTTON_L 18
#define SERVO_BUTTON_R 19

#define NGPIO          2


#define SERVO_MIN 1638
#define SERVO_INIT 4096
#define SERVO_MAX 8192
#define SERVO_STEP 32

typedef struct {
    unsigned int gpio;
    unsigned int pwm_slice_num;
    //unsigned int pwm_wrap;
    //float pwm_devider;

    unsigned int duty;
} SERVO;


#endif