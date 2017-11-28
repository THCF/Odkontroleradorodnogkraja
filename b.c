/*
 * servo.c
 *
 * Created: 14.11.2017 14:30:24
 *  Author: Microcontroller
 */ 

#include "servo.h"
#include <stdio.h>
#include "dev/usb_serial.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "dev/adc.h"

#define SERVO_MIN 28
#define SERVO_MAX 136
#define SERVO_BASE 84



void servo_init()
{
	cli();
	TCCR1A |= (1 << WGM10);
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << WGM13);
	TCCR1A |= (1 << COM1C1);
	TCCR1B |= (1 << CS12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 1249;
	OCR1C = SERVO_BASE;
	DDRB |= (1 << PB7);
	sei();
}

void servo_move(int pos, int vel)
{
	int new_pos = (pos * 56) / 100 + SERVO_BASE;
	
	if(new_pos > SERVO_MAX)
		new_pos = SERVO_MAX;
	
	if(new_pos < SERVO_MIN)
		new_pos = SERVO_MIN;
		
	if(vel == 0)
	{	
		if(pos == -1)
		{
			OCR1C = SERVO_BASE;
		}
		else
		{
			OCR1C = new_pos;
		}
	
	}
	else if(vel > 0)
	{
		int old_pos = OCR1C;
		if(old_pos > new_pos)
		{
			while(old_pos != new_pos)
			{
				old_pos--;
				OCR1C--;
				int i;
				for(i = 0; i < vel; i++)
					_delay_us(100);
			}
		}
		else if(old_pos < new_pos)
		{
			while(old_pos != new_pos)
			{
				old_pos++;
				OCR1C++;
				int i;
				for(i = 0; i < vel; i++)
					_delay_us(100);
			}
		}
		
	}
	

}