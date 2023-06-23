#ifndef VGA_PIXEL_H
#define	VGA_PIXEL_H

// clock
#define _PIXEL_FREQ 25e6

// clock GPIO
#define PX_CLOCK 4  // PWM Channel 2A

// color GPIOs
#define COLOR_GPIO_OFFSET 16 // First color GPIO pin
#define COLOR_GPIO_MASK (0x3F << COLOR_GPIO_OFFSET) // Color GPIO mask


// DMA
#define DMA_CHANNEL 0


#define FRAME_WIDTH 640 // Frame width in picture
#define FRAME_HEIGHT 480 // Frame height in picture



static dma_channel_config dma_config;

static uint16_t slice_num_px;

static pwm_config px_config;


#endif