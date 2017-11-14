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
#include "dev/adc.h"


int main(void) {
	usb_init();
	
	DDRF |= (1 << PF7);
	DDRF |= (1 << PF6);
	DDRF |= (1 << PF5);
	DDRF |= (1 << PF4);
	DDRF &= ~(1 << PF1);
	
	int sbutton1 = 0;
	int sbutton2 = 0;
	int sbutton3 = 0;
	
	int scale_lin_min = 0;
	int scale_lin_max = 1023;
	
	int scale_x_min = 0;
	int scale_x_max = 1023;
	
	int scale_y_min = 0;
	int scale_y_max = 1023;
	
	adc_init(1 << PF1);
	
	int num_clicks = 0;
	
	printf("Please scale Linear. First Left then Right!\n");
	while(num_clicks < 2) {
		
		PORTF &= (0x0f);
		PORTF |= (1 << PF5);
		_delay_ms(1);
		uint32_t linear = adc_get(PF1);
		
		PORTF = 0;
		_delay_ms(1);
		int button1 = PINF & (1 << PF1);
		
		if(button1 == 0 && num_clicks == 1)
		{
			scale_lin_min = linear;
			printf("Linear Scale min: %d\n", scale_lin_min);
			num_clicks++;
			_delay_ms(1000);
		}
		
		if(button1 == 0 && num_clicks == 0)
		{
			scale_lin_max = linear;
			printf("Linear Scale max: %d\n", scale_lin_max);
			num_clicks++;
			_delay_ms(1000);
		}
	}
	
	num_clicks = 0;
	
	printf("Please scale Joystick X. First Left then Right!\n");
	
	while(num_clicks < 2) {
		
		PORTF &= (0x0f);
		PORTF |= (1 << PF7);
		_delay_ms(1);
		PORTF |= (1 << PF6);
		_delay_ms(1);
		PORTF |= (1 << PF4);
		_delay_ms(1);
		uint32_t jx = adc_get(PF1);
		
		PORTF = 0;
		_delay_ms(1);
		int button1 = PINF & (1 << PF1);
		
		if(button1 == 0 && num_clicks == 1)
		{
			scale_x_min = jx;
			printf("Joystick X Scale min: %d\n", scale_x_min);
			num_clicks++;
			_delay_ms(1000);
		}
		
		if(button1 == 0 && num_clicks == 0)
		{
			scale_x_max = jx;
			printf("Joystick X Scale max: %d\n", scale_x_max);
			num_clicks++;
			_delay_ms(1000);
		}
	}
	
	num_clicks = 0;
	
	printf("Please scale Joystick Y. First Down then Up!\n");
	
	while(num_clicks < 2) {
		
		PORTF &= (0x0f);
		PORTF |= (1 << PF5);
		_delay_ms(1);
		PORTF |= (1 << PF4);
		_delay_ms(1);
		uint32_t jy = adc_get(PF1);
		
		PORTF = 0;
		_delay_ms(1);
		int button1 = PINF & (1 << PF1);
		
		if(button1 == 0 && num_clicks == 1)
		{
			scale_y_min = jy;
			printf("Joystick Y Scale min: %d\n", scale_y_min);
			num_clicks++;
			_delay_ms(1000);
		}
		
		if(button1 == 0 && num_clicks == 0)
		{
			scale_y_max = jy;
			printf("Joystick Y Scale max: %d\n", scale_y_max);
			num_clicks++;
			_delay_ms(1000);
		}
	}
	
	while(1) {
		
		// ADC - Linear
		
		PORTF &= (0x0f);
		PORTF |= (1 << PF5);
		_delay_ms(1);
		uint32_t linear = adc_get(PF1);
		
		// ADC - Joystick X
		
		PORTF &= (0x0f);
		PORTF |= (1 << PF7);
		_delay_ms(1);
		PORTF |= (1 << PF6);
		_delay_ms(1);
		PORTF |= (1 << PF4);
		_delay_ms(1);
		uint32_t jx = adc_get(PF1);
		
		// ADC - Joystick Y
		
		PORTF &= (0x0f);
		PORTF |= (1 << PF5);
		_delay_ms(1);
		PORTF |= (1 << PF4);
		_delay_ms(1);
		uint32_t jy = adc_get(PF1);
		
		PORTF = 0;
		_delay_ms(1);
		int button1 = PINF & (1 << PF1);
		
		if(button1 == 0)
		{
			if(sbutton1 == 0)
				sbutton1 = 1;
			else
				sbutton1 = 0;
		}
		
		
		PORTF |= (1 << PF7);
		_delay_ms(1);
		int button2 = PINF & (1 << PF1);
		
		
		if(button2 == 0)
		{
			if(sbutton2 == 0)
				sbutton2 = 1;
			else
				sbutton2 = 0;
		}
		
		
		PORTF |= (1 << PF6);
		_delay_ms(1);
		PORTF &= ~(1 << PF7);
		_delay_ms(1);
		int button3 = PINF & (1 << PF1);
		
		if(button3 == 0)
		{
			if(sbutton3 == 0)
				sbutton3 = 1;
			else
				sbutton3 = 0;
		}
		
		PORTF |= (1 << PF7);
		_delay_ms(1);
		PORTF |= (1 << PF6);
		_delay_ms(1);
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
		
		_delay_ms(50);
		
		
		linear = 100 - (linear * 100) / (scale_lin_max - scale_lin_min);
		
		printf("Linear: %d\n", (int) linear);
		
		jx = 100 - ( (jx + 1) * 100) / ((scale_x_max - scale_x_min) / 2);

		printf("Joystick X: %d\n", (int) jx);
		
		jy = 100 - ( (jy + 1) * 100) / ((scale_y_max - scale_y_min) / 2);

		printf("Joystick Y: %d\n", (int) jy);
				
	}

	return 0;
}



