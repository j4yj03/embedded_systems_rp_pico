#ifndef VGA_UART_H
#define	VGA_UART_H


// GPIOs
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// uart defines
#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE


extern void configure_uart();


#endif