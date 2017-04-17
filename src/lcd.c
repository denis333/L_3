 #include "lcd.h"
#include <util/delay.h>

void Lcd_Init()
{
/*Configurarea PINILOR D0..D7->OUT,0*/
	DDRC	  = 0xff;
	DATA_PORT = 0x00;
/*Configurarea Pinilor de Comanda RW,RS,E->OUT,0*/
	DDRD         |=  ((1 << PIN_E) | (1 << PIN_RS) | (1<<PIN_RW));
	CONTROL_PORT &= ~((1 << PIN_E) | (1 << PIN_RS) | (1<<PIN_RW));
/*Bus-ul de date pe 8 biti,afisare in 2 rinduri*/
	Read_Pins(0x38);
/*Curatirea ecranului*/	
	 Lcd_clear();
/*Display ON*/	 
	 Read_Pins(0x0c);
/*Incrementarea adresei la fiecare acces, Ecranul nu se deplaseaza.*/
	Read_Pins(0x06);

/*
	CONTROL_PORT |= (1 << PIN_RS);
	CONTROL_PORT &= ~(1 << PIN_RW);
	Read_Pins(0x31);
*/

}

void Lcd_clear(void)
{
	CONTROL_PORT &= ~(1 << PIN_RS);
	CONTROL_PORT &= ~(1 << PIN_RW);
	Read_Pins(0x01);
}

static void Read_Pins(unsigned char byte)
{
		DATA_PORT = byte;
		CONTROL_PORT |=  (1 << PIN_E); _delay_ms(1);
		CONTROL_PORT &= ~(1 << PIN_E); _delay_ms(1);
}

void Lcd_putc(unsigned char character)
{
	CONTROL_PORT |= (1 << PIN_RS);
	CONTROL_PORT &= ~(1 << PIN_RW);
	Read_Pins(character);
}
void Lcd_puts(char *str)
{
	while(*str)
	{
		Lcd_putc(*str++);
	}
}