
#include "servo_pwm.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"


void on_pwm_wrap() 
{
    static uint16_t duty = SERVO_MIN;

    static bool going_up = true;

    uint16_t slice_num = pwm_gpio_to_slice_num(PWM_OUT);
    
    // Disable interrupt enable flag and clear interrupt
    //pwm_set_irq_enabled(slice_num, false);
    pwm_clear_irq(slice_num);

    if (going_up) 
    {

        duty = duty + SERVO_STEP;

        if (duty >= SERVO_MAX) 
        {
            //duty = SERVO_MAX;
            going_up = false;
        }
    } else 
    {
        
        duty = duty - SERVO_STEP;

        if (duty <= SERVO_MIN) 
        {
            //duty = SERVO_MIN;
            going_up = true;
        }
    }

    // Set dutycycle counter
    pwm_set_gpio_level(PWM_OUT, duty);

    //sleep_ms(SLEEPMS);

    // Enable interrupt flag
    //pwm_set_irq_enabled(slice_num, true);
}




int main() 
{

    // Tell the ddfined pin is in charge of its value.
    gpio_set_function(PWM_OUT, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to GPIO 16 (it's slice 16)
    uint16_t slice_num = pwm_gpio_to_slice_num(PWM_OUT);

    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    

    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();

    // Set divider, target requeny is 50Hz or 20ms
    const float divider = _XTAL_FREQ / _PWM_FREQ / 4096 / 16; //12bit (8.4) mask + shift right 4 Bit
    pwm_config_set_clkdiv(&config, divider);

    // Set new wrap to adjust
    const uint16_t wrap = _XTAL_FREQ / divider / _PWM_FREQ - 1;
    pwm_config_set_wrap(&config, wrap);

    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);


    // Move Servo to middle position
    pwm_set_gpio_level(PWM_OUT, SERVO_INIT);

    sleep_ms(SLEEPMS*100);

    // Enable pwm wrap (+ global?) interrupt
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Everything after this point happens in the PWM interrupt handler
    while (1)
        tight_loop_contents();

}