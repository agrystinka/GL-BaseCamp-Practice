#ifndef DISPLAY_H
#define DISPLAY_H
#include "lcd_hd44780.h"
#include <stdint.h>

extern struct sk_lcd lcd;

enum emoji{
    HURT = 0,
    CHARGE = 1,
    BELL = 2,
    LOCK = 3,
    KEY = 4
};

void display_setup(void);
/*Print on display*/
void lcd_print_int(struct sk_lcd *lcd, int32_t num, char format);
//void lcd_print_emoji(struct sk_lcd *lcd, uint8_t c);
void lcd_print(struct sk_lcd *lcd, const char *format, ...);
void lcd_print_n(struct sk_lcd *lcd);

/*Display PWM*/
void timer1_set_pwm_backlight(uint8_t val);
void init_bkl_pwm(void);
void exti0_isr(void);

#endif
