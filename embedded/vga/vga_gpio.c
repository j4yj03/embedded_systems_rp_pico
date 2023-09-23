#include "vga.h"

    


void change_color_param(unsigned int gpio)
{

    switch(gpio)
    {
        default: break;
        case VGA_BUTTON_1: 
                        if (gpio_get(VGA_BUTTON_2))
                        {
                            color_bit_depth_index = (color_bit_depth_index + 1) % 3;
                            // drive on board LED when in 256 color mode
                            gpio_put(PICO_DEFAULT_LED_PIN, (color_bit_depth_index == 0));
                            break;
                        }

                        color_param_1 = (color_param_1 + 1) % 20;
                        break;
        

        case VGA_BUTTON_2:

                        if (gpio_get(VGA_BUTTON_1))
                        {
                            

                            color_bit_depth_index = (color_bit_depth_index + 1) % 3;
                            // drive on board LED when in 256 color mode
                            gpio_put(PICO_DEFAULT_LED_PIN, (color_bit_depth_index == 0));
                            break;
                        }
                        
                        
                        animation = (animation + 1) % 6;

                        color_bit_mask = COLOR_BITMASKS[color_bit_depth_index];
                        break;
                        
    }    
    uart_puts(UART_ID,"\n\ranimation: ");
    uart_puts(UART_ID, itoa(animation, int_string, 10));
    uart_puts(UART_ID," color_param_1: ");
    uart_puts(UART_ID, itoa(color_param_1, int_string, 10));
}


void on_button(uint gpio, uint32_t events)
{
    /* check/register debounce on gpio 

    if (debounce (gpio)) 
    {
        return;
    }
    */

    
    switch(events)
    {
        default: break;
        case GPIO_IRQ_EDGE_FALL:    // button press
                                change_color_param(gpio);
                                break;

        case GPIO_IRQ_EDGE_RISE:   // button release                              
                                
                                break;

    }
    

}  

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


    // buttons

    gpio_init (VGA_BUTTON_1);
    gpio_pull_up (VGA_BUTTON_1);

    gpio_init (VGA_BUTTON_2);
    gpio_pull_up (VGA_BUTTON_2);


    // set button_callback and enable
    gpio_set_irq_enabled_with_callback (VGA_BUTTON_1, GPIO_IRQ_EDGE_FALL + GPIO_IRQ_EDGE_RISE, true, on_button);
    gpio_set_irq_enabled (VGA_BUTTON_2, GPIO_IRQ_EDGE_FALL + GPIO_IRQ_EDGE_RISE, true);

    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    uart_puts(UART_ID,"\n\rHello GPIO!");

}
