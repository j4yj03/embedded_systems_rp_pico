#include "servo_pwm.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/dma.h"

/* one timeout for each gpio, set as debounce_timeouts[gpio] 
static volatile absolute_time_t debounce_timeouts[NGPIO] = { 0u };
*/
static SERVO servo;

// The buffer size needs to be a power of two and alignment must be the same
//__attribute__((aligned(MAX_STRING_LEN)))
static char buffer[MAX_STRING_LEN];

struct repeating_timer delay_timer;
struct repeating_timer cont_timer;

/*
char * itoa(int value) 
{
    char result[DMA_BUFFER_SIZE];


    char* ptr = result;
    int tmp_value;

    // Apply negative sign
    if (value < 0) *ptr++ = '-';

    do 
    {
        tmp_value = value;
        value /= 10;
        *ptr++ = "9876543210123456789" [10 + (tmp_value - value)];
    } while ( value );



    return result;
}
*/
char* itoa(int value, char* result, int base) 
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do 
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) 
    {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}



/** debounce function takes gpio pin to debounce as argument and
 *  returns true if pin is currently in debounce state, false otherwise
 
bool debounce (uint gpio)
{
  // check if gpio is less than DEBOUNCE_MS from time set 
  if (get_absolute_time() < debounce_timeouts[gpio]) 
  {
    return true;
  }

  // otherwise set new debouce timout for gpio pin 
  debounce_timeouts[gpio] = make_timeout_time_ms(DEBOUNCE_MS);

  return false;
}
*/


void fprint(char * s)
{

    //while (!uart_is_writable(UART_ID))
    //        tight_loop_contents();

    //strcpy(buffer, s);
    // TO DO String länge berechnen
    memset(buffer, 0, MAX_STRING_LEN);
    memcpy(buffer, s, MAX_STRING_LEN);
    //memcpy(buffer, s, MAX_STRING_LEN);

    // dma transfer trigger
    dma_channel_configure(
        DMA_CHANNEL,    // Channel to be configured
        &config,    // The configuration we just created
        &uart_get_hw(UART_ID)->dr,  // The initial write address
        &buffer,         // The initial read address
        MAX_STRING_LEN,     // Number of transfers; in this case each is 1 byte.
        true                  // Start immediately.
    );
    //dma_channel_hw_addr(DMA_CHANNEL)->al3_read_addr_trig = (uintptr_t) buffer;
    //dma_channel_transfer_from_buffer_now(DMA_CHANNEL, buffer, DMA_SIZE_8);
    
}

void change_duty_cycle()
{
    uint duty;

    uint gpio = servo.gpio;

    char * string = (char *) calloc(MAX_STRING_LEN, sizeof(char));
    char * string_arrow = (char *) calloc(MAX_STRING_LEN, sizeof(char));
    char * string_duty = (char *) calloc(4, sizeof(char));

    switch(gpio)
    {
        default: break;
        case SERVO_BUTTON_L:
            duty = servo.duty + SERVO_STEP;
            servo.duty = duty > SERVO_MAX ? SERVO_MAX : duty;

            strcpy(string_arrow, " <--\n\r");
            
            break;
        case SERVO_BUTTON_R:
            duty = servo.duty - SERVO_STEP;
            servo.duty = duty < SERVO_MIN ? SERVO_MIN : duty;

            strcpy(string_arrow, " -->\n\r"); 
            
            break;
    }

    if (duty == servo.duty)
    {
   
        strcpy(string, strcat(itoa(duty, string_duty, 10), string_arrow));

        fprint(string);
    }

    free(string);
    free(string_arrow);
    free(string_duty);

}


bool cont_timer_isr(struct repeating_timer *t)
{

    //uint * gpio = t->user_data;

    //uint gpio = servo.gpio;

    change_duty_cycle();

    return true;
}

bool delay_timer_isr(struct repeating_timer *t)
{

    //void * gpio = t->user_data;

    
    //change_duty_cycle(&gpio);

    cancel_repeating_timer(t);

    add_repeating_timer_ms(TIMER_CONST_MS, cont_timer_isr, NULL, &cont_timer);

    return true;

}


void on_pwm_wrap() 
{
    
    // clear interrupt
    pwm_clear_irq(servo.pwm_slice_num);

    // Set dutycycle counter
    pwm_set_gpio_level(PWM_OUT, servo.duty);
}



void on_button(uint gpio, uint32_t events)
{
    /* check/register debounce on gpio 

    if (debounce (gpio)) 
    {
        return;
    }
    */
   
   servo.gpio = gpio;
    
    switch(events)
    {
        default: break;
        case GPIO_IRQ_EDGE_FALL:    // button press
                                cancel_repeating_timer(&delay_timer);
                                cancel_repeating_timer(&cont_timer);
                                break;
                                
        case GPIO_IRQ_EDGE_RISE:   // button release                              
                                change_duty_cycle();
                                add_repeating_timer_ms(TIMER_DELAY_MS, delay_timer_isr, NULL, &delay_timer);
                                break;

    }
    

}

void on_uart_rx() 
{

    while (uart_is_readable(UART_ID)) 
    {
        uint8_t ch = uart_getc(UART_ID);
        /*
        char * string = (char *) calloc(MAX_STRING_LEN, sizeof(char));

        itoa(ch, string, 10);

        fprint(string);
        */

        servo.gpio = ch - SERVO_UART_OFFSET;

        change_duty_cycle();

        //free(string);

    }

    
}


static void configure_gpio()
{
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Tell the ddfined pin is in charge of its value.
    gpio_set_function(PWM_OUT, GPIO_FUNC_PWM);

    /* initialize buttons with pull-up resistors */
    gpio_init (SERVO_BUTTON_L);
    gpio_pull_up (SERVO_BUTTON_L);

    gpio_init (SERVO_BUTTON_R);
    gpio_pull_up (SERVO_BUTTON_R);

}

static void configure_pwm()
{

    // Find out which PWM slice is connected to GPIO 16 (it's slice 16)
    uint16_t slice_num = pwm_gpio_to_slice_num(PWM_OUT);

    servo.pwm_slice_num = slice_num;
    

    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();

    // Set divider, target requeny is 50Hz or 20ms
    const float divider = _XTAL_FREQ / _PWM_FREQ / 4096 / 16; //12bit (8.4) mask + shift right 4 Bit
    pwm_config_set_clkdiv(&config, divider);

    //servo.pwm_devider = divider;

    // Set new wrap to adjust
    const uint16_t wrap = _XTAL_FREQ / divider / _PWM_FREQ - 1;
    pwm_config_set_wrap(&config, wrap);

    //servo.pwm_wrap = wrap;

    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);


    // Move Servo to middle position
    pwm_set_gpio_level(PWM_OUT, SERVO_INIT);

    
    servo.duty = SERVO_INIT;

}

static void configure_uart()
{
    // Set up our UART
    //uart_init(UART_ID, BAUD_RATE);

    //uart_reset(UART_ID);
    //uart_unreset(UART_ID);

    uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Enable the UART, both TX and RX
    uart_get_hw(UART_ID)->cr = UART_UARTCR_UARTEN_BITS | UART_UARTCR_TXE_BITS | UART_UARTCR_RXE_BITS;


    // Enable FIFOs
    //hw_set_bits(&uart_get_hw(UART_ID)->lcr_h, UART_UARTLCR_H_FEN_RESET);
    // Turn off FIFO's - we want to do this character by character
    //uart_set_fifo_enabled(UART_ID, false);

    // Transmit DMA enable
    uart_get_hw(UART_ID)->dmacr = UART_UARTDMACR_TXDMAE_BITS;

    uart_puts(UART_ID, "\n\rHello, uart!\n\r");

}



static void configure_dma() 
{
    char * string = (char *) calloc(MAX_STRING_LEN, sizeof(char));

    dma_channel_config config = dma_channel_get_default_config(DMA_CHANNEL);


    // 8 bit per transfers. read address increments after each transfer
    channel_config_set_transfer_data_size(&config, DMA_SIZE_8);
    
    channel_config_set_write_increment(&config, false);
    channel_config_set_read_increment(&config, true);

    channel_config_set_ring(&config, false, MAX_STRING_LEN);
    // set data request
    channel_config_set_dreq(&config, uart_get_dreq(UART_ID, true));



    strcpy(string, "Hello, dma!\n\r"); 

    fprint(string);

    free(string);
    
}

static void configure_irq()
{
        // set button_callback and enable
    gpio_set_irq_enabled_with_callback (SERVO_BUTTON_L, GPIO_IRQ_EDGE_FALL + GPIO_IRQ_EDGE_RISE, true, on_button);
    gpio_set_irq_enabled (SERVO_BUTTON_R, GPIO_IRQ_EDGE_FALL + GPIO_IRQ_EDGE_RISE, true);


        // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);

    irq_set_enabled(UART_IRQ, true);

    // enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    pwm_clear_irq(servo.pwm_slice_num);
    pwm_set_irq_enabled(servo.pwm_slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);

    // Enable pwm wrap interrupt
    irq_set_enabled(PWM_IRQ_WRAP, true);

}

int main()
{
  
    configure_gpio();

    configure_uart();

    configure_pwm();

    configure_dma();
    
    configure_irq();


    sleep_ms(SLEEP_MS);

    


    // Everything after this point happens in the PWM interrupt handler
    while (1)
        tight_loop_contents();

}