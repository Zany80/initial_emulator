.include "keyboard.inc"
.include "data.inc"

screen0:
	ld hl, .strings
	ld bc, 0
	call indexed_print
	inc bc
	call indexed_print
.loop:
	call poll_character
	cp keyb
	jp z, .begin
	cp keyq
	jp z, .quit
	cp keys
	call z, save
	jr .loop

.begin:
	ld hl, data
	inc hl
	ld (hl), 1
	ret

.quit:
	ld a, 1
	out (2), a
	;//In case it doesn't work, halt
.qloop:
	halt
	jr .qloop

.strings: .dw mg1, mg2
mg1:
.db "The year is 1394. For years, the dark prince Algar has ruled the "
.db "country of Vlarzel with an iron fist. Now, people are starting to "
.db "fight back.\n"
.db "\n"
.db "Your job as the player is to maintain a network of informants, "
.db "plan out battles, and overthrow him.\n"
.db "\n"
.db "Unlike in most games, you don't control one character, or even a "
.db "group of characters. You alternate between many different people "
.db "with the job of coordinating them.\n"
.db "\n"
.db "Good luck."
.db 0x0A,0x0A,0

mg2:
.db "Press 'b' to begin.\n"
.db "Press 's' to save.\n"
.db "Press 'q' to quit."
.db 0
