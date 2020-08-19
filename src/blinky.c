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

void lcd_putstring(struct sk_lcd *lcd, const char *str)	// dummy example
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

	//if(custom_cgram_load(&lcd) != SK_EOK){
	//	lcd_print(&lcd, "ґ");
	//}
	//{
		//lcd_print(&lcd, "\tH, 12", 8, 9, 12);
		//lcd_print_int(&lcd, -98, 'd');

		//sk_lcd_cmd_setaddr(&lcd, 0x00, 1);   //cmd
		//_sk_lcd_cmd(&lcd, 0, 0, 0x04);
		//_sk_lcd_cmd(&lcd, 0, 0, 0x00);
		// sk_lcd_cmd_setaddr(&lcd, 0x00, 1);   //cmd
		// //delay_us_tim7(43);
		// uint8_t cuctom_ch1[8] = {0x00, 0x01, 0x1f, 0x10, 0x10, 0x10, 0x10, 0x00};
		// for(int i = 0; i < 8; i++)
		// 	lcd_data_set_byte(&lcd, cuctom_ch1[i]);
		// //if(SK_EOK == custom_cgram_load(&lcd))
		//lcd_print(&lcd, "р");
		custom_char_load(&lcd);
		//lcd_print(&lcd, "ґ");
		//lcd_print(&lcd, "п");
//	}
    while(1) {
    }
}
