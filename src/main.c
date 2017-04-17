#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "lm20.h"

void SystemInit(void);

unsigned char the_low_ADC; 
int ten_bit_value;
char str[10];



ISR(ADC_vect)
{
/*1.Citirea ADCL,are loc blocarea 
reg ADC pentru scriere*/
	the_low_ADC = ADCL;
/*2.Citirea ADCH,are loc deblocarea
reg. ADC,pastrarea datelor(ADCH,ADCL)*/	
	ten_bit_value = (ADCH << 2) | (the_low_ADC >> 6);
	ten_bit_value=To_Volts(ten_bit_value);
	
//get Temperature
	ten_bit_value=To_Temp(ten_bit_value,1);
	
	itoa(ten_bit_value,str,10);
	ADCSRA |= (1 << ADSC);
}

int main(void)
{
	SystemInit();
	Lcd_Init();
	sei();
    while (1) 
    {	
		if(TCNT0 > 240)
		{
			Lcd_clear();
			Lcd_puts(str);
			TCNT0 = 0;
		}
		
    }
}

void SystemInit()
{
/*---------TIMER0 INITIALIZATION----------*/	
	TCCR0 = (1 << CS02) | (1 << CS00); //clk/1024
	TCNT0=0;
/*----------ADC INITIALIZATION------------*/	
	//ADC PORT INIT
	DDRA=0x00;
	PORTA=0xff;

/*Alegerea canalului*/
	ADMUX = (0 << MUX0) | (0 << MUX1) | (0 << MUX2);
/*Select Ualim*/
	ADMUX |= (1 << REFS0) | (1 << REFS1);
/*Pozitionarea rezultatului*/
	ADMUX |= (1 << ADLAR);
/*Configurarea registrului ADCSRA*/
	ADCSRA |= (1 << ADIE) | (1 << ADIF) | (1 << ADATE) | (1 << ADEN) | (1 << ADPS2) | (1 << ADSC);
	SFIOR = 0x00;
}
