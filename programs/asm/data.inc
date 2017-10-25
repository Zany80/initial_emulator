.equ data, 0xF000
{

Data structure:

______________________________
|0| uint8_t|Save file*       |
|1|uint16_t|Current screen ID|
|3| uint8_t|Misc data**      |
|____________________________|

* Bits 0 and 1 specify the save file, bit 2 tells if this is first run
** Bytes marked misc data are used for different purposes by different
parts of the game at different times. Eg. byte 3 is used by two screens
of the intro to store info for back/forth, but is used later - once
those screens are inaccessible - for other purposes.

Note to self: byte 3 needs to be moved into the save file structure once
one is created.

}
