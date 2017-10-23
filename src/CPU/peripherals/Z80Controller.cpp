#include <main.hpp>

void Z80Controller(uint8_t value){
	switch(value){
		case 0:
			ZENITH::Main::instance->cpu->reset();
		case 1:
			ZENITH::Main::instance->shutdown();
	}
}
