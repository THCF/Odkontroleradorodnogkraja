#include <avr/io.h>

.global  adc_get_asm

.section .text

adc_get_asm:

; save  registers to  stack
push  r16
push  r17
push  r18

; only use pins 0 to 7
ldi r16,0b00000111
;ldi r17,1
and r16,r24
ori r16,(1<<REFS0)
sts ADMUX,r16

; set the bit ADSC to start a conversion
lds r16,ADCSRA
ori r16,(1<<ADSC)
sts ADCSRA,r16

; wait until the ADSC bit in the ADCRA register is cleared up again:
; that means the conversion is done
wloop:
lds r16,ADCSRA
sbrc r16,ADSC
rjmp wloop

lds	r24, ADCL
lds	r25, ADCH

; restore  registers  from  stack
pop  r18
pop  r17
pop  r16

ret