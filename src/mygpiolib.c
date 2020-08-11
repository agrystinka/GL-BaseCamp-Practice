#include "mygpiolib.h"
#include <libopencm3/stm32/gpio.h>
#include <stdint.h>
#include <stdbool.h>

//Internal functions of mygpiolib
uint32_t mgl_get_gpio_port(uint8_t port);
uint32_t mgl_get_gpio_pin(uint8_t pin);

//Definition of peripheral devices' pins

// some STM32F4DISCOVERY peripheral devices' pins
const mgl_pin mgl_led_orange = { .port=MGL_PORT_D, .pin=13, .inverse=0};
const mgl_pin mgl_led_red    = { .port=MGL_PORT_D, .pin=14, .inverse=0};
const mgl_pin mgl_led_green  = { .port=MGL_PORT_D, .pin=12, .inverse=0};
const mgl_pin mgl_led_blue   = { .port=MGL_PORT_D, .pin=15, .inverse=0};
const mgl_pin mgl_btn_usr    = { .port=MGL_PORT_A, .pin=0,  .inverse=0};
// some GLSK peripheral devices' pins
const mgl_pin mgl_btn_swt1   = { .port=MGL_PORT_C, .pin=11, .inverse=1};
const mgl_pin mgl_btn_swt2   = { .port=MGL_PORT_A, .pin=15, .inverse=1};
const mgl_pin mgl_btn_swt3   = { .port=MGL_PORT_C, .pin=9,  .inverse=1};
const mgl_pin mgl_btn_swt4   = { .port=MGL_PORT_C, .pin=6,  .inverse=1};
const mgl_pin mgl_btn_swt5   = { .port=MGL_PORT_C, .pin=8,  .inverse=1};

/**
 * An abstraction over basic GPIO functions.
 */

/**
 * mgl_clear() - set pin to 0.
 * @mgl_pin periph: peripheral device.
 *
 * Clear pin on port which are used by peripheral device.
 *
 * Return: void.
 */
void mgl_clear(mgl_pin periph)
{
    gpio_clear(mgl_get_gpio_port(periph.port), mgl_get_gpio_pin(periph.pin));
}

/**
 * mgl_set() - set pin to 1.
 * @mgl_pin periph: peripheral device.
 *
 * Set pin on port which are used by peripheral device.
 *
 * Return: void.
 */
void mgl_set(mgl_pin periph)
{
    gpio_set(mgl_get_gpio_port(periph.port), mgl_get_gpio_pin(periph.pin));
}

/**
 * mgl_toggle - invert pin.
 * @mgl_pin periph: peripheral device.
 *
 * Invert pin on port which are used by peripheral device.
 *
 * Return: void.
 */
void mgl_toggle(mgl_pin periph)
{
    gpio_toggle(mgl_get_gpio_port(periph.port), mgl_get_gpio_pin(periph.pin));
}

/**
 * mgl_set_value() - set pin to 1.
 * @mgl_pin periph: peripheral device.
 * @bool value: 0 or 1.
 *
 * Set specific value into pin on port which are used by peripheral device.
 *
 * Return: void.
 */
void mgl_set_value(mgl_pin periph, bool value)
{
    if(value == true)
        gpio_set(mgl_get_gpio_port(periph.port), mgl_get_gpio_pin(periph.pin));
    else if(value == false)
        gpio_clear(mgl_get_gpio_port(periph.port), mgl_get_gpio_pin(periph.pin));
}

/**
 * mgl_port_read() - read from peripheral device.
 * @mgl_pin periph: peripheral device.
 *
 * Be careful, because this function reads from port on which device work,
 * not on specific pin of this port.
 *
 * Return: uint16_t data.
 */
uint16_t mgl_port_read(mgl_pin periph)
{
    return gpio_port_read(mgl_get_gpio_port(periph.port));
}

/**
 * mgl_port_read() - write to peripheral device.
 * @mgl_pin periph: peripheral device.
 *
 * Be careful, because this function write to port on which device work,
 * not to specific pin of this port.
 *
 * Return: void.
 */
void mgl_port_write(mgl_pin periph, uint16_t data)
{
    gpio_port_write(mgl_get_gpio_port(periph.port), data);
}

/**
 * mgl_mode_setup_default() - setup device in default mode.
 * @mgl_pin periph: peripheral device.
 *
 * If device is LED, pin direction will be OUTPUT and output pin pullup will be NONE
 * If device is button, pin direction will be INPUT and output pin pullup will be PULLUP
 *
 * Return: void.
 */
void mgl_mode_setup_default(mgl_pin periph)
{
    uint8_t mode, pull_up_down;
    uint32_t port = mgl_get_gpio_port(periph.port);
    uint32_t pin = mgl_get_gpio_pin(periph.pin);

    //if periph is a LED
    if((periph.port == mgl_led_orange.port && periph.pin == mgl_led_orange.pin) ||
       (periph.port == mgl_led_green.port && periph.pin == mgl_led_green.pin) ||
       (periph.port == mgl_led_red.port && periph.pin == mgl_led_red.pin) ||
       (periph.port == mgl_led_blue.port && periph.pin == mgl_led_blue.pin)) {
        mode = GPIO_MODE_OUTPUT;
        pull_up_down = GPIO_PUPD_NONE;
    }

    //if periph is a button
    else
    if((periph.port == mgl_btn_usr.port && periph.pin == mgl_btn_usr.pin) ||
       (periph.port == mgl_btn_swt1.port && periph.pin == mgl_btn_swt1.pin) ||
       (periph.port == mgl_btn_swt2.port && periph.pin == mgl_btn_swt2.pin) ||
       (periph.port == mgl_btn_swt3.port && periph.pin == mgl_btn_swt3.pin) ||
       (periph.port == mgl_btn_swt4.port && periph.pin == mgl_btn_swt4.pin) ||
       (periph.port == mgl_btn_swt5.port && periph.pin == mgl_btn_swt5.pin)) {
        mode = GPIO_MODE_INPUT;
        pull_up_down = GPIO_PUPD_PULLUP;
    }
    else{
        mode = GPIO_MODE_OUTPUT;
        pull_up_down = GPIO_PUPD_NONE;
    }

    gpio_mode_setup(mgl_get_gpio_port(periph.port), mode, pull_up_down,
		mgl_get_gpio_pin(periph.pin));
}

/**
 * mgl_mode_setup() - setup device in default mode.
 * @mgl_pin periph: peripheral device.
 * @uint8_t mode: from 0 to 3 or GPIO constants:
 *                http://libopencm3.org/docs/latest/stm32f4/html/group__gpio__mode.html.
 * @uint8_t pull_up_down: from 0 to 2 or GPIO constants:
 *                        http://libopencm3.org/docs/latest/stm32f4/html/group__gpio__pup.html
 *
 * If you call this function with invalid arguments it will work as mgl_mode_setup_default()
 *
 * Return: void.
 */
void mgl_mode_setup(mgl_pin periph, uint8_t mode, uint8_t pull_up_down)
{
    //check for valid data
    //if invalid use default setup
    if(mode != GPIO_MODE_INPUT && mode != GPIO_MODE_OUTPUT &&
       mode != GPIO_MODE_AF && mode != GPIO_MODE_ANALOG)
        mgl_mode_setup_default(periph);
    else if(pull_up_down != GPIO_PUPD_NONE &&
            pull_up_down != GPIO_PUPD_PULLUP &&
            pull_up_down != GPIO_PUPD_PULLDOWN)
        mgl_mode_setup_default(periph);
    else
        gpio_mode_setup(mgl_get_gpio_port(periph.port), mode,
                        pull_up_down, mgl_get_gpio_pin(periph.pin));
}

//Work with buttons functions-----------------------------------------------------
// bool mgl_is_btn_pressed(mgl_pin btn)
// {
//     uint32_t btn_port = mgl_get_gpio_port(btn.port);
//     uint32_t btn_pin = mgl_get_gpio_pin(btn.pin);
//
//     //debouncing
//     int cnt = 0;
//     for(int i = 0; i < 10; i++)
//         if((gpio_port_read(btn_port) & btn_pin) == !btn.inverse)
//             cnt++;
//
//     if(cnt >= 5){
//         while((gpio_port_read(btn_port) & btn_pin) == !btn.inverse);
//         return true;
//     }
//     else
//         return false;
// }

//Internal functions of mygpiolib-------------------------------------------
uint32_t mgl_get_gpio_pin(uint8_t pin)
{
    // if (pin == 0)     return  GPIO0;
    // if (pin == 1)     return  GPIO1;
    // if (pin == 2)     return  GPIO2;
    // if (pin == 3)     return  GPIO3;
    // if (pin == 4)     return  GPIO4;
    // if (pin == 5)     return  GPIO5;
    // if (pin == 6)     return  GPIO6;
    // if (pin == 7)     return  GPIO7;
    // if (pin == 8)     return  GPIO8;
    // if (pin == 9)     return  GPIO9;
    // if (pin == 10)    return  GPIO10;
    // if (pin == 11)    return  GPIO11;
    // if (pin == 12)    return  GPIO12;
    // if (pin == 13)    return  GPIO13;
    // if (pin == 14)    return  GPIO14;
    // if (pin == 15)    return  GPIO15;
    return 1 << pin;
    //return GPIO0 << pin;
}

uint32_t mgl_get_gpio_port(enum ports port)
{
    // if (port == MGL_PORT_A)    return  GPIOA;
    // if (port == MGL_PORT_B)    return  GPIOB;
    // if (port == MGL_PORT_C)    return  GPIOC;
    // if (port == MGL_PORT_D)    return  GPIOD;
    // if (port == MGL_PORT_E)    return  GPIOE;
    // if (port == MGL_PORT_F)    return  GPIOF;
    // if (port == MGL_PORT_G)    return  GPIOG;
    // if (port == MGL_PORT_H)    return  GPIOH;
    // if (port == MGL_PORT_I)    return  GPIOI;
    // if (port == MGL_PORT_J)    return  GPIOJ;
    // if (port == MGL_PORT_K)    return  GPIOK;

    return GPIOA + (GPIOB - GPIOA) * port;
}
