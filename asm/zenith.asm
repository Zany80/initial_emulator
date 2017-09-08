.fill 0x100-$

; clear the screen - if you comment this out, running repeatedly (clicking the window repeatedly) will append the text to the screen
ld a, 0
out (3), a
; print the hello_world string
ld hl, hello_world
ld a, 0
call draw_str
; print using BDOS CP/M syscall
ld de, hello_world
ld c, 9
call 5
; halt the CPU
halt

hello_world: .db "Welcome to Zenith80! If you need help, ask on #other-consoles\non the Fantasy Consoles Discord server.\n$",0

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

; draw_str
; inputs:
;	hl: address of string
;	a: end character
draw_str:
	push hl
	call strlen
	pop hl
_:
	ld a, (hl)
	inc l
	jp nz, _
	inc h
_:
	out (3), a
	dec c
	ld a, c
	cp 0
	jp nz, --_
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
