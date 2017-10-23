start:
	call cls
	ld hl, .title
	call puts
	call line
	ld hl, .as
	call puts
	call menu
	halt
;//in case of interrupts. They aren't yet supported in zenith, but better to plan ahead
.end:
	jr .end

.title: .db "Rise\n",0
.as: .db "Note: your game will autosave. No need to worry about saving :)\n", 0
