#include <SFML/Graphics.hpp>

#include <main.hpp>

using sf::Keyboard;

/**
 * TODO: Document
 */

uint8_t keyboardInput(){
	bool caps=Keyboard::isKeyPressed(Keyboard::CAPS);
	bool shift=Keyboard::isKeyPressed(Keyboard::RShift)||Keyboard::isKeyPressed(Keyboard::LShift);
	if(Keyboard::isKeyPressed(Keyboard::Left))
		return 0xDB;
	if(Keyboard::isKeyPressed(Keyboard::Right))
		return 0xDD;
	if(Keyboard::isKeyPressed(Keyboard::Up))
		return 0xD2;
	if(Keyboard::isKeyPressed(Keyboard::Down))
		return 0xDC;
	if(Keyboard::isKeyPressed(Keyboard::Space))
		return 0xD3;
	if(Keyboard::isKeyPressed(Keyboard::Return))
		return 0xD4;
	if(Keyboard::isKeyPressed(Keyboard::BackSpace))
		return 0xA1;
	if(Keyboard::isKeyPressed(Keyboard::Delete))
		return 0xE1;
	//letters
	uint8_t val=0;
	if(Keyboard::isKeyPressed(Keyboard::A))
		val=30;
	if(Keyboard::isKeyPressed(Keyboard::B))
		val=35;
	if(Keyboard::isKeyPressed(Keyboard::C))
		val=33;
	if(Keyboard::isKeyPressed(Keyboard::D))
		val=23;
	if(Keyboard::isKeyPressed(Keyboard::E))
		val=13;
	if(Keyboard::isKeyPressed(Keyboard::F))
		val=24;
	if(Keyboard::isKeyPressed(Keyboard::G))
		val=25;
	if(Keyboard::isKeyPressed(Keyboard::H))
		val=26;
	if(Keyboard::isKeyPressed(Keyboard::I))
		val=18;
	if(Keyboard::isKeyPressed(Keyboard::J))
		val=27;
	if(Keyboard::isKeyPressed(Keyboard::K))
		val=28;
	if(Keyboard::isKeyPressed(Keyboard::L))
		val=29;
	if(Keyboard::isKeyPressed(Keyboard::M))
		val=37;
	if(Keyboard::isKeyPressed(Keyboard::N))
		val=36;
	if(Keyboard::isKeyPressed(Keyboard::O))
		val=19;
	if(Keyboard::isKeyPressed(Keyboard::P))
		val=20;
	if(Keyboard::isKeyPressed(Keyboard::Q))
		val=11;
	if(Keyboard::isKeyPressed(Keyboard::R))
		val=14;
	if(Keyboard::isKeyPressed(Keyboard::S))
		val=22;
	if(Keyboard::isKeyPressed(Keyboard::T))
		val=15;
	if(Keyboard::isKeyPressed(Keyboard::U))
		val=17;
	if(Keyboard::isKeyPressed(Keyboard::V))
		val=34;
	if(Keyboard::isKeyPressed(Keyboard::W))
		val=12;
	if(Keyboard::isKeyPressed(Keyboard::X))
		val=32;
	if(Keyboard::isKeyPressed(Keyboard::Y))
		val=16;
	if(Keyboard::isKeyPressed(Keyboard::Z))
		val=31;
	if(val!=0){
		//if (caps lock and not shift) OR (not caps lock and shift), set bit high
		if((caps&& (!shift)) || (!caps && shift))
			val|=0x40;
		return val;
	}
	if(Keyboard::isKeyPressed(Keyboard::Num1))
		val=1;
	if(Keyboard::isKeyPressed(Keyboard::Num2))
		val=2;
	if(Keyboard::isKeyPressed(Keyboard::Num3))
		val=3;
	if(Keyboard::isKeyPressed(Keyboard::Num4))
		val=4;
	if(Keyboard::isKeyPressed(Keyboard::Num5))
		val=5;
	if(Keyboard::isKeyPressed(Keyboard::Num6))
		val=6;
	if(Keyboard::isKeyPressed(Keyboard::Num7))
		val=7;
	if(Keyboard::isKeyPressed(Keyboard::Num8))
		val=8;
	if(Keyboard::isKeyPressed(Keyboard::Num9))
		val=9;
	if(Keyboard::isKeyPressed(Keyboard::Num0))
		val=10;
	if(Keyboard::isKeyPressed(Keyboard::SemiColon))
		val=30;
	if(Keyboard::isKeyPressed(Keyboard::Slash))
		val=40;
	if(Keyboard::isKeyPressed(Keyboard::Comma))
		val=39;
	if(Keyboard::isKeyPressed(Keyboard::Period))
		val=39;
	if(Keyboard::isKeyPressed(Keyboard::Equal))
		val=0x8B;
	if(Keyboard::isKeyPressed(Keyboard::Dash))
		val=0x8C;
	if(Keyboard::isKeyPressed(Keyboard::LBracket))
		val=0x8D;
	if(Keyboard::isKeyPressed(Keyboard::RBracket))
		val=0x8E;
	if(Keyboard::isKeyPressed(Keyboard::BackSlash))
		val=0x8F;
	if(Keyboard::isKeyPressed(Keyboard::Quote))
		val=0x90;
	if(Keyboard::isKeyPressed(Keyboard::Tilde))
		val=0x91;
	if(val>0){
		if(shift)
			val|=0x40;
		return val;
	}

	return 0x00;
}
