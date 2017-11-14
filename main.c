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

/*used for calibration function*/
uint32_t linearMin = 0;
uint32_t linearMax = 1023;
uint32_t jxMin = 0;
uint32_t jxMax = 1023;
uint32_t jyMin = 0;
uint32_t jyMax = 1023;

 //function print X axis and Y axis
 int32_t scalingX(int32_t xJ)
 {
   
	xJ = ((50 * xJ)/((jxMax - jxMin)/4) - 100);
	printf("Scaled X = %d\n", xJ);
	return xJ;
	
}

//function print X axis and Y axis
 int32_t scalingY(int32_t yJ)
 {
    yJ = ((50 * yJ)/((jyMax - jyMin)/4) - 100);
	printf("Scaled Y =  %d\n", yJ);  
	return yJ;
 }


// function prints potentiometer 
 uint32_t potentiometer_scaling(uint32_t linearScale)
 {
   	linearScale = ((50 * linearScale)/((linearMax - linearMin) / 2));
   	printf("linear is %d\n", linearScale);
   	return linearScale;
 }


//calibration functions

//---------------------------------------------------
void calibrateX()
{
	
	PORTF &= 0x0F;
	PORTF |= (1 << PF7);
	PORTF |= (1 << PF6);
	PORTF |= (1 << PF4);
	printf("Move JX to the left to calibrate.\n");
	fflush(stdout);
	_delay_ms(5000);
	jxMax = adc_get(PF1);
    PORTF &= 0x0F;
	PORTF |= (1 << PF7);
	PORTF |= (1 << PF6);
	PORTF |= (1 << PF4);
	printf("Move JX to the right to calibrate\n");
	fflush(stdout);
	_delay_ms(5000);
	jxMin = adc_get(PF1);

}
//---------------------------------------------------
void calibrateY()
{
	
	PORTF &= 0x0F;
	PORTF |= (1 << PF5);
	PORTF |= (1 << PF4);
	printf("Move JY to the down to calibrate.\n");
	fflush(stdout);
	_delay_ms(5000);
	jyMax = adc_get(PF1);
	PORTF &= 0x0F;
	PORTF |= (1 << PF5);
	PORTF |= (1 << PF4);
	printf("Move JY to the up to calibrate\n");
	fflush(stdout);
	_delay_ms(5000);
	jyMin = adc_get(PF1);

}
//---------------------------------------------------
void calibrateL()
{
	
	PORTF &= 0x0F;
    PORTF |= (1 << PF5);
	printf("Move linear to the left to calibrate.\n");
	fflush(stdout);
	_delay_ms(5000);
	linearMax = adc_get(PF1);

	
	PORTF &= 0x0F;
	PORTF |= (1 << PF5);
	printf("Move linear to the right to calibrate\n");
	fflush(stdout);
	_delay_ms(5000);
	linearMin = adc_get(PF1);

}
//flag used for calibration 
int flag = 0;

int main(void) {
	usb_init();
	
	
	//init input pin
	DDRF &= ~(1 << PF1);
	
	//init output MUX adresses
	DDRF |= (1 << PF4);
	DDRF |= (1 << PF5);
	DDRF |= (1 << PF6);
	DDRF |= (1 << PF7);
	
	//init output for switching on leds
	DDRB |= (1 << PB6);
	DDRB |= (1 << PB5);
	DDRC |= (1 << PC6);
	
	//buttons
	int button1;
	int button2;
	int button3;
	int button4;
    
	//adc_init
	adc_init(1 << PF1);
	
	while(1) {
		
		//button 1
		PORTF &= ~(0xF0);
		_delay_ms(1);
		button1 = PINF & (1 << PF1);
		
		//button 2
		PORTF |= (1 << PF7);
		_delay_ms(1);
		button2 = PINF & (1 << PF1);
		
		//button 3
		PORTF |= (1 << PF6);
		_delay_ms(1);
		PORTF &= ~(1 << PF7);
		_delay_ms(1);
		button3 = PINF & (1 << PF1);
		
		//button 4
		PORTF |= (1 << PF6);
		_delay_ms(1);
		PORTF |= (1 << PF7);
		_delay_ms(1);
		button4 = PINF & (1 << PF1);
		
		
		//adc x
		PORTF &= 0x0F;
		PORTF |= (1 << PF7);
		PORTF |= (1 << PF6);
		PORTF |= (1 << PF4);
		_delay_us(1);
		int32_t jX = adc_get(PF1);
		
		//adc y
		PORTF &= 0x0F;
		PORTF |= (1 << PF5);
		PORTF |= (1 << PF4);
		_delay_us(1);
		int32_t jY = adc_get(PF1);
		
		//adc linear potentiometer
		PORTF &= 0x0F;
		PORTF |= (1 << PF5);
		_delay_us(1);
		uint32_t linear_p = adc_get(PF1);

		if(flag == 0)
		{ 
			_delay_ms(100);
			calibrateX();
			_delay_ms(100);
			calibrateY();
			_delay_ms(100);
			calibrateL();
		}
		flag = 1;

		////////////////////////////////////////////////////
		if(button4 == 0)
		{
			button1 = !button1;
			button2 = !button2;
			button3 = !button3;
			
		}
		if(button1 == 0)
		{
			PORTB |= (1 << PB6);
		}
		if(button2 == 0)
		{
			PORTB |= (1 << PB5);
		}
		if(button3 == 0)
		{
			PORTC |= (1 << PC6);
		}
		
			
	    _delay_ms(50);	
		PORTB &= ~(1 << PB6);
		PORTB &= ~(1 << PB5);
		PORTC &= ~(1 << PC6);
		_delay_ms(50);	
		
		///////////////////////////////////////////////////
		//printf("Raw value of x %d\n",jX);
		//printf("Raw value of y %d\n",jY);
         
        jX = scalingX(jX); 
        jY = scalingY(jY);
        linear_p =  potentiometer_scaling(linear_p);
 		
    }
	
	

	return 0;
}



