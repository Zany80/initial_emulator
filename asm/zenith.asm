jp start

hello_world:
	.db "Hi there! If you need help, ask on the Fantasy Consoles Discord server.\n"
	.db "The link can be found from http://pleasanta.tk/zenith80",0

start:
	ld hl, hello_world
	ld a, 0
	call print_str
	halt

;;print_str
;;inputs:
;; hl: string address
;; a: end character
print_str:
	push hl
	call strlen
	pop hl
	ret

;;strlen
;;inputs:
;; hl: string address
;; a: end character
;;outputs:
;; b: string length
strlen:
