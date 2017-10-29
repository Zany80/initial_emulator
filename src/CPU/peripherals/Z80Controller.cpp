#include <main.hpp>
#include <zenith80.hpp>

ZENITH_HEADER

void Z80Controller(uint8_t value){
	switch(value){
		case 0:
			Main::instance->cpu->reset();
			break;
		case 1:
			Main::instance->shutdown();
			break;
		case 2:
			Main::instance->cpu->savePMem();
			break;
		case 3:
			Main::instance->cpu->swapBanks();
			break;
		case 4:
			////output text to display
			////virt_address of string in hl, x and y position in b and c respectively, color in e, size in d
			uint8_t bank;
			{
				uint16_t virt_address=Main::instance->cpu->getHL().word;
				uint8_t * physical_address=Main::instance->cpu->ram->getBankFromAddress(virt_address)+(virt_address%0x4000);
				word BC=Main::instance->cpu->getBC();
				word DE=Main::instance->cpu->getDE();
				Main::instance->gpu->drawText((char*)physical_address,BC.B.h,BC.B.l,DE.B.h,DE.B.l);
			}
			break;
		case 5:
			////Color to clear with in b
			Main::instance->gpu->clear(Main::instance->cpu->getBC().B.h);
			break;
		case 6:
			////Upload sprite to GPU. HL contains address of sprite, B contains size, C contains index
			word BC=Main::instance->cpu->getBC();
			word HL=Main::instance->cpu->getHL();
			////Main::instance->gpu->uploadSprite(BC.B.l,HL.word,(BC.B.h&240)>>4);
			break;
	}
}

ZENITH_FOOTER
