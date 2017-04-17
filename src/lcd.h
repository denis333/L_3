#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>

/*Define Data Pins,RW,RS and E Pins*/
#define DATA_PORT PORTC
#define PIN_DATA0 0
#define PIN_DATA1 1
#define PIN_DATA2 2
#define PIN_DATA3 3
#define PIN_DATA4 4
#define PIN_DATA5 5
#define PIN_DATA6 6
#define PIN_DATA7 7

#define CONTROL_PORT PORTD
#define PIN_E  3
#define PIN_RW 4
#define PIN_RS 5

void Lcd_Init(void);
void Lcd_putc(unsigned char);
void Lcd_puts(char*);
void Lcd_clear(void);
static void Read_Pins(unsigned char);
#endif /* LCD_H_ */