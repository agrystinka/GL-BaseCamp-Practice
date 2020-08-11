#include "mygpiolib.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stdint.h>
#include <stdbool.h>

bool pin_read(uint32_t gpioport, uint16_t pin)
{
    uint8_t major = 0;
    // assuming buttons are ACTIVE LOW
    for(uint8_t i = 0; i < 10; i++)
	   if (!(gpio_port_read(gpioport) & pin)) //click
		    major++;

    if(major > 6){
        while(!(gpio_port_read(gpioport) & pin));
        return true;
    }
	return false;
}

int main(void)
{
    rcc_periph_clock_enable(RCC_GPIOD);    // leds are here
    rcc_periph_clock_enable(RCC_GPIOC);    // glsk buttons are here
    // gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
    // gpio_mode_setup(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO9 );
    mgl_mode_setup(mgl_led_green, MGL_DEFAULT, MGL_DEFAULT);
    mgl_mode_setup(mgl_led_blue, MGL_DEFAULT, MGL_DEFAULT);
    mgl_mode_setup(mgl_btn_swt3, MGL_DEFAULT, MGL_DEFAULT);

    mgl_set(mgl_led_green);
    mgl_set(mgl_led_blue);

    while (1) {
        if (pin_read(GPIOC, GPIO9))    // button up (SWT3)
            mgl_toggle(mgl_led_green);
    }

}
