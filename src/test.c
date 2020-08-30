//simple sample to test buttons and leds

#include "mygpiolib.h"
//#include "timers.h"
//#include "display.h"
#include "errors.h"
//#include "lcd_hd44780.h"
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
//
// struct sk_lcd lcd = {
// 	.pin_group_data = &sk_io_lcd_data,
// 	.pin_rs = &sk_io_lcd_rs,
// 	.pin_en = &sk_io_lcd_en,
// 	.pin_rw = &sk_io_lcd_rw,
// 	//.pin_bkl = &sk_io_lcd_bkl,
// 	.set_backlight_func = &timer1_set_pwm_backlight,
// 	.delay_func_us = NULL,
// 	.delay_func_ms = &delay_ms_systick,
// 	.is4bitinterface = true,
// 	.charmap_func = &sk_lcd_charmap_rus_cp1251
// // };
//
// void lcd_putstring(struct sk_lcd *lcd, const char *str)	// dummy example
// {
// 	char *ptr = str;
// 	while (*ptr != '\0') {
// 		sk_lcd_putchar(lcd, *ptr);
// 		ptr++;
// 	}
// }

int main(void)
{
  rcc_periph_clock_enable(RCC_GPIOD);    // leds are here
  rcc_periph_clock_enable(RCC_GPIOC);    // glsk buttons are here
//	rcc_periph_clock_enable(RCC_GPIOE);	   // lcd is connected to port E

	//display_setup();
//	lcd_setup_default();
	mgl_mode_setup_default(mgl_led_orange);
	mgl_mode_setup_default(mgl_led_green);
  mgl_mode_setup_default(mgl_led_red);
	mgl_mode_setup_default(mgl_led_blue);

  mgl_mode_setup_default(mgl_btn_swt1);
	mgl_mode_setup_default(mgl_btn_swt2);
  mgl_mode_setup_default(mgl_btn_swt3);
	mgl_mode_setup_default(mgl_btn_swt4);
  mgl_mode_setup_default(mgl_btn_swt5);

  mgl_mode_setup_default(mgl_btn_usr);

	mgl_set(mgl_led_orange);
  mgl_set(mgl_led_blue);
  mgl_set(mgl_led_red);
  mgl_set(mgl_led_green);

  while(1) {
      //mgl_clear(mgl_led_orange);
      if(mgl_is_btn_pressed(mgl_btn_swt1))
        mgl_toggle(mgl_led_orange);
      // if(mgl_is_btn_pressed(mgl_btn_swt3))
      //   mgl_toggle(mgl_led_red);
      // if(mgl_is_btn_pressed(mgl_btn_swt4))
      //   mgl_toggle(mgl_led_green);
      // if(mgl_is_btn_pressed(mgl_btn_swt5))
      //   mgl_toggle(mgl_led_blue);
      //
      // if(mgl_is_btn_pressed(mgl_btn_swt2)){
      //     mgl_toggle(mgl_led_green);
      //     mgl_toggle(mgl_led_blue);
      // }
      // if(mgl_is_btn_pressed(mgl_btn_usr)){
      //     mgl_toggle(mgl_led_red);
      //     mgl_toggle(mgl_led_orange);
      // }
  }
}
