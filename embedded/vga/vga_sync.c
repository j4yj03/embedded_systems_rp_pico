#include "vga.h"

static uint16_t slice_num_hsync;




/** \brief configure pwm channel for hsync
 *   
*/
void hsync_irq()
{

    gpio_clr_mask(COLOR_GPIO_MASK);

    vga.line_counter++; 

    vga.vsync_level = (vga.line_counter > 0);

    // set vsync level
    gpio_put_masked(V_SYNC_GPIO_MASK, (vga.vsync_level << V_SYNC_GPIO_OFFSET));
    //gpio_put(V_SYNC_GPIO_OFFSET, vga.vsync_level);


    // is current sync cycle in visible position
    // assert left to right
    vga.display_on = (vga.line_counter > VSYNC_FP_LINES) && (vga.line_counter < (VSYNC_DISPLAY_LINES + VSYNC_BP_LINES));

    
    
    

    //if (vga.line_counter > 519) {
    if (vga.line_counter > VSYNC_NPW_LINES) {

        
        //vga.line_counter = -2;
        vga.line_counter = - VSYNC_PW_LINES;
        //vga.vsync_level = ()
        vga.frame_counter++;
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

    slice_num_hsync = pwm_gpio_to_slice_num(HSYNC_A);

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

    gpio_set_function(HSYNC_A, GPIO_FUNC_PWM);

    pwm_init(slice_num_hsync, &hsync_config, false);

    uart_puts(UART_ID, "Start HSYNC!\n\r");
    
    pwm_set_enabled(slice_num_hsync, true);

    pwm_set_chan_level(slice_num_hsync, 0, hsync_duty);


}

uint16_t hsync_get_counter() {
    return (uint16_t)(pwm_hw->slice[slice_num_hsync].ctr);
}


void configure_irq()
{
    pwm_clear_irq(slice_num_hsync);    

    
    irq_set_exclusive_handler(PWM_IRQ_WRAP, hsync_irq);

    pwm_set_irq_enabled(slice_num_hsync, true);
    irq_set_enabled(PWM_IRQ_WRAP, true);

}