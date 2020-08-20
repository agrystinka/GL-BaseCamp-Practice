#include "display.h"
#include <stdint.h>

//
// bool isemoji = false;
// bool ischar  = false;
void lcd_print_int(struct sk_lcd *lcd, int32_t num, char format)
{
	//hex
	if(format == 'h'){

	}
	//binary
	else if(format == 'b'){

	}
	//any other - decimal
	else{
		if (num < 0){
			sk_lcd_putchar(lcd, '-');
			num = 0 - num;
		}
		char str[16];
		int i = 0;
		while(num > 0){
			str[i] = num % 10 + '0';
			num /= 10;
			i++;
		}
		str[i] = '\0';

		i--;
		while(i >= 0){
			sk_lcd_putchar(lcd, str[i]);
			i--;
		}
	}
}

// void lcd_print_float(struct sk_lcd *lcd, float num)
// {
//
// }

void lcd_print_n(struct sk_lcd *lcd)
{
	static uint8_t cnt = 0;
	if(cnt == 0){
		sk_lcd_cmd_setaddr(&lcd, 0x40, false);
	}
	else{
		//make delay for 1 s and clear display
		delay_ms_systick(1000);
		sk_lcd_cmd_clear(&lcd);
		//TODO: make one line up shift
	}
	cnt = 1 - cnt;
}
//
// void lcd_print_emoji(struct sk_lcd *lcd, uint8_t c)
// {
// 	//lcd_custom_emoji_load(lcd);
// 	uint8_t address = 0;
// 	switch (c) {
// 		case HURT:   address =  0x07; //change
// 		case CHARGE: address =  0x01;
// 		case BELL:   address =  0x02;
// 		case LOCK:   address =  0x03;
// 		case KEY:    address =  0x04;
// 		default:     address =  0xFF;	// black square for unknown symbols
// 	}
// 	sk_lcd_write_byte(lcd, address);
// 	//lcd_custom_char_load(lcd);
// }

void lcd_print(struct sk_lcd *lcd, const char *format, ...)
{
	// static bool isload = false;
	// if(!isload){
	// 	lcd_custom_char_load(lcd);
	// 	isload = true;
	// }
	int cnt = 0;
	char *ptr = format;
	while (*ptr != '\0') {
		if(*ptr == '\n'){
			lcd_print_n(&lcd);
	   	}
		else if(*ptr == '\t'){
			sk_lcd_putchar(lcd, ' ');
			sk_lcd_putchar(lcd, ' ');
	   	}
		else if(*ptr == '\r'){
			sk_lcd_cmd_rethome(lcd);
	   	}
		else{
			sk_lcd_putchar(lcd, *ptr);
		}
		ptr++;
		cnt++;
		if(cnt >= 16){
			cnt = 0;
			lcd_print_n(&lcd);
		}
	}
}
