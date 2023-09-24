# VGA PIO

## Überblick

Dieses Projekt realisiert eine VGA Ausgabe auf dem rp2040 mithilfe der Pico spezifischen PIO Co-Prozessoren.

> `vga_pio.c` main funktion
<br>

> `vga_pio_uart.c` Initialisierung uart
<br>

> `vga_pio_gpio.c` Initialisierung gpio und button irq handel
<br>

> `vga_pio_dma.c`  Initialisirung der DMA channels
<br>

> `vga_pio_draw.c` Funktionen zum befüllen des VGA scanline buffers
<br>

> `vga_hsync.pio` 
<br>

> `vga_vsync.pio`
<br>

> `vga_rgb.pio`
<br>

> `picture.py`
<br>