
#include "servo_pwm.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"


/*
int main() 
{

    static uint16_t fade = 0;

    // Set PWM Pin
    gpio_set_function(PWM_OUT, GPIO_FUNC_PWM);
    // Find out which PWM slice is connected to GPIO 16 (it's slice 16)
    uint slice_num = pwm_gpio_to_slice_num(PWM_OUT);

    pwm_set_enabled(slice_num, true);

    
    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 377.f);


    
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);

    //pwm_set_gpio_level(PWM_OUT, 65535 / 2);

    
    while (1)
    {
        while(fade < 256) // besser pwm_config max
        {
            pwm_set_gpio_level(PWM_OUT, fade);
            fade++;
            sleep_ms(SLEEPMS);
            
        }
        
        while(fade > 0)  
        {
            pwm_set_gpio_level(PWM_OUT, fade);
            fade--;
            sleep_ms(SLEEPMS);

        }
        
    }
        
}*/


int main() {

    static uint16_t fade = SERVO_MIN;

    const int fade_step = 6;
 
    // Tell GPIO 0 and 1 they are allocated to the PWM
    gpio_set_function(PWM_OUT, GPIO_FUNC_PWM);
    //gpio_set_function(PWM_OUT, GPIO_FUNC_PWM);
 
    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    uint slice_num = pwm_gpio_to_slice_num(PWM_OUT);
 
    // Set period of 4 cycles (0 to 3 inclusive)
    pwm_set_wrap(slice_num, 65535);

    // pwm clock divider
    pwm_set_clkdiv(slice_num, 36.f);

    // Set the PWM running
    pwm_set_enabled(slice_num, true);

    // Set channel A output high for one cycle before dropping
    pwm_set_chan_level(slice_num, PWM_CHAN_A, (0.5) * 65535);
    // Set initial B output high for three cycles before dropping
    // pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);

    
 
    // Note we could also use pwm_set_gpio_level(gpio, x) which looks up the
    // correct slice and channel for a given GPIO.

    sleep_ms(500);

    while (1)
    {

        while(fade < SERVO_MAX) // besser pwm_config max
        {
            //pwm_set_gpio_level(PWM_OUT, fade);
            pwm_set_chan_level(slice_num, PWM_CHAN_A, fade);
            fade = fade + fade_step;
            sleep_ms(SLEEPMS);
            
        }
        
        while(fade > SERVO_MIN)  
        {
            //pwm_set_gpio_level(PWM_OUT, fade);
            pwm_set_chan_level(slice_num, PWM_CHAN_A, fade);
            fade = fade - fade_step;
            sleep_ms(SLEEPMS);

        }
        
    }

}