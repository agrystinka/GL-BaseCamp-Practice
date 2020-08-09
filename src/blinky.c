#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdint.h>
#include <stdbool.h>

//This file will be devided on few .c and .h
//It`s work version

/*TO DO: add invert flag for buttons*/

/**
 * struct pin_s.
 * @gpio: Port from A to K.
 *        Size: 4 bits
 *        A = 0000, B = 0001, C = 0010, D = 0011, E = 0100,
 *        F = 0101, G = 0111, H = 1000, I = 1001, J = 1010,
 *        K = 1011, 1100 - 1111 are invalid codes
 * @pin: pin of port from 0 to 15
 *       Size: 4 bits
 * @inverse:
 *           Size: 1 bit
 *           if 0 - ACTIVE HIGHT
 *           if 1 - ACTIVE LOW
 * @mode:
 *      Size: 2 bits
 *      if 00 - input
 *      if 01 - output
 *      if 10 - alternative function
 *      if 11 - analog
 * @pullupdown:
 *      Size: 2 bits
 *      if 00 - none
 *      if 01 - pullup
 *      if 10 - pulldown
 *      if 11 - invalid code
 * @reserved: reserved for future use
 *      Size: 3 bits
 *
 *
 * pin_s is a packed structure.
 */
 struct  __attribute__((__packed__)) pin_s {
     unsigned int port : 4;
     unsigned int pin : 4;
     unsigned int inverse : 1;
     unsigned int mode : 2;
     unsigned int pullupdown : 2;
     unsigned int reserved : 3
 };

 enum PERIPHERAL_DEVICES {
     //user button on STM BOARD
     BTN_BLUE = 0,
     //LEDs on STM BOARD, the numbers are equal to LDx on board
     LED_ORANGE = 3,
     LED_GREEN = 4,
     LED_RED = 5,
     LED_BLUE = 6,
     //buttons on GLSK_BOARD
     BTN_SWT1, BTN_SWT2, BTN_SWT3, BTN_SWT4, BTN_SWT5 //6-10
 };

 enum MODE {
     //the same that in orginal GPIO_MODE
     INPUT = 0,
     OUTPUT = 1,
     AF = 2,
     ANALOG = 3
 };

 enum PULLUPDOWN {
     //the same that in orginal GPIO_PUPD
     NONE = 0,
     PULLUP = 1,
     PULLDOWN = 2
 };

//Core functions of my lib
uint32_t get_pin(uint8_t peripheral_device);
uint32_t get_port(uint8_t peripheral_device);
//Basic functions
void clear(uint8_t peripheral_device);
void set(uint8_t peripheral_device);
void toggle(uint8_t peripheral_device);
void mode_setup(uint8_t peripheral_device, uint8_t mode, uint8_t pullupdown);
//Work with LEDs
void switch_on_led(uint8_t led);
void switch_off_led(uint8_t led);
void switch_led(uint8_t led);
//Work with buttons
bool is_btn_pressed(uint8_t btn);
//Extra
void softdelay(volatile uint32_t N);

int main(void)
{
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOC);

    mode_setup(LED_GREEN, OUTPUT, NONE);
    mode_setup(LED_BLUE, OUTPUT, NONE);
    mode_setup(BTN_SWT1, INPUT, PULLUP);

    switch_on_led(LED_GREEN);
    switch_on_led(LED_BLUE);

    while (1) {
    //    sample 1
        if(is_btn_pressed(BTN_SWT1))
            switch_led(LED_GREEN);
    }
}

void softdelay(volatile uint32_t N)
{
    while (N--);
}

//Work with LEDs-----------------------------------------------------
void switch_on_led(uint8_t led)
{
    gpio_set(get_port(led), get_pin(led));
}

void switch_off_led(uint8_t led)
{
    gpio_clear(get_port(led), get_pin(led));
}

void switch_led(uint8_t led)
{
    gpio_toggle(get_port(led), get_pin(led));
}

//Basic functions-----------------------------------------------------------
void clear(uint8_t peripheral_device)
{
    gpio_clear(get_port(peripheral_device), get_pin(peripheral_device));
}

void set(uint8_t peripheral_device)
{
    gpio_set(get_port(peripheral_device), get_pin(peripheral_device));
}

void toggle(uint8_t peripheral_device)
{
    gpio_toggle(get_port(peripheral_device), get_pin(peripheral_device));
}

void mode_setup(uint8_t peripheral_device, uint8_t mode, uint8_t pullupdown)
{
    gpio_mode_setup(get_port(peripheral_device), mode, pullupdown, get_pin(peripheral_device));
}

//Work with buttons-----------------------------------------------------
bool is_btn_pressed(uint8_t btn)
{
    uint32_t btn_port = get_port(btn);
    uint32_t btn_pin = get_pin(btn);

    //debouncing
    int cnt = 0;
    for(int i = 0; i < 10; i++)
        if(!(gpio_port_read(btn_port) & btn_pin))
            cnt++;

    if(cnt >= 5){
        while(!(gpio_port_read(btn_port) & btn_pin));
        return true;
    }
    else
        return false;
}

//Core functions of mygpiolib
uint32_t get_pin(uint8_t peripheral_device)
{
    if (peripheral_device == BTN_BLUE)     return  GPIO0;
    if (peripheral_device == LED_GREEN)    return  GPIO12;
    if (peripheral_device == LED_ORANGE)   return  GPIO13;
    if (peripheral_device == LED_RED)      return  GPIO14;
    if (peripheral_device == LED_BLUE)     return  GPIO15;
    if (peripheral_device == BTN_SWT1)     return  GPIO11;
    if (peripheral_device == BTN_SWT2)     return  GPIO15;
    if (peripheral_device == BTN_SWT3)     return  GPIO9;
    if (peripheral_device == BTN_SWT4)     return  GPIO6;
    if (peripheral_device == BTN_SWT5)     return  GPIO8;
}

uint32_t get_port(uint8_t peripheral_device)
{
    if (peripheral_device == BTN_BLUE)     return  GPIOA;
    if (peripheral_device == LED_GREEN)    return  GPIOD;
    if (peripheral_device == LED_ORANGE)   return  GPIOD;
    if (peripheral_device == LED_RED)      return  GPIOD;
    if (peripheral_device == LED_BLUE)     return  GPIOD;
    if (peripheral_device == BTN_SWT1)     return  GPIOC;
    if (peripheral_device == BTN_SWT2)     return  GPIOA;
    if (peripheral_device == BTN_SWT3)     return  GPIOC;
    if (peripheral_device == BTN_SWT4)     return  GPIOC;
    if (peripheral_device == BTN_SWT5)     return  GPIOC;
}
