/*
 * main.c
 *
 *  Created on: 18/mar/2014
 *      Author: lasagni
 */

#include <stdio.h>
#include "dev/usb_serial.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "servo.h"
#include "dev/adc.h"

volatile int sbutton1 = 0;
volatile int sbutton2 = 0;
volatile int sbutton3 = 0;

volatile uint32_t jx = 0;
volatile uint32_t jy = 0;
volatile uint32_t linear = 0;

ISR(TIMER3_COMPA_vect)
{
	if(sbutton1 == 1)
	{
		PORTB ^= (1 << PB6);
	}
	else if(sbutton1 == 0)
	{
		PORTB &= ~(1 << PB6);
	}
	
	if(sbutton2 == 1)
	{
		PORTB ^= (1 << PB5);
	}
	else if(sbutton2 == 0)
	{
		PORTB &= ~(1 << PB5);
	}
	
	if(sbutton3 == 1)
	{
		PORTC ^= (1 << PC6);
	}
	else if(sbutton3 == 0)
	{
		PORTC &= ~(1 << PC6);
	}
}


ISR(TIMER0_OVF_vect)
{
	int oldF = PORTF;

	PORTF &= (0x0f);
	PORTF |= (1 << PF5);
	_delay_us(1);
	linear = adc_get(PF1);
	
	// ADC - Joystick X
	
	PORTF &= (0x0f);
	PORTF |= (1 << PF7);
	PORTF |= (1 << PF6);
	PORTF |= (1 << PF4);
	_delay_us(1);
	jx = adc_get(PF1);
	
	// ADC - Joystick Y
	
	PORTF &= (0x0f);
	PORTF |= (1 << PF5);
	PORTF |= (1 << PF4);
	_delay_us(1);
	jy = adc_get(PF1);
	
	linear = 100 - (linear * 100) / (1023);
	jx = 100 - ( (jx + 1) * 100) / (1023 / 2);
	jy = 100 - ( (jy + 1) * 100) / (1023 / 2);
	
	printf("Linear: %d\n", (int) linear);
	printf("Joystick X: %d\n", (int) jx);
	printf("Joystick Y: %d\n", (int) jy);
	
	PORTF = oldF;
	_delay_us(1);
}

ISR(TIMER1_COMPA_vect)
{
	servo_move(jx,linear);
}

void timer3_init()
{
	cli();
	TCCR3B |= (1 << WGM32);
	TCCR3B |= (1 << CS32);
	TIMSK3 |= (1 << OCIE3A);
	OCR3A = 3124;
	TCNT3 = 0;
	sei();
	
}

void timer0_init()
{
	cli();
	TCCR0B |= (1 << CS00) | (1 << CS02);
	TIMSK0 |= (1 << TOIE0);
	sei();
}


int main(void) {
	usb_init();
	
	DDRF |= (1 << PF7);
	DDRF |= (1 << PF6);
	DDRF |= (1 << PF5);
	DDRF |= (1 << PF4);
	DDRF &= ~(1 << PF1);
	
	
	adc_init(1 << PF1);
	servo_init();
	timer0_init();
	timer3_init();
	
	

	while(1) 
	{
		PORTF = 0;
		_delay_us(1);
		int button1 = PINF & (1 << PF1);
		
		if(button1 == 0)
		{
			if(sbutton1 == 0)
				sbutton1 = 1;
			else
				sbutton1 = 0;
		}
		
		
		PORTF |= (1 << PF7);
		_delay_us(1);
		int button2 = PINF & (1 << PF1);
		
		if(button2 == 0)
		{
			if(sbutton2 == 0)
				sbutton2 = 1;
			else
				sbutton2 = 0;
		}
		
		
		PORTF |= (1 << PF6);
		PORTF &= ~(1 << PF7);
		_delay_us(1);
		int button3 = PINF & (1 << PF1);
		
		if(button3 == 0)
		{
			if(sbutton3 == 0)
				sbutton3 = 1;
			else
				sbutton3 = 0;
		}
		
		PORTF |= (1 << PF7);
		PORTF |= (1 << PF6);
		_delay_us(1);
		int button4 = PINF & (1 << PF1);
		
		if(button4 == 0)
		{
			if(sbutton1 == 0)
				sbutton1 = 1;
			else
				sbutton1 = 0;
			
			if(sbutton2 == 0)
				sbutton2 = 1;
			else
				sbutton2 = 0;
			
			if(sbutton3 == 0)
				sbutton3 = 1;
			else
				sbutton3 = 0;
		}
	}

	return 0;
}



