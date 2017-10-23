.include "keyboard.inc"
.include "data.inc"

screen2:
	call cls
	ld hl, .strings
	ld bc, 0
	call indexed_print
	inc bc
	call indexed_print
	inc bc
	call indexed_print
.loop:
	call shared@screens
	cp keyl
	jp z, .lay
	cp keyo
	jp z, .look
	jr .loop
	
.lay:
	ld hl, data
	ld b, 3
.loopinc:
	inc hl
	djnz .loopinc
	ld a, 3
	cp (hl)
	jp z, .nomore
	inc (hl)
	dec hl \ dec hl
	dec (hl)
	ret
.look:

	ret

.nomore:
	call line
	ld hl, .nothappening
	call puts
	jr .loop

.strings: .dw string1@screen2, shared_options, options@screen2

.string1: .db "You rise to your feet, leaning against a nearby wall.\n"
.db 0x0A,0

.options: .db "'l' - Lay back down\n'o' - lOok around",0
.nothappening: .db "Yeah, no. I don't have time to code up an amusing "
.db "scenario for all however many times you want to do that.\n"
.db "Sorry, not sorry.\n"
.db 0
