.include "keyboard.inc"

screens:

.dw intro, awaken, toyourfeet, cell, numbskull1

.shared:
	call poll_character
	cp keyq
	jp z, quit
	cp keym
	jp z, to_menu
	ret

shared_options: .db "'q' - Quit\n'm' - exit to Main menu\n",0
