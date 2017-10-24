.include "keyboard.inc"
.include "data.inc"

screen2:
	call cls
	ld hl, data
	inc hl \ inc hl \ inc hl
	ld a, 0
	cp (hl)
	ld hl, .strings
	ld bc, 5
	ld a, 3
	call z, .ploopadj
	call looped_print
.loop:
	call shared@screens
	cp keyl
	jp z, .lay
	cp keyo
	jp z, .look
	jr .loop

.ploopadj:
	ld bc, 0
	ld a, 8
	ret
	
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
	ld hl, data
	inc hl
	inc (hl)
	ret

.nomore:
	call line
	ld hl, .nothappening
	call puts
	jr .loop

.strings:
.dw string2@screen2, dark_prince@shared_strings, string3@screen2, dark_country@shared_strings,string4@screen2
.dw string1@screen2, shared_options, options@screen2

.string1: .db "You rise to your feet, leaning against a nearby wall.\n"
.db "\n"
.db 0

.string2:
.db "As you get up, memories rush back in to your skull. Your name is "
.db "Adaer Lerauk, and you are a ",'"',"covert operative",'"',". You "
.db "work for RISE (RISE! Imperialism Shall End!), a group dedicated to "
.db "overthrowing the dark prince "
.db 0

.string3: .db " and creating peace in ",0

.string4:
.db ".\n"
.db "\n"
.db "Your current mission is to steal a copy of the battle plans "
.db 0

.options: .db "'l' - Lay back down\n'o' - lOok around",0
.nothappening: .db "Yeah, no. I don't have time to write up an amusing "
.db "scenario for all however many times you want to do that.\n"
.db "Sorry, not sorry.\n"
.db 0
