#include <SFML/Graphics.hpp>

#include <main.hpp>

using sf::Keyboard;

/**
 * TODO: Document
 */

uint8_t keyboardInput(){
	return ZENITH::Main::instance->key();
}
