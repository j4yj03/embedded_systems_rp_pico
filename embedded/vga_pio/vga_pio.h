#ifndef VGA_PIO_H
#define	VGA_PIO_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/dma.h"

#include "vga_pio_dma.h"
#include "vga_pio_gpio.h"
#include "vga_pio_uart.h"

// pioasm assembled programs:
#include "vga_hsync.pio.h"
#include "vga_vsync.pio.h"
#include "vga_rgb.pio.h"


// VGA timing constants
#define HSYNC_ACTIVE   655    // (active + frontporch - 1) - one cycle delay for mov
#define VSYNC_ACTIVE   479    // (active - 1)
#define RGB_ACTIVE 319    // (horizontal active)/2 - 1
// #define RGB_ACTIVE 639 // change to this if 1 pixel/byte


#define FRAME_WIDTH 480 //320 //640
#define FRAME_HEIGHT 320 //240 //480

#define N_PIXEL (FRAME_WIDTH * FRAME_HEIGHT) 

// pio state machine idx
#define HSYNC_PIO_SM 0
#define VSYNC_PIO_SM 1
#define RGB_PIO_SM 2


// Pixel color array that is DMA's to the PIO machines and
// a pointer to the ADDRESS of this color array.
// Note that this array is automatically initialized to all 0's (black)
extern char vga_data_array[N_PIXEL];


extern PIO pio;

extern int COLOR_BITMASKS[];
extern int color_bitmask_idx;
extern int color_gpoio_bitmask;
extern int color_shift_bits;
extern int color_animation;

#endif