/**
 * libsk GL-SK on-board LCD abstraction layer
 *
 * The display is WH1602B (based on HD44780 controller)
 */
#ifndef DISPLAY_H
#define DISPLAY_H
#include "errors.h"
#include "mygpiolib.h"
#include <stdint.h>

/** Pointer to delay(uint32_t var) function defined as type */
typedef void (*sk_delay_func_t)(uint32_t);

struct sk_lcd {
	/** HD44780 data pins (DB0..DB7 or DB4..DB7) represented as :c:type:`sk_pin_group` */
	mgl_pin_group *pin_group_data;
	/** HD44780 register select pin (RS) represented as :c:type:`sk_pin` */
	mgl_pin *pin_rs;
	/** HD44780 enable pin (E) represented as :c:type:`sk_pin` */
	mgl_pin *pin_en;
	/** HD44780 enable pin (R/W) represented as :c:type:`sk_pin`.
     *  Set to NULL if not used (always grounded on board) */
	mgl_pin *pin_rw;
	/** Display backlight pin. Set to NULL if not used */
	mgl_pin *pin_bkl;
	/** Pointer to backlight control function (i.e. to set backlight LED PWM level).
      * Set to NULL if not used. This way only two levels will be possible
      * (0 for OFF and != 0 for ON) */
	void (*set_backlight_func)(uint8_t);
	/** Pointer to user-provided delay function with microsecond resolution.
	  * Set to NULL to use ms delay as a fallback */
	sk_delay_func_t delay_func_us;
	/** Pointer to user-provided delay function with millisecond resolution.
      * Set to NULL to use us delay as a fallback */
	sk_delay_func_t delay_func_ms;
	/** True for 4-bit HD44780 interface, False for 8-bit. Only 4-bit IF is supported for now */
	unsigned int is4bitinterface : 1;
};

void lcd_setup_default(void);
void lcd_init_4bit(struct sk_lcd *lcd);
sk_err sk_lcd_set_backlight(struct sk_lcd *lcd, uint8_t level);
#endif
