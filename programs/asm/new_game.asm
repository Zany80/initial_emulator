.include "data.inc"

new_game:
	ld hl, data
	ld b, 3
.loop:
	ld (hl), 0
	inc hl
	djnz .loop
	call cls
	call main_game
	ret
