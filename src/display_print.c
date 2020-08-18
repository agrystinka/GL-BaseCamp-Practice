#include "display.h"
#include <stdint.h>

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

void lcd_print_n(struct sk_lcd *lcd)
{
	static cnt = 0;
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

void lcd_print(struct sk_lcd *lcd, const char *format, ...)
{
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
