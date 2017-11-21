#include <avr/io.h>

.global  joystick_init_asm

.section .text

joystick_init_asm:

push  r16
push  r17
push  r18

; configure MUX IOs
; set PF7, PF6, PF5 and PF4 as outputs
lds r16,DDRF
ori r16,((1<<PF7) | (1<<PF6) | (1<<PF5) | (1<<PF4))
sts DDRF,r16

; set PF1 as input
lds r17,DDRF
andi r17,~(1<<PF1)
sts DDRF,r17

cpi r24,1
breq yaxis

xaxis:
lds r16,PORTF
andi r16,~((1<<PF7) | (1<<PF6) | (1<<PF5) | (1<<PF4))
sts PORTF,r16

lds r16,PORTF
ori r16,((1<<PF4) | (1<<PF6) | (1<<PF7))
sts PORTF,r16


cpi r24,0
breq jumpp

yaxis:
lds r16,PORTF
andi r16,~((1<<PF7) | (1<<PF6) | (1<<PF5) | (1<<PF4))
sts PORTF,r16

lds r16,PORTF
ori r16,((1<<PF4) | (1<<PF5))
sts PORTF,r16

jumpp:

pop  r18
pop  r17
pop  r16
ret