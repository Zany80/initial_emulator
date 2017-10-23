.include "keyboard.inc"

menu:
	call line
	ld hl, .strings
	ld bc, 0
	call indexed_print
.poll:
	call poll_character
	ld de, .end
	push de
	cp keyn
	jp z, new_game
	cp keyl
	jp z, load_game
	cp keya
	jp z, about
	cp keys
	jp z, settings
	jp .nokey
.end:
	ret

.nokey:
	ld hl, .strings
	ld bc, 1
	call indexed_print
	jr .poll

.strings:
.dw _1, _2

_1: .db "Select option: \n",9,"'n' - New Game\n", 0
_2: .db "Invalid key pressed!\n",0
