;//poll_character [Input]
;//Polls for input from the keyboard
;//Outputs:
;//	a: character from the keyboard
poll_character:
	in a, (1)
	cp 0
	jp z, poll_character
	ret
