.include "data.inc"
.include "keyboard.inc"

cell:
	call cls
	ld hl, data
	inc hl \ inc hl \ inc hl
	ld a, 0
	cp (hl)
	ld bc, 1
	ld a, 3
	ld hl, .strings
	call nz, .printloopprep
	call looped_print
.loop:
	call shared@screens
	cp keyb
	jp z, .numbskull
	jr .loop

.numbskull:
	ld hl, data
	inc hl
	inc (hl)
	ret

.printloopprep:
	dec bc
	;//inc a
	ret

.strings: .dw wastedtime@cell, string@cell, shared_options, options@cell
.string: .db "You look around and quickly ascertain your location from "
.db "your surroundings, hastily preparing an escape plan.\n"
.db "\n"
.db "Location: some sort of prison cell.\n"
.db "\n"
.db "Obviously, that is not enough information to figure out an escape "
.db "plan, so you look around a bit more and take stock of your "
.db "resources.\n"
.db "\n"
.db "There is the obvious (and cliched) locked door and barred cell. "
.db "In the back-right corner, there is a - is that a *cookie*?! "
.db "Okaaaaay then. Moving on. Near it, there is a rope, hanging from "
.db "the ceiling. Other than that, there is air, which you can't see, "
.db "but seeing as you are still breathing you are quite certain it is "
.db "there.\n"
.db "\n"
.db "Select your escape plan:\n"
.db "\n"
.db 0

.options: .db "'b' - Bash your head into the wall until your captors "
.db "release you\n"
.db ""
.db 0

.wastedtime: .db "After wasting time standing up and sitting down like "
.db "some sort of weird chicken human mutant thing, you decide to move on.\n",0x0A,0
