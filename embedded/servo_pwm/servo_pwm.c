
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
    
    pwm_set_irq_enabled(slice_num, false);
    pwm_clear_irq(slice_num);

    if (going_up) {

        duty = duty + SERVO_STEP;

        if (duty >= SERVO_MAX) 
        {
            duty = SERVO_MAX;
            going_up = false;
        }
    } else {
        
        duty = duty - SERVO_STEP;

        if (duty <= SERVO_MIN) 
        {
            duty = SERVO_MIN;
            going_up = true;
        }
    }
    // Square the fade value to make the LED's brightness appear more linear
    // Note this range matches with the wrap value
    pwm_set_gpio_level(PWM_OUT, duty);

    sleep_ms(SLEEPMS);

    pwm_set_irq_enabled(slice_num, true);
}
/*
void on_button_press()
{
    static 

}
*/

int main() 
{

    // Tell the LED pin that the PWM is in charge of its value.
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
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 37.f);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);



    pwm_set_gpio_level(PWM_OUT, SERVO_INIT);

    sleep_ms(SLEEPMS*100);

    irq_set_enabled(PWM_IRQ_WRAP, true);
    // Everything after this point happens in the PWM interrupt handler, so we
    // can twiddle our thumbs
    while (1)
        tight_loop_contents();

}