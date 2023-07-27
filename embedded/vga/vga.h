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


extern int COLOR_BITMASKS[];

extern int COLOR_ANIMATION[];

extern char int_string[];

extern int line_counter;

extern int frame_counter;

extern int color;

extern int color_param_1;
extern int color_bit_depth_index;
extern int color_bit_mask;

extern int animation;

extern bool hsync_high;

extern int first_visible_col;

extern int last_visible_col;


extern semaphore_t display_on; 



#endif