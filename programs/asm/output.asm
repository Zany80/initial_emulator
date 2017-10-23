;//puts [Display]
;//Prints a string to the screen
;//Inputs:
;//	hl: address of string to print
puts:
	ld a,(hl)
	cp 0
	ret z
	out (0), a
	inc hl
	jr puts

line:
	ld a, 0x0A
	out (0),a
	ret

cls:
	ld a, 0
	out (0), a
	ret

;//indexed_print [Display]
;//Prints a string based on array address and index
;//Inputs:
;//	hl: array address
;//	bc: index
indexed_print:
	push hl
	add hl, bc
	add hl, bc
	ld e, (hl)
	inc hl
	ld d, (hl)
	push de \ pop hl
	call puts
	pop hl
	ret
