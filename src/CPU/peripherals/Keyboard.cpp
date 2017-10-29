#include <SFML/Graphics.hpp>

#include <main.hpp>
#include <CPU/peripherals/Keyboard.hpp>

using sf::Keyboard;

ZENITH_HEADER

/**
 * TODO: Document
 */

uint8_t keyboardInput(){
	return Main::instance->key();
}

uint8_t keyCode(sf::Event::KeyEvent k){
	uint8_t v=0;
	bool caps=Keyboard::isKeyPressed(Keyboard::CAPS);
	//letters
	switch(k.code){
		case Keyboard::Q:
			v=0x0B;break;
		case Keyboard::W:
			v=0x0C;break;
		case Keyboard::E:
			v=0x0D;break;
		case Keyboard::R:
			v=0x0E;break;
		case Keyboard::T:
			v=0x0F;break;
		case Keyboard::Y:
			v=0x10;break;
		case Keyboard::U:
			v=0x11;break;
		case Keyboard::I:
			v=0x12;break;
		case Keyboard::O:
			v=0x13;break;
		case Keyboard::P:
			v=0x14;break;
		case Keyboard::A:
			v=0x15;break;
		case Keyboard::S:
			v=0x16;break;
		case Keyboard::D:
			v=0x17;break;
		case Keyboard::F:
			v=0x18;break;
		case Keyboard::G:
			v=0x19;break;
		case Keyboard::H:
			v=0x1A;break;
		case Keyboard::J:
			v=0x1B;break;
		case Keyboard::K:
			v=0x1C;break;
		case Keyboard::L:
			v=0x1D;break;
		case Keyboard::Z:
			v=0x1F;break;
		case Keyboard::X:
			v=0x20;break;
		case Keyboard::C:
			v=0x21;break;
		case Keyboard::V:
			v=0x22;break;
		case Keyboard::B:
			v=0x23;break;
		case Keyboard::N:
			v=0x24;break;
		case Keyboard::M:
			v=0x25;break;
		default:
			break;
	}
	if(v!=0){
		//if uppercase, set group to 1, otherwise leave at 0
		if((caps&&!k.shift)||(k.shift&&!caps))
			v|=0x40;
		return v;
	}
	//numbers and symbols
	//Everything in this switch/case simply changes group (a bitwise-OR operation) when shift is pressed
	switch(k.code){
		case Keyboard::Num1:
			v=0x01;break;
		case Keyboard::Num2:
			v=0x02;break;
		case Keyboard::Num3:
			v=0x03;break;
		case Keyboard::Num4:
			v=0x04;break;
		case Keyboard::Num5:
			v=0x05;break;
		case Keyboard::Num6:
			v=0x06;break;
		case Keyboard::Num7:
			v=0x07;break;
		case Keyboard::Num8:
			v=0x08;break;
		case Keyboard::Num9:
			v=0x09;break;
		case Keyboard::Num0:
			v=0x0A;break;
		case Keyboard::Equal:
			v=0x8B;break;
		case Keyboard::Dash:
			v=0x8C;break;
		case Keyboard::LBracket:
			v=0x8D;break;
		case Keyboard::RBracket:
			v=0x8E;break;
		case Keyboard::BackSlash:
			v=0x8F;break;
		case Keyboard::Quote:
			v=0x90;break;
		case Keyboard::Tilde:
			v=0x91;break;
		case Keyboard::Comma:
			v=0x26;break;
		case Keyboard::Period:
			v=0x27;break;
		case Keyboard::Slash:
			v=0x28;break;
		case Keyboard::SemiColon:
			v=0x1E;break;
		default:
			break;
	}
	if(v!=0){
		//if shift is pressed, set group to 1 (symbol), otherwise leave at 0 (number)
		if(k.shift)
			v|=0x40;
		return v;
	}
	switch(k.code){
		case Keyboard::Escape:
			return 0xDE;
		case Keyboard::Return:
			return 0xD4;
		case Keyboard::Space:
			return 0xD3;
		case Keyboard::Up:
			return 0xD2;
		case Keyboard::Left:
			return 0xDB;
		case Keyboard::Down:
			return 0xDC;
		case Keyboard::Right:
			return 0xDD;
		case Keyboard::BackSpace:
			return 0xA3;
		case Keyboard::Delete:
			return 0xE3;
		case Keyboard::Pause:
			return 0x92;
		case Keyboard::Insert:
			return 0x93;
		default:
			break;
	}
	return 0;
}

ZENITH_FOOTER
