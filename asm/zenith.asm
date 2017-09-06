ld hl, hello_world
ld a, 0
call strlen
ld a, c
out (1), a
halt

hello_world: .db "Hello, World!",0

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
    push af
    ld a, 0x0A
    out (0), a
    pop af
    ret
space:
    push af
    ld a, ' '
    out (0), a
    pop af
    ret

; strlen
; inputs:
;	hl: string address
;	a: end character
; outputs:
;	c: length
; notes:
;	maximum length: 256
strlen:
	ld bc, 256
	cpir
	push af
	ld a, 255
	sub a, c
	ld c, a
	pop af
	ret
