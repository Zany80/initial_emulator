ld b, 42
ld a, b
out (1), a
ld h, 0
ld l, 1
ld c, 0
ld a, c
out (1), a
ld c, (hl)
ld a, c
out (1), a
halt
