quit:
	ld a, 1
	out (2), a
	;//In case it doesn't work, halt
.qloop:
	halt
	jr .qloop
