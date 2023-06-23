#ifndef VGA_PIXEL_H
#define	VGA_PIXEL_H

// clock
#define _PIXEL_FREQ 25e6

// clock GPIO
#define PX_CLOCK 5

// color GPIOs
#define COLOR_GPIO_OFFSET 16 // First color GPIO pin
#define COLOR_GPIO_MASK (0x3F << COLOR_GPIO_OFFSET) // Color GPIO mask


// DMA
#define DMA_CHANNEL 0


#endif