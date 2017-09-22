#ifndef _ZENITH80_CPU_PERIPHERALS_KEYBOARD_HPP_
#define _ZENITH80_CPU_PERIPHERALS_KEYBOARD_HPP_

#include <zenith80.hpp>
#include <defines.hpp>
#include <CPU/Z80Device.hpp>

#include <SFML/Graphics.hpp>
using sf::Event;

ZENITH_HEADER

class Keyboard : public Z80Device{
	public:
		Keyboard();
		void out(uint8_t value) override;
		uint8_t in() override;
};

ZENITH_FOOTER

#endif
