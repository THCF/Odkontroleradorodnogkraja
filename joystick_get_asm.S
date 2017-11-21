
#include <avr/io.h>

.global  joystick_get_asm

.section .text

joystick_get_asm:

push  r16
push  r17
push  r18

mov r22,r24
mov r23,r25
ldi r24,0
ldi r25,0

ldi r18,5
ldi r19,0
ldi r20,0
ldi r21,0

mov r24,r22
mov r25,r23

ldi r20,0
ldi r21,2

sub   r24, r20
sbc   r25, r21

pop  r18
pop  r17
pop  r16
ret
