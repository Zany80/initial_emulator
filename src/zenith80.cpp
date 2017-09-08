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
using std::cerr;
using std::endl;

#include <fstream>
using std::ifstream;
using std::streampos;
using std::ios;

#include <string>
using std::string;

#include <config.h>

#include <fonts/Famirids.h>

int main(int argc,char ** argv){
	return Zenith80::Main::main(argc,argv);
}

ZENITH_HEADER

int Main::main(int argc,char **argv){
	Main z(argc,argv);
	return z.run();
}

class DummyMemory : public Z80Memory{
	public:
		DummyMemory(const char * name);
		uint8_t getOpcode(uint16_t address) override;
		uint8_t getByte(uint16_t address) override;
		void setByte(uint16_t address,uint8_t value) override;
		uint16_t getWord(uint16_t address) override;
		void setWord(uint16_t address,uint16_t value) override;
	private:
		uint8_t * memory;
		uint8_t * emu;
};

DummyMemory::DummyMemory(const char * name){
	//load data from zenith.bin
	ifstream f(name, ios::in | ios::binary | ios::ate);
	if(Main::instance->cpm_emu){
		emu=new uint8_t[0x100]{
			0xC3,0x00,0x01,0x00,0x00,0x00,0xC9
		};
	}
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
	for(int i=0;i<65536;i++)
		memory[i]=0;
	cout<<"Loading program from "<<name<<endl;
	streampos size=f.tellg();
    f.seekg(0x00);
    char* mem=(char*)memory;
    if(Main::instance->cpm_emu){
		mem+=0x100;
		f.seekg(0x100);
	}
    f.read(mem,size);
    f.close();
}

uint8_t DummyMemory::getByte(uint16_t address){
	Main::instance->cpu->tstates+=3;
	if(Main::instance->cpm_emu&&address<0x100)
		return emu[address];
	return memory[address];
}

uint8_t DummyMemory::getOpcode(uint16_t address){
	Main::instance->cpu->tstates+=4;
	if(Main::instance->cpm_emu&&address==5){
		uint8_t C=Main::instance->cpu->getBC().B.l;
		word DE=Main::instance->cpu->getDE();
		switch(C){
			case 0:
				cout<<"BDOS system reset"<<endl;
				Main::instance->cpu->reset();
				break;
			case 2:
				cout<<(char)DE.B.l;
				break;
			case 9:
				while(memory[DE.word]!='$')
					cout<<(char)memory[DE.word++];
				cout.flush();
				break;
			default:
				cout<<"BDOS call: "<<(int)C<<endl;
				break;
		}
	}
	if(Main::instance->cpm_emu&&address<0x100)
		return emu[address];
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
		uint8_t tvalue=0;
		bool t=false;
};

void DummyDevice::out(uint16_t port,uint8_t value){
	Main::instance->cpu->tstates+=4;
	ports[port]=value;
	switch(port){
		case 0:
			cout<<value;
			if((amount++)%1024==0)
				cout.flush();
			break;
		case 1:
			cout<<(int)value;
			break;
		case 2:
			if(t)
				cout<<(int)((int)(tvalue<<8)+value)<<endl;
			else
				tvalue=value;
			t=!t;
			break;
		case 3:
			Main::instance->put_char((char)value);
			break;
		default:
			cout<<(int)value<<" written to port "<<(int)port<<endl;
			break;
	}
}

uint8_t DummyDevice::in(uint16_t port){
	Main::instance->cpu->tstates+=4;
	return ports[port];
};

Main::Main(int argc,char ** argv){
	Main::instance=this;
	window=new RenderWindow(VideoMode(800,600),"Zenith80");
	window->setVerticalSyncEnabled(true);
	background=Color(3,225,197);
	string name="zenith.bin";
	bool name_changed=false;
	cpm_emu=false;
	for(int i=1;i<argc;i++){
		string arg=argv[i];
		if(arg=="--file"){
			cout<<"--file recognized, ";
			if(name_changed)
				cout<<"discarding previous argument \""<<name.c_str()<<"\" and ";
			if(i+1==argc){
				cerr<<"but no file specified! Ignoring option."<<endl;
				break;
			}
			name=argv[++i];
			cout<<"loading \""<<name<<"\"..."<<endl;
			name_changed=true;
		}
		else if(arg=="--cpm"){
			cpm_emu=true;
		}
		else{
			cerr<<"Unrecognized option \""<<arg<<"\", ignoring..."<<endl;
		}
	}
	if(!name_changed)
		cout<<"No --file argument received, defaulting to \"zenith.bin\"..."<<endl;
	cpu=new Z80(new DummyMemory(name.c_str()),new DummyDevice());
	default_font=new Font;
	default_font->loadFromMemory(&Famirids_ttf,Famirids_ttf_len);
	git_revision=new Text(string("Git revision: ")+STRINGIFY(GIT_REVISION),*default_font,23);
	git_revision->setFillColor(Color(255,0,0));
	git_revision->setPosition(0,0);
	terminal=new Text("",*default_font,18);
	terminal->setFillColor(Color(39,91,201));
	terminal->setPosition(0,25);
	terminal_string="";
}

Main::~Main(){
	delete window;
	delete cpu;
	delete git_revision;
	delete default_font;
}

int Main::run(){
	while(window->isOpen()){
		window->clear(this->background);
		window->draw(*(this->git_revision));
		window->draw(*(this->terminal));
		window->display();
		cpu->executeXInstructions(4.1*1000*1000/60);
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

void Main::put_char(char c){
	terminal_string+=c;
	if(c==' ')
		terminal_string+=' ';
	if(c==0)
		terminal_string="";
	terminal->setString(terminal_string);
}

ZENITH_FOOTER
