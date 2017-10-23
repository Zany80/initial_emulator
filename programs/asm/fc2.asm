start:
	call cls
	ld hl, .strings
	ld bc, 0
	ld a, 3
	call looped_print
	call menu
	halt
;//in case of interrupts. They aren't yet supported in zenith, but better to plan ahead
.end:
	jr .end

.strings: .dw title@start,copyright@start,as@start
.title: .db "RISE\n",0
.as: .db "\nNote: your game will autosave. No need to worry about saving :)\n", 0
.copyright: .db "Created by Noam Preil.\nCopyright 2017 Noam Preil\n",0
