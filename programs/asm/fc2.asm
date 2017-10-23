.org 0x0000

title: .db "Rise\n",0

start:
	call cls
	ld hl, title
	call puts
	call menu
	halt
;//in case of interrupts. They aren't yet supported in zenith, but better to plan ahead
.end:
	jr .end
