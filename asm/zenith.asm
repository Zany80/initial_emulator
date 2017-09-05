ld bc, 419
ld de, 8131
call add16
ld hl, hello
ld a, 0
call print_str
halt

hello: .db "Hello, World!",0

; add16
; inputs:
;	bc and de: the two numbers to add together
; outputs:
;	bc: the result
add16:
	ld a, c
	add a, e
	push af
	ld a, b
	adc a, d
	ld b, a
	pop af
	ld c, a
	ret

; print16
; inputs:
;	bc: the 16-bit number to print
print16:
	push af
	ld a, b
	out (2), a
	ld a, c
	out (2), a
	pop af
	ret

; print_str
; inputs:
;	hl: address of string
;	a: end character
print_str:
	call strlen
	ret

newline:
	ld a, 0x0A
	out (0), a
	ret
space:
	ld a, ' '
	out (0), a
	ret

; strlen
; inputs:
;	hl: string address
;	a: end character
; outputs:
;	c: length
; notes:
;	maximum length: 255
strlen:
	ld bc, 255
	cpir
	push af
	ld a, 255
	sub a, c
	ld c, a
	pop af
	ret
