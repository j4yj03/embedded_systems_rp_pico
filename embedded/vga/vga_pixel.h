#ifndef VGA_PIXEL_H
#define	VGA_PIXEL_H

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


#define DISPLAY_WIDTH 640 // Frame width in picture
#define DISPLAY_HEIGHT 480 // Frame height in picture









extern void configure_gpio();
extern void configure_dma_px();
extern void configure_pwm_px();
extern void output_colorbits();
extern void calculate_colorbits();

extern int COLOR_BITMASKS[];

extern int COLOR_ANIMATION[];


#endif