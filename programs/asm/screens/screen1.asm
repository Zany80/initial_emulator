.include "keyboard.inc"
.include "data.inc"

screen1:
	call cls
	ld hl, data
	ld bc, 3
	add hl, bc
	ld a, 0
	cp (hl)
	jp nz, .repeated
	ld hl, .strings
	ld bc, 0
	call indexed_print
	call line
	inc bc
	call indexed_print
	inc bc
	call indexed_print
.loop:
	call shared@screens
	cp keys
	jp z, .advance
	jr .loop
	
.advance:
	ld hl, data
	inc hl
	ld (hl), 2
	ret

.repeated:
	ld b, 0
	ld c, (hl)
	inc c \ inc c
	ld hl,.strings
	add hl, bc
	add hl, bc
	ld e, (hl)
	inc hl
	ld d, (hl)
	push de \ pop hl
	ld a, (hl)
	inc hl
	ld bc, 0
.repeatedprintloop:
	push af
	call indexed_print
	pop af
	inc bc
	dec a
	cp 0
	jp nz, .repeatedprintloop
	call line \ call line
	ld hl, .strings
	ld bc, 1
	call indexed_print
	inc bc
	call indexed_print
.rloop:
	call shared@screens
	cp keys
	jp z, .advance
	jr .rloop

.strings: .dw string1@screen1, shared_options,options@screen1,stringsr1@screen1,stringsr2@screen1,stringsr3@screen1
.string1: .db "You wake up with a pounding headache. ",'"',"Where - where am I?",'"'," you mutter, slowly becoming more alert.\n",0
.options: .db "'s' - Stand up",0
.layagain: .db "You lay back down, grasping your head in pain.",0
.nootherreason: .db " There's obviously no other reason for you to sit back down, so that must be it.",0
.reallyman: .db "...really? Really? Was this *really* necessary? Stop fooling around!",0
.stringsr1: .db 1 \ .dw layagain@screen1
.stringsr2: .db 2 \ .dw layagain@screen1,nootherreason@screen1
.stringsr3: .db 1 \ .dw reallyman@screen1
