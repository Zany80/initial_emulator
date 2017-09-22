#include <SFML/Graphics.hpp>

#include <main.hpp>

using sf::Keyboard;

/**
 * TODO: Document
 */

uint8_t keyboardInput(){
	if(Keyboard::isKeyPressed(Keyboard::Left))
		return 0xDB;
	if(Keyboard::isKeyPressed(Keyboard::Right))
		return 0xDD;
	if(Keyboard::isKeyPressed(Keyboard::Up))
		return 0xD2;
	if(Keyboard::isKeyPressed(Keyboard::Down))
		return 0xDC;
	//letters
	uint8_t letterVal=0;
	if(Keyboard::isKeyPressed(Keyboard::A))
		letterVal=30;
	if(Keyboard::isKeyPressed(Keyboard::B))
		letterVal=35;
	if(Keyboard::isKeyPressed(Keyboard::C))
		letterVal=33;
	if(Keyboard::isKeyPressed(Keyboard::D))
		letterVal=23;
	if(Keyboard::isKeyPressed(Keyboard::E))
		letterVal=13;
	if(Keyboard::isKeyPressed(Keyboard::F))
		letterVal=24;
	if(Keyboard::isKeyPressed(Keyboard::G))
		letterVal=25;
	if(Keyboard::isKeyPressed(Keyboard::H))
		letterVal=26;
	if(Keyboard::isKeyPressed(Keyboard::I))
		letterVal=18;
	if(Keyboard::isKeyPressed(Keyboard::J))
		letterVal=27;
	if(Keyboard::isKeyPressed(Keyboard::K))
		letterVal=28;
	if(Keyboard::isKeyPressed(Keyboard::L))
		letterVal=29;
	if(Keyboard::isKeyPressed(Keyboard::M))
		letterVal=37;
	if(Keyboard::isKeyPressed(Keyboard::N))
		letterVal=36;
	if(Keyboard::isKeyPressed(Keyboard::O))
		letterVal=19;
	if(Keyboard::isKeyPressed(Keyboard::P))
		letterVal=20;
	if(Keyboard::isKeyPressed(Keyboard::Q))
		letterVal=11;
	if(Keyboard::isKeyPressed(Keyboard::R))
		letterVal=14;
	if(Keyboard::isKeyPressed(Keyboard::S))
		letterVal=22;
	if(Keyboard::isKeyPressed(Keyboard::T))
		letterVal=15;
	if(Keyboard::isKeyPressed(Keyboard::U))
		letterVal=17;
	if(Keyboard::isKeyPressed(Keyboard::V))
		letterVal=34;
	if(Keyboard::isKeyPressed(Keyboard::W))
		letterVal=12;
	if(Keyboard::isKeyPressed(Keyboard::X))
		letterVal=32;
	if(Keyboard::isKeyPressed(Keyboard::Y))
		letterVal=16;
	if(Keyboard::isKeyPressed(Keyboard::Z))
		letterVal=31;
	if(letterVal!=0){
		//set first two bits based on case - if lower case group 0, upper case group 1
		letterVal&=0x3F;
		//if (caps lock and not shift) OR (not caps lock and shift), set bit high
		bool caps=Keyboard::isKeyPressed(Keyboard::CAPS);
		bool shift=Keyboard::isKeyPressed(Keyboard::RShift)||Keyboard::isKeyPressed(Keyboard::LShift);
		if((caps&& (!shift)) || (!caps && shift))
			letterVal|=0x40;
		return letterVal;
	}

	return 0x00;
}
