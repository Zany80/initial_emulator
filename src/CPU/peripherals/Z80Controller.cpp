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
	}
}

ZENITH_FOOTER
