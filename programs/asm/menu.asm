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
.dw options@menu, invalid@menu

.options: .db "Select option: \n","\t'n' - New Game\n","\t'l' - Load Game\n","\t'a' - About page\n","\t's' - Settings screen\n", 0
.invalid: .db "Invalid key pressed!\n",0

to_menu:
	ld a, 0
	out (2), a
