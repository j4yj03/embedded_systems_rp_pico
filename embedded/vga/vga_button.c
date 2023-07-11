#include "vga.h"



void change_color_param()
{
    unsigned int gpio = vga.button_gpio;

    switch(gpio)
    {
        default: break;
        case VGA_BUTTON_1: vga.color_param_1++; break;
        case VGA_BUTTON_2: vga.color_param_1--; break;
    }
    
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