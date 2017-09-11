jp start

start:
	; tests:
	;ld r, n
	;out (n), a
	;ld r, r'
	ld a, 12
	ld b, 0
	out (4), a
	ld b, 12
	ld a, 0
	ld a, b
	out (4), a
	; tests:
	;ld dd, nn
	;ld r, (hl)
	ld a, 91
	out (4), a
	ld a, 1
	ld hl, t1
	ld a, (hl)
	out (4), a
	; tests:
	;ld a, (bc)
	ld a, 12
	out (4), a
	ld bc, t2
	ld a, (bc)
	out (4), a
	; tests: ld hl, (nn)
	ld a, 109
	out (4), a
	ld hl, (t4)
	ld a, l
	out (4), a
	ld a, 0
	out (4), a
	ld a, h
	out (4), a
	; tests ld (nn), hl
	ld hl, t3
	ld a, 109
	out (4), a
	ld (t5), hl
	ld hl, (t5)
	ld a, l
	out (4), a
	halt

t1: .db 91
t2: .db 12
t3: .db t4
t4: .db 109
t5: .db 0x0000
