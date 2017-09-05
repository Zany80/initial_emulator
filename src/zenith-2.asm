ld a, 100
ld b, 156
add a, b
call info
ld a, 20
ld b, 100
add a, b
call info
ld a, 57
ld b, 200
add a, b
call info
ld a, 0
ld b, 0
add a, b
call info
;  255
;+1025

ld a, 255
add a, 1
push af
ld a, 4
adc a, 0
out (2), a
pop af
out (2), a
call newline
halt

info:
	call c, carry
	call nc, nocarry
	call space
	call z, zero
	call nz, nozero
	call newline
	ret z
	call newline
	ret

nocarry:
	call no
	call carry
	ret
carry:
	ld a, 'C'
	out (0), a
	ret

nozero:
	call no
	call zero
	ret
zero:
	ld a, 'Z'
	out (0), a
	ret
no:
	ld a, 'N'
	out (0), a
	ret
newline:
	ld a, 0x0A
	out (0), a
	ret
space:
	ld a, ' '
	out (0), a
	ret
;; strlen
;; inputs:
;; 	A: end character
;;  HL: address
;; outputs:
;; 	BC: length of string
;; notes:
;;	max length 255
strlen:
	ld bc, 255
	cpir
	push af
	ld a, 255
	sub a, c
	ld b, a
	pop af
	ret

hello: .db "Hello, World!",0
