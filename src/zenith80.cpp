/*
 * Main.cxx
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "main.hpp"
#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::Color;
using sf::Event;

#include <iostream>
using std::cout;
using std::endl;

int main(int argc,char ** argv){
	return Zenith80::Main::main(argc,argv);
}

ZENITH_HEADER

int Main::main(int argc,char **argv){
	Main z;
	return z.run(argc,argv);
}

class DummyMemory : public Z80Memory{
	public:
		DummyMemory();
		uint8_t getByte(uint16_t address) override;
		void setByte(uint16_t address,uint8_t value) override;
		uint16_t getWord(uint16_t address) override;
		void setWord(uint16_t address,uint16_t value) override;
	private:
		uint8_t * memory;
};

DummyMemory::DummyMemory(){
	memory=new uint8_t[65536]{
		0x3E,0x48,0xD3,0x00,
		0x3E,0x45,0xD3,0x00,
		0x3E,0x4C,0xD3,0x00,
		0x3E,0x4C,0xD3,0x00,
		0x3E,0x4F,0xD3,0x00,
		0x3E,0x0A,0xD3,0x00,

	};
}

uint8_t DummyMemory::getByte(uint16_t address){
	return memory[address];
}

uint16_t DummyMemory::getWord(uint16_t address){
	uint16_t lsb=getByte(address);
	uint16_t hsb=getByte((address+1));
	return (hsb<<8)+lsb;
}

void DummyMemory::setByte(uint16_t address,uint8_t value){
	memory[address]=value;
}

void DummyMemory::setWord(uint16_t address,uint16_t value){
	memory[address]=value&0xFF;
	memory[address+1]=value>>8;
}

class DummyDevice : public Z80Device{
	public:
		void out(uint8_t port,uint8_t value) override;
		uint8_t in(uint8_t port) override;
};

void DummyDevice::out(uint8_t port,uint8_t value){
	if(port==0)
		cout<<value;
	else if(port==1)
		cout<<(int)value;
	else
		cout<<(int)value<<" written to port "<<(int)port<<endl;
}

uint8_t DummyDevice::in(uint8_t port){
	return 0;
};

Main::Main(){
	window=new RenderWindow(VideoMode(800,600),"Zenith-80");
	background=Color(255,255,255);
	cpu=new Z80(new DummyMemory(),new DummyDevice());
}

Main::~Main(){
	delete window;
}

int Main::run(int argc,char ** argv){
	while(window->isOpen()){
		this->processEvents();
		window->clear(this->background);
		window->display();
		cpu->executeXInstructions(100);
	}
	return 0;
}

void Main::processEvents(){
	Event e;
	while(this->window->pollEvent(e)){
		switch(e.type){
			case Event::Closed:
				window->close();
				break;
			default:
			//	cout<<"[Event Manager] Received event of type "<<e.type<<endl;
				break;
		}
	}
}

ZENITH_FOOTER
