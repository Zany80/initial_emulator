#ifndef _ZENITH80_CPU_PERIPHERALS_KEYBOARD_HPP_
#define _ZENITH80_CPU_PERIPHERALS_KEYBOARD_HPP_

#include <zenith80.hpp>
#include <defines.hpp>
#include <CPU/Z80Device.hpp>

#include <SFML/Graphics.hpp>
using sf::Event;

ZENITH_HEADER

uint8_t keyCode(Event::KeyEvent k);

ZENITH_FOOTER

#endif
