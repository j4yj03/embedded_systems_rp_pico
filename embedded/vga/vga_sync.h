#ifndef VGA_SYNC_H
#define	VGA_SYNC_H

// Clocks
#define _HSYNC_PWM_FREQ 31250
#define _VSYNC_PWM_FREQ 59.88

// GPIOs

#define HSYNC_A 9
#define HSYNC_B 8
#define VSYNC_A 7
#define VSYNC_B 6


// Duty cycles

#define HSYNC_DC 0.12








static inline void configure_pwm_hsync();

static inline void configure_pwm_vsync();

#endif