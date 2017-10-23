about:
	ld hl, .strings
	ld bc, 0
	call indexed_print
	ret

.strings:
	.dw a1
a1:
	.db "Sup!",0
