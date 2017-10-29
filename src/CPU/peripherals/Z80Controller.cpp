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
				if(virt_address<0x4000)
					bank=0;
				else if(virt_address<0x8000)
					bank=1;
				else if(virt_address<0xC000)
					bank=2;
				else
					bank=3;
				uint8_t * physical_address=Main::instance->cpu->ram->getBank(bank)+(virt_address%0x4000);
				word BC=Main::instance->cpu->getBC();
				word DE=Main::instance->cpu->getDE();
				Main::instance->drawText((char*)physical_address,BC.B.h,BC.B.l,DE.B.h,DE.B.l);
			}
			break;
		case 5:
			////Color to clear with in b
			Main::instance->clear(Main::instance->cpu->getBC().B.h);
			break;
		case 6:
			Main::instance->display();
			break;
	}
}

ZENITH_FOOTER
