jp start

hello_world:
	.db "Hi there! If you need help, ask on the Fantasy Consoles Discord server.\n"
	.db "The link can be found from http://pleasanta.tk/zenith80\n",0

start:
	ld hl, hello_world
	ld a, 0
	call print_str
	;im 1
	in a, (1)
	add a, 48
	out (0), a
	ld a, 48
	out (0), a
	ld a, 'A'
	ld b, 26
_:
	out (0), a
	inc a
	djnz -_
	halt

print16:
	push af
	ld a, h
	out (2), a
	ld a, l
	out (2), a
	pop af
	ret

;;print_str
;;inputs:
;; hl: string address
;; a: end character
print_str:
	push hl
	call strlen
	pop hl
_:
	ld a, (hl)
	out (0), a
	inc hl
	djnz -_
	ret

;;strlen
;;inputs:
;; hl: string address
;; a: end character
;;outputs:
;; b: string length
strlen:
	push af
	push bc
	ld bc, 256
	cpir
	ld a, 255
	sub c
	pop bc
	ld b, a
	pop af
	ret

test:
	.db hello_world
