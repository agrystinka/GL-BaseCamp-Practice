/*
 * This file containse few alternative samples of work with
 * LEDs, buttons, timers(TIM7, TIM2), systick and interrupts.
 * It is just studing samples, which are not united in one task/project.
 *
 * All samples use mygpiolib - an abstraction over libopencm3/stm32/gpio
*/
#include "mygpiolib.h"
#include "timers.h"
#include "display.h"
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

static void test_bkl_func(uint8_t val)
{
	mgl_set_value(sk_io_lcd_bkl, (bool)val);
}

int main(void)
{
    //SAMPLE WITH DISPLAY
    rcc_periph_clock_enable(RCC_GPIOD);     // leds are here
	rcc_periph_clock_enable(RCC_GPIOE);		// lcd is here

    mgl_mode_setup_default(mgl_led_orange);
    lcd_setup_default();

	mgl_pin_group_set(sk_io_lcd_data, 0x00);
	mgl_set(mgl_led_orange);

	systick_setup();
	cm_enable_interrupts();

    struct sk_lcd lcd = {
    .pin_group_data = &sk_io_lcd_data,
    .pin_rs = &sk_io_lcd_rs,
    .pin_en = &sk_io_lcd_en,
    .pin_rw = &sk_io_lcd_rw,
    .pin_bkl = &sk_io_lcd_bkl,
    //.set_backlight_func = &test_bkl_func,
    .delay_func_us = NULL,
    .delay_func_ms = &delay_ms_systick,
    .is4bitinterface = true
    };

	sk_lcd_set_backlight(&lcd, 0xFF);

    while (1) {
		mgl_clear(mgl_led_orange);
		lcd_init_4bit(&lcd);
		mgl_set(mgl_led_orange);
        delay_ms_systick(500);
    }

    // rcc_periph_clock_enable(RCC_GPIOD);  //LEDs enable
	// rcc_periph_clock_enable(RCC_TIM7);
	// //Switch on red diod untill all settings end
	// mgl_mode_setup_default(mgl_led_red);
	// mgl_set(mgl_led_red);
    //
	// mgl_mode_setup_default(mgl_led_blue);
	// mgl_mode_setup_default(mgl_led_green);
	// mgl_mode_setup_default(mgl_led_orange);
	// gpio_set_output_options(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO12 | GPIO13 | GPIO15);
	// //switch on diods: blue, green, orange
	// mgl_set(mgl_led_blue);
	// mgl_set(mgl_led_green);
	// mgl_set(mgl_led_orange);
    //
	// //sample 3
	// //timer_tim7_setup(1000);
    //
	// //sample 2
	// systick_setup();
    //
	// //sample 1
	// one_shot_timer_tim7_setup();
	// //timer_tim7_setup(1000);
    //
	// cm_enable_interrupts();
    //
	// //Switch aff red diod
	// mgl_clear(mgl_led_red);
    //
    // while (1) {
	// 	//sample 2
	// 	mgl_toggle(mgl_led_green);
	// 	delay_ms_systick(1000);
    //
	// 	//sample 1
	// 	mgl_toggle(mgl_led_orange);
	// 	delay_us_tim7(5000000);
    // }
}
