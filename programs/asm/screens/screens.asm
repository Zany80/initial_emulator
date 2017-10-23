.include "keyboard.inc"

screens:

.dw screen0, screen1, screen2

.shared:
	call poll_character
	cp keyq
	jp z, quit
	cp keym
	jp z, to_menu
	ret

shared_options: .db "'q' - Quit\n'm' - Exit to main menu\n",0
