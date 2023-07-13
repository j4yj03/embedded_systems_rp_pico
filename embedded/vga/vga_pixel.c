#include "vga.h"



/** \brief configure gpio for color output
 *   
*/
inline void configure_gpio() {

    gpio_init_mask(COLOR_GPIO_MASK_256 | V_SYNC_GPIO_MASK);
    gpio_set_dir_out_masked(COLOR_GPIO_MASK_256 | V_SYNC_GPIO_MASK);

    //gpio_init_mask(V_SYNC_GPIO_MASK);
    //gpio_set_dir_out_masked(V_SYNC_GPIO_MASK);

    gpio_clr_mask(V_SYNC_GPIO_MASK | COLOR_GPIO_MASK_256);
    //gpio_clr_mask(COLOR_GPIO_MASK);


    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
}


/** \brief configure dma channel for transfering pixel data
 *   
 * 

inline void configure_dma_px()
{
    dma_config = dma_channel_get_default_config(DMA_CHANNEL);


    // 8 bit per transfers. read address increments after each transfer
    channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_8);
    
    channel_config_set_write_increment(&dma_config, false);
    channel_config_set_read_increment(&dma_config, true);

    //channel_config_set_ring(&dma_config, false, MAX_STRING_LEN);
    // set data request
    channel_config_set_dreq(&dma_config, pwm_get_dreq(slice_num_px));

}

** \brief configure pwm channel for pixel clock
 *   
 *

inline void configure_pwm_px()
{
    float px_divider = _XTAL_FREQ / _PIXEL_FREQ / 4096 / 16;

    uint16_t px_wrap = _XTAL_FREQ / px_divider / _PIXEL_FREQ - 1;

    slice_num_px = pwm_gpio_to_slice_num(PX_CLOCK);

    px_duty = px_wrap * PX_DC;    // ????????

    pwm_config_set_phase_correct(&px_config, false);
    pwm_config_set_clkdiv(&px_config, px_divider);
    pwm_config_set_clkdiv_mode(&px_config, PWM_DIV_FREE_RUNNING);
    pwm_config_set_output_polarity(&px_config, false, false);
    pwm_config_set_wrap(&px_config, px_wrap);

    pwm_init(slice_num_px, &px_config, false);

}



** \brief function to configure read and write adress for DMA and start DMA channel
 *   

void output_color() {

    uint8_t * writeaddr; 

    uint8_t * readaddr;

    uint16_t vertical_counter = pwm_get_counter(slice_num_vsync) & FRAME_HEIGHT;

    readaddr = &framebuffer[vertical_counter][0];

    writeaddr = (uint8_t*) sio_hw->gpio_out; //+ (0x3F << COLOR_GPIO_OFFSET);

    // Set color output
    //sio_hw->gpio_togl = (sio_hw->gpio_out ^ (color << COLOR_GPIO_OFFSET)) & (COLOR_GPIO_MASK);

        // dma transfer trigger
    dma_channel_configure(
        DMA_CHANNEL,    // channel
        &dma_config,    // configuration
        ++writeaddr,      // write address
        readaddr,       // read address
        FRAME_WIDTH + 1,    // 640 pixel transfers + 1 blanking
        true            // start immediately.
    );


}

** \brief function to calculate color bytes
 *   
 *  

void calculate_color(uint16_t vertical_counter)
{
    uint8_t idx;

    for(int i = 0; i <= FRAME_WIDTH + 1; i++)
    {
        idx = (uint8_t) (i + 1) / 10;
        framebuffer[vertical_counter][i] = colormap[idx] * (i < FRAME_WIDTH);
    }
    
}
*/


/** \brief function to set color bits
 *   
 *  
*/
void calculate_colormasks() {

    

    //sio_hw->gpio_togl = (sio_hw->gpio_out ^ (color << COLOR_GPIO_OFFSET)) & (COLOR_GPIO_MASK);

}
/*

int bits[8] = {0x0, 0x07, 0x3F, 0x38, 0xF8, 0xF, 0xC0, 0xFF};
int check[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};
*/
/** \brief function to calculate color bytes
 *   
 *  

inline void calculate_colorbits() 
{

    int temp;

    unsigned int hysnc_count = hsync_get_counter();

    switch (vga.animation)
    {
        default: break;

        case 0: // horizontale Streifen
            temp = (int) (vga.line_counter / 64); 
            vga.color =  bits[temp % 8]; 
            gpio_put_masked(vga.color_bit_mask, (vga.color << COLOR_GPIO_OFFSET)); 
            break;
            
        case 1: // vertikale Streifen
                while(hsync_get_counter() < 3800)
                {
                    temp = (int) (~(hsync_get_counter()) / (4096 >> vga.color_param_1)); 

                    gpio_put_masked(vga.color_bit_mask, (bits[temp % 8] << COLOR_GPIO_OFFSET)); 
                };
                break;

        case 2: // schachbrett
            while(hsync_get_counter() < 3800)
                {
                    temp = (int) (~(hsync_get_counter()) / (4096 >> vga.color_param_1)); 

                    gpio_put_masked(vga.color_bit_mask, (bits[temp % 8] << COLOR_GPIO_OFFSET)); 
                };
                break;

        case 3: 
            vga.color = (((vga.frame_counter ^ vga.line_counter) |~ 0x1F8) >> vga.color_param_1) & (0xFF); 
            gpio_put_masked(vga.color_bit_mask, (vga.color << COLOR_GPIO_OFFSET)); 
            break;

        case 4: vga.color = (((vga.frame_counter * ((hsync_get_counter() - 600) * 3 ^~ vga.line_counter)))  >> (vga.color_param_1 + 13)) & (0xFF);
                gpio_put_masked(vga.color_bit_mask, (vga.color << COLOR_GPIO_OFFSET)); 
                
                break;   

        

        case 5: vga.color = (vga.frame_counter * (((hsync_get_counter()) *~ vga.line_counter)) >> (vga.color_param_1 + 20)) & (0xFF);
                gpio_put_masked(vga.color_bit_mask, (vga.color << COLOR_GPIO_OFFSET)); 
                break;

}
*/    