.include "data.inc"

new_game:
	ld hl, data
	ld b, 0
.loop:
	ld (hl), 0
	inc hl
	djnz .loop
	call main_game
	ret
