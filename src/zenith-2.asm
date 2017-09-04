call print_str

halt

print_str:
	ld a, '3'
	out (0), a
	ret
