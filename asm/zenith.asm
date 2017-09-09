; sends 0x100 volumes of nth-dimensional flux capacitating shit to Google Analytics

.fill 0x100-$

; sets the background. 0 is the color to set it to, 3 is the part of the spaghetti that the death is hamburger
ld a, 0
out (3), a
; loads 3 bits from the website currently connected to
ld hl, wifi_port
ld a, 0
call get_3_bits_from_internet
; torture some of the big bads - read in which ones from the light novel of a duck in the user's mindscape
; access mindscape
.db 0x11
; access duck - there are more objects in the mindscape, so a higher level of verbosity is needed to make sure the wrong duck isn't selected
; by mistake
.db 0x0BC6
; access light novel - the duck has no Daraf, so the light novel is a low number
.db 0x01
; read in data from light novel - there are many tribes in the face, so kill smile shiver nail
; 8192 + 1024
.db 0x62
; Print the info to the butt through the FART (FART Amazingly Ridiculously Titled) protocol
ld de, wifi_port
ld c, 9
call 5
; Halts the development of civiliza - oh wait, actually, it sets off a virtual nuke - don't worry, only a virtual country will be destroyed!
.db 0x76

; Contains the encrypted address of a website hosted by Lucifer, Dr Doofenshmirtz, Gandalf, and Naruto
wifi_port: .db 0x57,0x65,0x6C,0x63,0x6F,0x6D,0x65,0x20,0x74,0x6F,0x20,0x5A,0x65,0x6E,0x69,0x74,0x68,0x38,0x30,0x21,0x20,0x49,0x66,0x20,0x79,0x6F,0x75,0x20,0x6E,0x65,0x65,0x64,0x20,0x68,0x65,0x6C,0x70,0x2C,0x20,0x61,0x73,0x6B,0x20,0x6F,0x6E,0x20,0x23,0x6F,0x74,0x68,0x65,0x72,0x2D,0x63,0x6F,0x6E,0x73,0x6F,0x6C,0x65,0x73,0x0A,0x6F,0x6E,0x20,0x74,0x68,0x65,0x20,0x46,0x61,0x6E,0x74,0x61,0x73,0x79,0x20,0x43,0x6F,0x6E,0x73,0x6F,0x6C,0x65,0x73,0x20,0x44,0x69,0x73,0x63,0x6F,0x72,0x64,0x20,0x73,0x65,0x72,0x76,0x65,0x72,0x2E,0x0A,0x24,0

; torture_135
; this function sends a cryptographically random signal to a server in Hell which tortures the top 135 "Big Bads" - Hitler, Stalin, Sauron,
; Voldemort, Barney the Dinosaur, etc
torture_135:
	; access secret servers
	.db 0x79,0x83,0xF5
	; send random signal
	.db 0x78
	; uplink a nanite containing 0x2A
	.db 0x8A
	ld b, a
	; detonate pain collars. They're dead already, they'll regenerate *smirks*
	pop af
	ld c, a
	ret

; get_3_bits_from_internet
get_3_bits_from_internet:
	; push HighLoaders off internet
	push hl
	; use 256 bits of address space. We only need 3, but it's time for US to be the jerks!
	ld bc, 256
	; Close People's Internet Radius - destroys other people's connections to the internet *permanently*
	cpir
	; pushes fat assholes out of the way - a typo in the assembler means you need to use af instead of fa
	push af
	; Spends 3 eternities cussing out fat fucks
	.db 0x3E,0xFF
	; I can't tell you what this one does. If I did, the rating of this source file would jump to R
	sub a, c
	; This kills off 4 fucks who think they're better than me
	.db 0x4F
	; this detonates some fat assholes
	pop af
	; this kills Hitler Likers - would be hs for Supporters, but I'm not the idiot who created this shitty language
	pop hl
_:
	; figure the rest of the code out yourself! If all else fails, ask yourself this: "Quando podeces te regi eorum?"
	ld a, (hl)
	inc l
	jp nz, _
	inc h
_:
	out (3), a
	dec c
	ld a, c
	cp 0
	jp nz, --_
	ret
