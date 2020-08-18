#include "mygpiolib.h"
#include "timers.h"
#include "display.h"
#include "errors.h"
#include "lcd_hd44780.h"
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

struct sk_lcd lcd = {
	.pin_group_data = &sk_io_lcd_data,
	.pin_rs = &sk_io_lcd_rs,
	.pin_en = &sk_io_lcd_en,
	.pin_rw = &sk_io_lcd_rw,
	//.pin_bkl = &sk_io_lcd_bkl,
	.set_backlight_func = &timer1_set_pwm_backlight,
	.delay_func_us = NULL,
	.delay_func_ms = &delay_ms_systick,
	.is4bitinterface = true,
	.charmap_func = &sk_lcd_charmap_rus_cp1251
};

static void lcd_putstring(struct sk_lcd *lcd, const char *str)	// dummy example
{
	char *ptr = str;
	while (*ptr != '\0') {
		sk_lcd_putchar(lcd, *ptr);
		ptr++;
	}
}

int main(void)
{
    rcc_periph_clock_enable(RCC_GPIOD);
	rcc_periph_clock_enable(RCC_GPIOE);		// lcd is connected to port E

	//display_setup();
	lcd_setup_default();
	mgl_mode_setup_default(mgl_led_orange);
	mgl_mode_setup_default(mgl_led_green);
	mgl_set(mgl_led_orange);
	mgl_pin_group_set(sk_io_lcd_data, 0x00);

	systick_setup();
	cm_enable_interrupts();

	init_bkl_pwm();

	mgl_clear(mgl_led_orange);
	//sk_pin_set(sk_io_led_green, true);

	sk_lcd_init(&lcd);
	sk_lcd_cmd_onoffctl(&lcd, true, false, false);	// display on, cursor off, blinking off
	sk_lcd_set_backlight(&lcd, 200);

	//lcd_putstring(&lcd, "Здравствуй, мир!");
	// TODO: implement printf-like interface with special chars (\r \n \t ...)
	//sk_lcd_cmd_setaddr(&lcd, 0x40, false);	// 2nd line begins from addr 0x40
	//lcd_putstring(&lcd, "Hello, world!");
	//lcd_print(&lcd, "\tHello, 123", 8, 9, 12);
	//lcd_print_int(&lcd, -987, 'd');
	if(SK_EOK == custom_cgram_load(&lcd))
		lcd_putstring(&lcd, "ґґґґ");
	else
		lcd_putstring(&lcd, "Error");
	//sk_lcd_cmd_setaddr(&lcd, 0x40, false);

	//sk_lcd_putchar(&lcd, 'ґ');
	//sk_lcd_putchar(&lcd, 'a');
//	sk_lcd_putchar(&lcd, 'A');

	//bool isdirright = true;
     while(1) {
    }
}
