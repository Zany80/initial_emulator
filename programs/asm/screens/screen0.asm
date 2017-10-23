.include "keyboard.inc"
.include "data.inc"

screen0:
	call cls
	ld hl, .strings
	ld bc, 0
	ld a, 7
	call looped_print
.loop:
	call shared@screens
	cp keyb
	jp z, .begin
	jr .loop

.begin:
	ld hl, data
	inc hl
	ld (hl), 1
	ret

.strings: .dw string1@screen0, dark_prince@shared_strings, string2@screen0
.dw dark_country@shared_strings, string3@screen0, shared_options, options@screen0
.string1:
.db "The year is 1394. For years, the dark prince ",0

.string2:
.db " has ruled the country of ",0

.string3:
.db " with an iron fist. Now, people are starting to "
.db "fight back.\n"
.db "\n"
.db "Your job as the player is to maintain a network of informants, "
.db "plan out battles, and overthrow him.\n"
.db "\n"
.db "Unlike in most games, you don't control one character, or even a "
.db "group of characters. You alternate between many different people "
.db "with the job of coordinating them.\n"
.db "\n"
.db "However, there *are* still traditional RPG elements - exploring, "
.db "fighting, character classes, levelling up - but the characters "
.db "are premade, and each one has their own ",'"',"feel",'"',".\n"
.db "\n"
.db "Good luck.\n"
.db 0x0A,0

.options:
.db "'b' - Begin\n"
.db 0
