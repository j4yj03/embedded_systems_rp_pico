#ifndef VGA_SYNC_H
#define	VGA_SYNC_H

// Clocks
#define _HSYNC_PWM_FREQ 31250
#define _VSYNC_PWM_FREQ 59.88

// GPIOs

#define HSYNC_B 9   // PWM Channel 4B
#define HSYNC_A 8   // PWM Channel 4A
#define VSYNC_B 7   // PWM Channel 3B
#define VSYNC_A 6   // PWM Channel 3A


// Duty cycles

#define HSYNC_DC 0.12




static uint16_t slice_num_hsync;

static uint16_t slice_num_vsync;

static pwm_config hsync_config;

static pwm_config vsync_config;



static inline void configure_pwm_hsync();

static inline void configure_pwm_vsync();

#endif