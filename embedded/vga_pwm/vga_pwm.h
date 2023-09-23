#ifndef VGA_H
#define	VGA_H

//#define VGA_UART_DEBUG


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <string.h>

#include "vga_pwm_sync.h"
#include "vga_pwm_gpio.h"
#include "vga_pwm_uart.h"

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


#define FRAME_WIDTH 324 //320 //640
#define FRAME_HEIGHT 240 //240 //480

extern char int_string[];

extern int line_counter;
extern int frame_counter;

extern int COLOR_BITMASKS[];

extern int color;
extern int color_shift_bits;
extern int color_bit_depth_index;
extern int color_gpoio_mask;
extern int color_animation;

extern int FIRST_VISIBLE_COL;
extern int LAST_VISIBLE_COL;


extern semaphore_t display_on; 



#endif