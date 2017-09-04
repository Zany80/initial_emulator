jp main_code

print_str:
	ld a, '3'
	out (0), a
	ret

main_code:
	call print_str
	halt
