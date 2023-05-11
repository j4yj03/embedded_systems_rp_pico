
#include "servo_pwm.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"


int main() {

    static uint16_t duty = SERVO_MIN;
 
    // Tell GPIO 0 and 1 they are allocated to the PWM
    gpio_set_function(PWM_OUT, GPIO_FUNC_PWM);
    //gpio_set_function(PWM_OUT, GPIO_FUNC_PWM);
 
    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    uint slice_num = pwm_gpio_to_slice_num(PWM_OUT);
 
    // Set period of 4 cycles (0 to 3 inclusive)
    pwm_set_wrap(slice_num, 65535);

    // pwm clock divider
    pwm_set_clkdiv(slice_num, 38.3f);

    // Set the PWM running
    pwm_set_enabled(slice_num, true);

    // Set channel A output high for one cycle before dropping
    pwm_set_chan_level(slice_num, PWM_CHAN_A, SERVO_INIT);
    // Set initial B output high for three cycles before dropping
    // pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);

    
 
    // Note we could also use pwm_set_gpio_level(gpio, x) which looks up the
    // correct slice and channel for a given GPIO.

    sleep_ms(500);

    while (1)
    {

        while(duty < SERVO_MAX) // besser pwm_config max
        {
            //pwm_set_gpio_level(PWM_OUT, duty);
            pwm_set_chan_level(slice_num, PWM_CHAN_A, duty);
            duty = duty + SERVO_STEP;
            sleep_ms(SLEEPMS);
            
        }
        
        while(duty > SERVO_MIN)  
        {
            //pwm_set_gpio_level(PWM_OUT, duty);
            pwm_set_chan_level(slice_num, PWM_CHAN_A, duty);
            duty = duty - SERVO_STEP;
            sleep_ms(SLEEPMS);

        }
        
    }

}