#ifndef VGA_H
#define	VGA_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <string.h>

#include "vga_sync.h"
#include "vga_gpio.h"
#include "vga_uart.h"

//#include "pico/time.h"
#include "pico/stdlib.h"
#include "pico/sync.h"

#include "hardware/irq.h"
#include "hardware/gpio.h"

//#include "hardware/dma.h"
//#include "hardware/structs/systick.h"



// clocks
#define _XTAL_FREQ 125e6

#define SLEEP_MS 1000


#define FRAME_WIDTH 320 //320 //640
#define FRAME_HEIGHT 240 //240 //480

/*
    4000 clocks @ 125MHz TOP (32 µs)

    3200 clocks @ 125MHz displaytime (25.6µs)
    800 clocks @ 125MHz blanking (6.4µs) 
    
*/
#define FIRST_VISIBLE_COL 600  // 18% (3.84µs PW + 1.92µs BP)
#define LAST_VISIBLE_COL 3900 // 98% (32µs TOP - 0.640µs FP)



extern int COLOR_BITMASKS[];

extern int COLOR_ANIMATION[];

extern char int_string[];

extern int line_counter;

extern int frame_counter;

extern int color;

extern int color_shift_bits;
extern int color_bit_depth_index;
extern int color_gpoio_mask;

extern int animation;

extern bool hsync_high;

extern int first_visible_col;

extern int last_visible_col;


extern semaphore_t display_on; 



#endif