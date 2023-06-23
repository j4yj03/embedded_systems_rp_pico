#include "vga.h"
#include "vga_sync.h"



/** \brief configure pwm channel for hsync
 *   
*/
static inline void configure_pwm_hsync()
{
    const float hsync_divider = _XTAL_FREQ / _HSYNC_PWM_FREQ / 4096 / 16; //12bit (8.4) mask + shift right 4 Bit

    const uint16_t hsync_wrap = _XTAL_FREQ / hsync_divider / _HSYNC_PWM_FREQ - 1;

    const uint16_t slice_num_chan_hsync = pwm_gpio_to_slice_num(HSYNC_A);

    pwm_config hsync_config = {0, 0, 0};

    pwm_config_set_phase_correct(&hsync_config, false);
    pwm_config_set_clkdiv(&hsync_config, hsync_divider);
    pwm_config_set_clkdiv_mode(&hsync_config, PWM_DIV_FREE_RUNNING);
    pwm_config_set_output_polarity(&hsync_config, false, false);
    pwm_config_set_wrap(&hsync_config, hsync_wrap);

    gpio_set_function(HSYNC_A, GPIO_FUNC_PWM);
    gpio_set_function(HSYNC_B, GPIO_FUNC_PWM);

    pwm_init(slice_num_chan_hsync, &hsync_config, false);

    pwm_set_both_levels(slice_num_chan_hsync, 0);

}

/** \brief configure pwm channel for vsync
 *   
*/
static inline void configure_pwm_vsync()
{
    const float vsync_divider = _XTAL_FREQ / _VSYNC_PWM_FREQ / 4096 / 16;

    const uint16_t vsync_wrap = _XTAL_FREQ / vsync_divider / _VSYNC_PWM_FREQ - 1;

    const uint16_t slice_num_vsync = pwm_gpio_to_slice_num(VSYNC_A);

    pwm_config vsync_config = {0, 0, 0};

    pwm_config_set_phase_correct(&vsync_config, false);
    pwm_config_set_clkdiv(&vsync_config, vsync_divider);
    pwm_config_set_clkdiv_mode(&vsync_config, PWM_DIV_B_RISING);
    pwm_config_set_output_polarity(&vsync_config, false, false);
    pwm_config_set_wrap(&vsync_config, vsync_wrap);

    gpio_set_function(VSYNC_A, GPIO_FUNC_PWM);
    gpio_set_function(VSYNC_B, GPIO_FUNC_PWM);

    pwm_init(slice_num_vsync, &vsync_config, false);

    pwm_set_chan_level(slice_num_vsync, 0, 0);
}




void hsync_irq()
{

}

void vsync_irq()
{

}