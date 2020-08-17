#include "display.h"
#include <stddef.h>

// Clear Display and Return Home commands
static const uint32_t DELAY_CLRRET_US = 1530;
// Read Data from RAM and Write Data to RAM commands
static const uint32_t DELAY_READWRITE_US = 43;
// Read Busy Flag and Address command
static const uint32_t DELAY_BUSYFLAG_US = 0;
// Entry Mode Set, Display ON/OFF Control, Cursor or Display Shift,
// Function Set, Set CGRAM Address, Set DDRAM Address commands
static const uint32_t DELAY_CONTROL_US = 39;
// TODO: ...
static const uint32_t DELAY_ENA_STROBE_US = 1;
// ...
static const uint32_t DELAY_INIT0_US = 4100;
static const uint32_t DELAY_INIT1_US = 100;

// const struct sk_lcd lcd = {
//     .pin_group_data = &sk_io_lcd_data,
//     .pin_rs = &sk_io_lcd_rs,
//     .pin_en = &sk_io_lcd_en,
//     .pin_rw = &sk_io_lcd_rw,
//     .pin_bkl = &sk_io_lcd_bkl,
//     //.set_backlight_func = &test_bkl_func,
//     .delay_func_us = NULL,
//     .delay_func_ms = &sk_tick_delay_ms,
//     .is4bitinterface = true
// };

void lcd_setup_default(void)
{
    gpio_mode_setup(mgl_get_gpio_port(sk_io_lcd_data.port), GPIO_MODE_OUTPUT,
				    GPIO_PUPD_NONE, sk_io_lcd_data.pins);
}

sk_err sk_lcd_set_backlight(struct sk_lcd *lcd, uint8_t level)
{
	if (NULL == lcd)
		return SK_EWRONGARG;

	if (NULL != lcd->set_backlight_func) {
		// try to set with user provided function
		lcd->set_backlight_func(level);
	} else if (NULL != lcd->pin_bkl) {
		// fallback to direct pin control if available
		mgl_set_value(*lcd->pin_bkl, (bool)level);
	} else {
		return SK_EWRONGARG;
	}

	return SK_EOK;
}


/**
  * Private: Provides abstaction over two delay functions passed when constructing sk_lcd object
  *
  * Detect if optimal delay function is applicable and use it when possible with fallback
  * to unoptimal variants
  */
static void lcd_delay_us(struct sk_lcd *lcd, uint32_t us)
{
	if (NULL == lcd)
		return;

	sk_delay_func_t msfunc = lcd->delay_func_ms,
					usfunc = lcd->delay_func_us;

	if ((NULL == msfunc) && (NULL == usfunc))
		return;

	if (NULL == msfunc) {
		// only us-resolution func is set -> use unoptimal us delay
		usfunc(us);
		return;
	}

	if (NULL == usfunc) {
		// only ms-resolution func is set -> use rounded us delay
		msfunc((us % 1000) ? (1 + us / 1000) : (us / 1000));
		return;
	}

	// both functions are set -> use ms delay for divisor and us for remainder
	if (us / 1000)
		msfunc(us / 1000);
	if (us % 1000)
		usfunc(us % 1000);
}

static void lcd_data_set_halfbyte(struct sk_lcd *lcd, uint8_t half)
{
	mgl_set(*lcd->pin_en);
	mgl_pin_group_set(*lcd->pin_group_data, half & 0x0F);
	lcd_delay_us(lcd, DELAY_ENA_STROBE_US);
	mgl_clear(*lcd->pin_en);
	lcd_delay_us(lcd, DELAY_ENA_STROBE_US);
}


static void lcd_data_set_byte(struct sk_lcd *lcd, uint8_t byte)
{
	if (lcd->is4bitinterface) {
		lcd_data_set_halfbyte(lcd, byte >> 4);
		lcd_data_set_halfbyte(lcd, byte & 0x0F);
	} else {
		// 8 bit data interface

		// (!) not implemented yet
	}
}


static void lcd_rsrw_set(struct sk_lcd *lcd, bool rs, bool rw)
{
	mgl_set_value(*lcd->pin_rs, rs);
	mgl_set_value(*lcd->pin_rw, rw);
}


static void lcd_send_byte(struct sk_lcd *lcd, bool rs, uint8_t byte)
{
	lcd_rsrw_set(lcd, rs, true);
	lcd_data_set_byte(lcd, byte);
}


void lcd_init_4bit(struct sk_lcd *lcd)
{
	lcd_rsrw_set(lcd, 0, 0);
	lcd_data_set_halfbyte(lcd, 0b0011);
	lcd_delay_us(lcd, DELAY_INIT0_US);

	lcd_rsrw_set(lcd, 0, 0);
	lcd_data_set_halfbyte(lcd, 0b0010);
	lcd_delay_us(lcd, DELAY_INIT1_US);

	lcd_rsrw_set(lcd, 0, 0);
	lcd_data_set_halfbyte(lcd, 0b0010);
	lcd_delay_us(lcd, DELAY_CONTROL_US);

	// set display on/off: bit2 -- display on (D), bit1 -- cursor on (C), bit 0 -- blink on (B)
	lcd_rsrw_set(lcd, 0, 0);
	lcd_data_set_byte(lcd, 0b00001000 | 0b110);
	lcd_delay_us(lcd, DELAY_CONTROL_US);

	// clear display
	lcd_rsrw_set(lcd, 0, 0);
	lcd_data_set_byte(lcd, 0b00000001);
	lcd_delay_us(lcd, DELAY_CLRRET_US);

	// entry mode set: bit1 -- decrement/increment cnt (I/D), bit0 -- display noshift / shift (SH)
	lcd_rsrw_set(lcd, 0, 0);
	lcd_data_set_byte(lcd, 0b00000100 | 0b10);
	lcd_delay_us(lcd, DELAY_CLRRET_US);
}
