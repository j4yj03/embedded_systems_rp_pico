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



#define SERVO_MIN 1638
#define SERVO_INIT 4096
#define SERVO_MAX 8192
#define SERVO_STEP 32

typedef struct {
    uint16_t gpio;
    uint16_t pwm_slice_num;
    uint16_t pwm_config;

    uint16_t duty;
} SERVO;


#endif