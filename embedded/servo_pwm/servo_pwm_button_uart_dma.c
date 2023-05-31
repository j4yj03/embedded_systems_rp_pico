#include "servo_pwm.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include <hardware/dma.h>

/* one timeout for each gpio, set as debounce_timeouts[gpio] 
static volatile absolute_time_t debounce_timeouts[NGPIO] = { 0u };
*/
static SERVO servo;

// The buffer size needs to be a power of two and alignment must be the same
__attribute__((aligned(8)))
static char buffer[8];

struct repeating_timer delay_timer;
struct repeating_timer cont_timer;


char* itoa(int value) 
{
    char result[8];


    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';

    do 
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );



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


void change_duty_cycle()
{
    uint duty;

    uint gpio = servo.gpio;

    switch(gpio)
    {
        default: break;
        case SERVO_BUTTON_L:
            duty = servo.duty + SERVO_STEP;
            servo.duty = duty > SERVO_MAX ? SERVO_MAX : duty;

            buffer = "--> " + itoa(duty) + "\n";

            break;
        case SERVO_BUTTON_R:
            duty = servo.duty - SERVO_STEP;
            servo.duty = duty < SERVO_MIN ? SERVO_MIN : duty;

            buffer = "<--" + itoa(duty) + "\n";
            break;
    }

    /*

    uart_puts(UART_ID, "GPIO: ");
    uart_puts(UART_ID, itoa(gpio, buffer, 10));
    uart_puts(UART_ID, "\n\r");
    uart_puts(UART_ID, "DC: ");
    uart_puts(UART_ID, itoa(duty, buffer, 10));
    uart_puts(UART_ID, "\n\r");
    */
}


bool cont_timer_isr(struct repeating_timer *t)
{

    //uint * gpio = t->user_data;

    uint gpio = servo.gpio;

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
    
    switch(events)
    {
        default: break;
        case GPIO_IRQ_EDGE_FALL:    // button press
                                cancel_repeating_timer(&delay_timer);
                                cancel_repeating_timer(&cont_timer);
                                break;
        case GPIO_IRQ_EDGE_RISE:   // button release
                                change_duty_cycle();
                                servo.gpio = gpio;
                                add_repeating_timer_ms(TIMER_DELAY_MS, delay_timer_isr, NULL, &delay_timer);
                                break;

    }
    

}

void on_uart_rx() 

{

    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);

        switch(ch)
        {
            default:break;

            case 'a':
                        change_duty_cycle(18);
                        break;
            case 'd':   change_duty_cycle(19);
                        break;
        }

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
    uart_init(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);

    uart_puts(UART_ID, "\n\rHello, Servo!\n\r");

}



static void configure_dma() 
{
    dma_channel_config config = dma_channel_get_default_config(DMA_CHANNEL);
    channel_config_set_transfer_data_size(&config, DMA_SIZE_8);

    // The read and write address are constant
    channel_config_set_read_increment(&config, false);
    channel_config_set_write_increment(&config, false);

    // 
    channel_config_set_dreq(&config, DREQ_UART0_TX);

    // Transmit '2^32 - 1' symbols, this should suffice for any practical case,
    // otherwise, the channel could be triggered again
    dma_channel_configure(
        channel,    // Channel to be configured
        &config,    // The configuration we just created
        &uart0_hw->dr,  // The initial write address
        &buffer,         // The initial read address
        count_of(buffer),     // Number of transfers; in this case each is 1 byte.
        true                  // Start immediately.
    );
}

static void configure_irq()
{
        // set button_callback and enable
    gpio_set_irq_enabled_with_callback (SERVO_BUTTON_L, GPIO_IRQ_EDGE_FALL + GPIO_IRQ_EDGE_RISE, true, on_button);
    gpio_set_irq_enabled (SERVO_BUTTON_R, GPIO_IRQ_EDGE_FALL + GPIO_IRQ_EDGE_RISE, true);

    irq_set_enabled(UART_IRQ, true);

    // enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
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