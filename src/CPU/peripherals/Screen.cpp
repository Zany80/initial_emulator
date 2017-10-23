#include <main.hpp>

void screenOutput(uint8_t value){
	ZENITH::Main::instance->putchar((char)value);
}

void integerOutput(uint8_t value){
	ZENITH::Main::instance->putint((int)value);
}
