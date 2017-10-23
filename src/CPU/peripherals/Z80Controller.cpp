#include <main.hpp>

void Z80Controller(uint8_t value){
	switch(value){
		case 0:
			ZENITH::Main::instance->cpu->reset();
			break;
		case 1:
			ZENITH::Main::instance->shutdown();
			break;
		case 2:
			ZENITH::Main::instance->cpu->savePMem();
			break;
	}
}
