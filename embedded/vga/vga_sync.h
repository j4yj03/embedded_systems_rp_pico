#ifndef VGA_SYNC_H
#define	VGA_SYNC_H

// Clocks
#define _HSYNC_PWM_FREQ 31250
#define _VSYNC_PWM_FREQ 60

// GPIOs

#define HSYNC_B 9   // PWM Channel 4B
#define HSYNC_A 8   // PWM Channel 4A

#define V_SYNC_GPIO_OFFSET 6 // V-sync GPIO pin
#define V_SYNC_GPIO_MASK (1ul << V_SYNC_GPIO_OFFSET)

//#define VSYNC_B 7   // PWM Channel 3B
//#define VSYNC_A 6   // PWM Channel 3A


// Duty cycles

#define HSYNC_DC 0.12

#define HSYNC_BLANKING_DC 0.18

#define VSYNC_BP_LINES 29
#define VSYNC_FP_LINES 10
#define VSYNC_DISPLAY_LINES 480
#define VSYNC_PW_LINES 2
#define VSYNC_TOTAL_LINES 521
#define VSYNC_NPW_LINES 519

#define VSYNC_DC 0.003838






extern void configure_pwm_hsync();
extern void configure_pwm_vsync();
extern void start_pwm_sync();

extern void configure_irq();

extern uint16_t hsync_get_counter();

#endif