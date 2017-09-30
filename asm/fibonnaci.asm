;~ ;jp start

;Inputs: B: number in the sequence
;Notes: when B is 0, the 256th number in the sequence is returned
;Outputs: C
fibonnaci:
	ld c, 1
	ld a, 0
	inc b
	inc b
_:
	add a, c
	push af
	sub a, c
	ld c, a
	pop af
	djnz -_
	ret

start:

	;~ ld b, 5
	;~ call fibonnaci
	;~ ld a, c
	;~ call int8tonum
	;~ call print_str
	halt

;;convert 8-bit number to decimal string
;;inputs:
;; hl: address at which to store string. WARNING: Up to four bytes will be overwritten, make sure the data isn't important.
;;outputs:
;; a: 0
int8tonum:
	ret

;;Print a string
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
