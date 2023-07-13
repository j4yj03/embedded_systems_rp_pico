#include "vga.h"



void change_color_param()
{
    unsigned int gpio = vga.button_gpio;

    switch(gpio)
    {
        default: break;
        case VGA_BUTTON_1: 
                        if (gpio_get(VGA_BUTTON_2))
                        {
                            vga.color_bit_depth_index = (vga.color_bit_depth_index + 1) % 3;
                            // drive on board LED when in 256 color mode
                            gpio_put(PICO_DEFAULT_LED_PIN, (vga.color_bit_depth_index == 0));
                            break;
                        }

                        vga.color_param_1 = (vga.color_param_1 + 1) % 17;
                        break;
        

        case VGA_BUTTON_2:

                        if (gpio_get(VGA_BUTTON_1))
                        {
                            

                            vga.color_bit_depth_index = (vga.color_bit_depth_index + 1) % 3;
                            // drive on board LED when in 256 color mode
                            gpio_put(PICO_DEFAULT_LED_PIN, (vga.color_bit_depth_index == 0));
                            break;
                        }
                        
                        
                        vga.animation = (vga.animation + 1) % 6;

                        vga.color_bit_mask = COLOR_BITMASKS[vga.color_bit_depth_index];
                        break;
                        
    }
    /*
    uart_puts(UART_ID,"\n\r");
    uart_puts(UART_ID,"color shift right: ");
    uart_puts(UART_ID, itoa(vga.color_param_1 , int_string, 10));
    uart_puts(UART_ID,"\n\r");

    uart_puts(UART_ID,"bit depth: ");
    uart_puts(UART_ID, itoa(((vga.color_bit_depth_index + 1) % 3) + 1, int_string, 10));
    uart_puts(UART_ID," (bit per channel) ");
    uart_puts(UART_ID,"\n\r");*/
    
}


void on_button(uint gpio, uint32_t events)
{
    /* check/register debounce on gpio 

    if (debounce (gpio)) 
    {
        return;
    }
    */
   
   vga.button_gpio = gpio;
    
    switch(events)
    {
        default: break;
        case GPIO_IRQ_EDGE_FALL:    // button press
                                change_color_param();
                                break;

        case GPIO_IRQ_EDGE_RISE:   // button release                              
                                
                                break;

    }
    

}


void configure_button()
{

    gpio_init (VGA_BUTTON_1);
    gpio_pull_up (VGA_BUTTON_1);

    gpio_init (VGA_BUTTON_2);
    gpio_pull_up (VGA_BUTTON_2);


    // set button_callback and enable
    gpio_set_irq_enabled_with_callback (VGA_BUTTON_1, GPIO_IRQ_EDGE_FALL + GPIO_IRQ_EDGE_RISE, true, on_button);
    gpio_set_irq_enabled (VGA_BUTTON_2, GPIO_IRQ_EDGE_FALL + GPIO_IRQ_EDGE_RISE, true);

}