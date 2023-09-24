#ifndef VGA_PIO_GPIO_H
#define	VGA_PIO_GPIO_H

// color GPIOs
#define COLOR_GPIO_OFFSET 6 // First color GPIO pin
#define COLOR_GPIO_MASK_256 (0xFF << COLOR_GPIO_OFFSET) // Color GPIO mask 8-bit
#define COLOR_GPIO_MASK_64 (0xDB << COLOR_GPIO_OFFSET) // Color GPIO mask 2-bit per channel
#define COLOR_GPIO_MASK_8 (0x49 << COLOR_GPIO_OFFSET) // Color GPIO mask 1-bit per channel


#define LED_PIN PICO_DEFAULT_LED_PIN


#define VGA_BUTTON_1 16
#define VGA_BUTTON_2 17


#define HSYNC_GPIO_OFFSET 4   // PWM Channel 2A
#define HSYNC_GPIO_MASK (1ul << HSYNC_GPIO_OFFSET)

#define VSYNC_GPIO_OFFSET 2 // V-sync GPIO pin
#define VSYNC_GPIO_MASK (1ul << VSYNC_GPIO_OFFSET)


extern void configure_gpio();

#endif