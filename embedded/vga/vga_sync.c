#include "vga.h"

static uint16_t slice_num_hsync;

/** \brief configure pwm channel for hsync
 *   
*/
void hsync_irq()
{
    // clear color bits
    gpio_clr_mask(COLOR_GPIO_MASK_256);

    sem_release(&display_on);
    // release semaphore
    //hsync_high = true;

    // increment linecounter 
    line_counter++;

    //uart_puts(UART_ID, itoa(line_counter, int_string, 10));

    gpio_put_masked(V_SYNC_GPIO_MASK, ((line_counter > 0) << V_SYNC_GPIO_OFFSET));
    //gpio_put(V_SYNC_GPIO_OFFSET, vga.vsync_level);


    // line_counter > 519 ?
    if (line_counter > VSYNC_NPW_LINES) 
    {
        // line_counter = -2 start with negative Pulse
        line_counter = - VSYNC_PW_LINES;

        // increment framecounter
        frame_counter++;
    }

    pwm_clear_irq(slice_num_hsync);
}




/** \brief configure pwm channel for hsync
 *   
*/
inline void configure_pwm_hsync()
{
    // no prescaler needed
    //const float hsync_divider = 1;// _XTAL_FREQ; // / _HSYNC_PWM_FREQ / 4096 / 16; //12bit (8.4) mask + shift right 4 Bit

    const uint16_t hsync_wrap = _XTAL_FREQ / _HSYNC_PWM_FREQ;// / hsync_divider;

    slice_num_hsync = pwm_gpio_to_slice_num(H_SYNC_GPIO_OFFSET);

    static pwm_config hsync_config = {0, 0, 0};


    // calculate counter compare value (duty cycle)
    const uint16_t hsync_duty = hsync_wrap * HSYNC_DC;

    // stop blanking after 720 pwm counts (18%)
    //vga.hsync_blanking_cc = hsync_wrap * HSYNC_BLANKING_DC;


    pwm_config_set_phase_correct(&hsync_config, false);
    pwm_config_set_clkdiv(&hsync_config, 1);

    pwm_config_set_clkdiv_mode(&hsync_config, PWM_DIV_FREE_RUNNING);
    pwm_config_set_output_polarity(&hsync_config, true, true);
    pwm_config_set_wrap(&hsync_config, hsync_wrap);

    gpio_set_function(H_SYNC_GPIO_OFFSET, GPIO_FUNC_PWM);

    pwm_init(slice_num_hsync, &hsync_config, false);



    pwm_set_enabled(slice_num_hsync, true);

    pwm_set_chan_level(slice_num_hsync, 0, hsync_duty);

    uart_puts(UART_ID, "\n\rStart HSYNC! pwm_slice: ");
    uart_puts(UART_ID, itoa(slice_num_hsync, int_string, 10));


}
/*
inline unsigned int hsync_get_counter() 
{
    return (pwm_hw->slice[slice_num_hsync].ctr);
}
*/

void configure_pwm_irq()
{
    pwm_clear_irq(slice_num_hsync);    

    
    irq_set_exclusive_handler(PWM_IRQ_WRAP, hsync_irq);

    pwm_set_irq_enabled(slice_num_hsync, true);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    uart_puts(UART_ID, "\n\rPWM Interrupt enabled!");

}