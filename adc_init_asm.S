#include <avr/io.h>

#define _IO(x) (x-0x20)

.global  adc_init_asm

.section .text

adc_init_asm:

push  r16
push  r17

; 1. Enable the ADC by setting the bit ADEN in the register ADCSRA,
; and configure the prescaler with a  division factor of 128
; selecting ADPS2:0 as mentioned in the Atmega2560 datasheet.
ldi r16,(1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2)
sts ADCSRA,r16

; choose pins 0 to 7 and free running mode
ldi r17,(0 << MUX5) | (0 << ADTS0) | (0 << ADTS1) | (0 << ADTS2)
sts ADCSRB,r17

; AVcc with external capacitor on AREF pin
ldi r16,(1 << REFS0)
sts ADMUX,r16

; set PF1 as input
in r17,_IO(DDRF)
andi r17,~(1<<PF1)
out _IO(DDRF),r17

; disable pull-up
in r17,_IO(PORTF)
andi r17,~(1<<PF1)
out _IO(PORTF),r17

clr r16
loop:
mov r24,r16
call adc_get_asm
inc r16
cpi r16, 7
brne loop

pop  r17
pop  r16

ret