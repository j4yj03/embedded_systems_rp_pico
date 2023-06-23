#include "vga.h"
#include "vga_pixel.h"


/** \brief configure gpio for color output
 *   
*/
void configure_color_gpio() {

    gpio_init_mask(COLOR_GPIO_MASK);
    gpio_set_dir_out_masked(COLOR_GPIO_MASK);
}


/** \brief configure dma channel for transfering pixel data
 *   
 *  \param pwm_slice_num_px PWM Slice number
*/
static inline void configure_dma_px(uint16_t pwm_slice_num_px)
{
     dma_channel_config config = dma_channel_get_default_config(DMA_CHANNEL);


    // 8 bit per transfers. read address increments after each transfer
    channel_config_set_transfer_data_size(&config, DMA_SIZE_8);
    
    channel_config_set_write_increment(&config, false);
    channel_config_set_read_increment(&config, true);

    channel_config_set_ring(&config, false, MAX_STRING_LEN);
    // set data request
    channel_config_set_dreq(&config, pwm_get_dreq(pwm_slice_num_px, true));

}

/** \brief configure pwm channel for pixel clock
 *   
 *  \return PWM slice number of pixel clock
*/
static inline uint16_t configure_pwm_px()
{
    float px_divider = _XTAL_FREQ / _PIXEL_FREQ / 4096 / 16;

    uint16_t px_wrap = _XTAL_FREQ / vsync_divider / _PIXEL_FREQ - 1;

    uint16_t slice_num_px = pwm_gpio_to_slice_num(PX_CLOCK);

    pwm_config px_config = {0, 0, 0};

    pwm_config_set_phase_correct(&px_config, false);
    pwm_config_set_clkdiv(&px_config, px_divider);
    pwm_config_set_clkdiv_mode(&px_config, PWM_DIV_FREE_RUNNING);
    pwm_config_set_output_polarity(&px_config, false, false);
    pwm_config_set_wrap(&px_config, px_wrap);

    pwm_init(slice_num_px, &px_config, false);

    return slice_num_px;
}
