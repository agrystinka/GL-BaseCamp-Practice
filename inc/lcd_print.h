/**
 * libsk GL-SK on-board LCD abstraction layer
 *
 * The display is WH1602B (based on HD44780 controller)
 */
#ifndef LCD_PRINT_H
#define LCD_PRINT_H
#include "errors.h"
#include "display.h"
#include "mygpiolib.h"

//void lcd_print_int(struct sk_lcd *lcd, int32_t num);
void lcd_print(struct sk_lcd *lcd, const char *format, ...);

#endif
