#include "lcd_print.h"
#include <stdint.h>

// void lcd_print_int(struct sk_lcd *lcd, int32_t num)
// {
// 	if (num < 0)
// 		sk_lcd_putchar(lcd, 'm');
// 	char str[16];
// 	int i = 0;
// 	while(num != 0){
// 		str[i] = num % 10 + '0';
// 		num /= 10;
// 		i++;
// 	}
// 	str[i] = '\0';
//
// 	i--;
// 	while(i >= 0){
// 		sk_lcd_putchar(lcd, str[i]);
// 		i--;
// 	}
//
// 	//lcd_putstring(&lcd, str);
// }

static void lcd_print_sn(struct sk_lcd *lcd)
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
	//int cnt = 0;
	char *ptr = format;
	while (*ptr != '\0') {
		if(*ptr == '\n'){
			lcd_print_sn(&lcd);
	   	}
		else if(*ptr == '\t'){
			sk_lcd_putchar(lcd, ' ');
			sk_lcd_putchar(lcd, ' ');
	   	}
		else if(*ptr != '%'){
			sk_lcd_putchar(lcd, *ptr);
		}
		// else {
		// 	if(*(ptr + 1) == 'c'){
		// 		sk_lcd_putchar(lcd, 'A');
		// 	}
		// 	if(*(ptr + 1) == 'd'){
		// 		sk_lcd_putchar(lcd, 'B');
		// 	}
		// 	if(*(ptr + 1) == 'f'){
		// 		sk_lcd_putchar(lcd, 'C');
		// 	}
		// 	if(*(ptr + 1) == 's'){
		// 		sk_lcd_putchar(lcd, 'C');
		// 	}
		// 	else{
		// 		sk_lcd_putchar(lcd, '%');
		// 		sk_lcd_putchar(lcd, *(ptr + 1));
		// 	}
		// 	ptr++;
		// 	//cnt++;
		// }
		ptr++;
	}
}
