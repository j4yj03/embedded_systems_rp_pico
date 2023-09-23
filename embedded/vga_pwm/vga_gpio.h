#ifndef VGA_GPIO_H
#define	VGA_GPIO_H

// clock
#define _PIXEL_FREQ 25e6

// clock GPIO
#define PX_CLOCK 4  // PWM Channel 2A

//duty cycle
#define PX_DC 0.5

// color GPIOs
#define COLOR_GPIO_OFFSET 6 // First color GPIO pin
#define COLOR_GPIO_MASK_256 (0xFF << COLOR_GPIO_OFFSET) // Color GPIO mask 8-bit
#define COLOR_GPIO_MASK_64 (0xDB << COLOR_GPIO_OFFSET) // Color GPIO mask 2-bit per channel
#define COLOR_GPIO_MASK_8 (0x49 << COLOR_GPIO_OFFSET) // Color GPIO mask 1-bit per channel



// DMA
#define DMA_CHANNEL 0


#define LED_PIN PICO_DEFAULT_LED_PIN


#define VGA_BUTTON_1 16
#define VGA_BUTTON_2 17


extern void configure_button();




extern void configure_gpio();
//extern void configure_dma_px();
//extern void configure_pwm_px();





#endif