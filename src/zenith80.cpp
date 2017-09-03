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
using sf::Mouse;

#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;
using std::streampos;
using std::ios;

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
		uint8_t getOpcode(uint16_t address) override;
		uint8_t getByte(uint16_t address) override;
		void setByte(uint16_t address,uint8_t value) override;
		uint16_t getWord(uint16_t address) override;
		void setWord(uint16_t address,uint16_t value) override;
	private:
		uint8_t * memory;
};

DummyMemory::DummyMemory(){
	//load data from zenith.bin
	ifstream f("zenith.bin", ios::in | ios::binary | ios::ate);
    if (!f.is_open()){
		cout<<"Failed to load data from file, using compiled in program."<<endl;
		cout<<"Note: this usually means you don't have a file zenith.bin in the current working directory (if you don't know what that means, it's probably the folder this program is located within."<<endl;
		//memory=new uint8_t[65536]{
			////output 0x59 to port 0 (0x59 is ASCII 'Y')
			////0x3E,0x59,0xDE,0x00

		//};
			//~ for(int i=0;i<65536;i++)
				//~ memory[i]=0;
			memory=new uint8_t[65536]{
			0x3E,0x48,0xD3,0x00,
			0x3E,0x45,0xD3,0x00,
			0x3E,0x4C,0xD3,0x00,
			0x3E,0x4C,0xD3,0x00,
			0x3E,0x4F,0xD3,0x00,
			0x3E,0x0A,0xD3,0x00,

		};
		//memory=new uint8_t[65536]{
			//0x00
		//};
		return;
	}
	memory=new uint8_t[65536];
	cout<<"Loading program from zenith.bin .."<<endl;
	streampos size=f.tellg();
    f.seekg(0, ios::beg);
    f.read((char*)memory,size);
    for(int i=size;i<65536;i++)
		memory[i]=0x00;
    f.close();
}

uint8_t DummyMemory::getByte(uint16_t address){
	Main::instance->cpu->tstates+=3;
	return memory[address];
}

uint8_t DummyMemory::getOpcode(uint16_t address){
	Main::instance->cpu->tstates+=4;
	return memory[address];
}

uint16_t DummyMemory::getWord(uint16_t address){
	uint16_t lsb=getByte(address);
	uint16_t hsb=getByte((address+1));
	return (hsb<<8)+lsb;
}

void DummyMemory::setByte(uint16_t address,uint8_t value){
	Main::instance->cpu->tstates+=3;
	memory[address]=value;
}

void DummyMemory::setWord(uint16_t address,uint16_t value){
	setByte(address,value&0xFF);
	setByte(address+1,value>>8);
}

class DummyDevice : public Z80Device{
	public:
		void out(uint16_t port,uint8_t value) override;
		uint8_t in(uint16_t port) override;
	private:
		uint16_t amount=0;
		uint8_t ports[65536];
};

void DummyDevice::out(uint16_t port,uint8_t value){
	Main::instance->cpu->tstates+=4;
	ports[port]=value;
	if(port==0){
		cout<<value;
		if((amount++)%1024==0)
			cout.flush();
	}
	else if(port==1)
		cout<<(int)value;
	else
		cout<<(int)value<<" written to port "<<(int)port<<endl;
}

uint8_t DummyDevice::in(uint16_t port){
	Main::instance->cpu->tstates+=4;
	return ports[port];
};

Main::Main(){
	window=new RenderWindow(VideoMode(800,600),"Zenith80");
	window->setVerticalSyncEnabled(true);
	background=Color(3,255,190);
	cpu=new Z80(new DummyMemory(),new DummyDevice());
	Main::instance=this;
}

Main::~Main(){
	delete window;
}

int Main::run(int argc,char ** argv){
	while(window->isOpen()){
		window->clear(this->background);
		window->display();
		cpu->executeXInstructions(1.5*1000*1000/60);
		this->processEvents();
	}
	return 0;
}

void Main::processEvents(){
	Event e;
	while(this->window->pollEvent(e)){
		switch(e.type){
			case Event::Closed:
				window->close();
				cout<<endl<<cpu->getTStates()<<" tstates"<<endl;
				break;
			case Event::MouseButtonPressed:
				if(e.mouseButton.button==Mouse::Button::Left){
					if(cpu->isHalted())
						cpu->reset();
				}
				break;
			default:
			//	cout<<"[Event Manager] Received event of type "<<e.type<<endl;
				break;
		}
	}
}

ZENITH_FOOTER
