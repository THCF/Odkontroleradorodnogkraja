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
#include "dev/adc.h"

#define SERVO_MIN 7
#define SERVO_MAX 34
#define SERVO_BASE 21



void servo_init()
{
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	TCCR0A |= (1 << COM0B1);
	TCCR0B |= (1 << CS00) | (1 << CS02);

	OCR0B = SERVO_BASE;
	DDRD |= (1 << PD0);
}

void servo_move(int pos, int vel)
{	
	int new_pos = (pos * 14) / 100 + SERVO_BASE;
	
	if(new_pos > SERVO_MAX)
		new_pos = SERVO_MAX;
	
	if(new_pos < SERVO_MIN)
		new_pos = SERVO_MIN;
	
	if(vel == 0)
	{
		if(pos == -1)
		{
			OCR0B = SERVO_BASE;
		}
		else
		{
			OCR0B = new_pos;
		}
	}
	else if(vel > 0)
	{
		int old_pos = OCR0B;
		if(old_pos > new_pos)
		{
			while(old_pos != new_pos)
			{
				old_pos--;
				OCR0B--;
				int i;
				for(i = 0; i < vel; i++)
					_delay_ms(1);
			}
		}
		else if(old_pos < new_pos)
		{
			while(old_pos != new_pos)
			{
				old_pos++;
				OCR0B++;
				int i;
				for(i = 0; i < vel; i++)
					_delay_ms(1);
			}
		}
		
	}
	

}