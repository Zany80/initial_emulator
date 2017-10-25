.include "data.inc"

check_first_run:
	ld hl, data
	bit 2, (hl)
	call z, .first_run
	ret

.first_run:
	ld hl, .first_yay
	call puts
	ld hl, data
	ld a, (hl)
	or 0x04
	ld (hl), a
	ld b, 0
	ld a, 4
	inc hl
.overwrite:
	ld (hl), 0
	inc hl
	djnz .overwrite
	dec a
	cp 0
	jp nz, .overwrite
	ret

.first_yay: .db "Note: as this is your first time running the game, "
.db "now clearing all save data...\n"
.db "\n"
.db 0
