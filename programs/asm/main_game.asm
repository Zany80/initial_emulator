.include "data.inc"

main_game:
	ld hl, main_game
	push hl
	;//load in position from `data` structure
	ld hl, data
	inc hl
	ld e,(hl)
	inc hl
	ld d,(hl)
	ld hl, screens
	add hl, de
	add hl, de
	ld e, (hl)
	inc hl
	ld d, (hl)
	push de
	ret
