jp start

hello_world:
	.db "Hi there! If you need help, ask on the Fantasy Consoles Discord server.\n"
	.db "The link can be found from http://pleasanta.tk/zenith80\n",0

lowercase: .db "lowercase a pressed",0

start:
	ld hl, hello_world
	ld a, 0
	call print_str
	im 1
	call poll_input

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
	inc l
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

poll_input:
_:
	in a, (1)
	cp 0
	jp z, -_
	ret
