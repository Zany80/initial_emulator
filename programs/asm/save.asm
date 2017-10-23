save:
	push af
	ld a, 2
	out (2), a
	pop af
	ret
