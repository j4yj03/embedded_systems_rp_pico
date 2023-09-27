# VGA PWM

## Überblick

Dieses Projekt realisiert eine VGA Ausgabe auf dem rp2040 mithilfe einer taktgebenden PWM.

- vga_pwm.c
![image info](../uml/main_vga.png)

- vga_pwm_uart.c
- vga_pwm_sync.c
![image info](../uml/configure_pwm_hsync.png)
![image info](../uml/irq_pwm_hsync.png)

- vga_pwm_gpio.c 