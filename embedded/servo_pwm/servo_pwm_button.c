#include "servo_pwm.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"


/* one timeout for each gpio, set as debounce_timeouts[gpio] */
static volatile absolute_time_t debounce_timeouts[NGPIO] = { 0u };

static SERVO * servo;


/** debounce function takes gpio pin to debounce as argument and
 *  returns true if pin is currently in debounce state, false otherwise
 */
bool debounce (uint gpio)
{
  /* check if gpio is less than DEBOUNCE_MS from time set */
  if (get_absolute_time() < debounce_timeouts[gpio]) {
    return true;
  }

  /* otherwise set new debouce timout for gpio pin */
  debounce_timeouts[gpio] = make_timeout_time_ms(DEBOUNCE_MS);

  return false;
}


void on_pwm_wrap() 
{
    
    // clear interrupt
    pwm_clear_irq(servo->pwm_slice_num);

    // Set dutycycle counter
    pwm_set_gpio_level(PWM_OUT, servo->duty);
}

void on_button_falling(uint gpio, uint32_t events)
{
    /* check/register debounce on gpio */
    if (debounce (gpio)) 
    {
        return;
    }

    switch(gpio)
    {
        default: break;
        case SERVO_BUTTON_L:
            servo->duty = servo->duty + SERVO_STEP;
            break;
        case SERVO_BUTTON_R:
            servo->duty = servo->duty - SERVO_STEP;
            break;
    }
}


int main()
{
    
    // Tell the ddfined pin is in charge of its value.
    gpio_set_function(PWM_OUT, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to GPIO 16 (it's slice 16)
    uint16_t slice_num = pwm_gpio_to_slice_num(PWM_OUT);

    servo->pwm_slice_num = slice_num;

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

    //servo->pwm_devider = divider;

    // Set new wrap to adjust
    const uint16_t wrap = _XTAL_FREQ / divider / _PWM_FREQ - 1;
    pwm_config_set_wrap(&config, wrap);

    //servo->pwm_wrap = wrap;

    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);


    /* initialize buttons with pull-up resistors */
    gpio_init (SERVO_BUTTON_L);
    gpio_pull_up (SERVO_BUTTON_L);

    gpio_init (SERVO_BUTTON_R);
    gpio_pull_up (SERVO_BUTTON_R);

    /* set button_callback */
    gpio_set_irq_enabled_with_callback (SERVO_BUTTON_L, GPIO_IRQ_EDGE_FALL, true, on_button_falling);
    gpio_set_irq_enabled (SERVO_BUTTON_R, GPIO_IRQ_EDGE_FALL, true);


    // Move Servo to middle position
    pwm_set_gpio_level(PWM_OUT, SERVO_INIT);

    sleep_ms(SLEEP_MS);

    // Enable pwm wrap (+ global?) interrupt
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Everything after this point happens in the PWM interrupt handler
    while (1)
        tight_loop_contents();

}