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
using std::to_string;
using std::string;

#include <config.h>
#include <defines.hpp>
#include "icon.c"

#include <fonts/Famirids.h>

#include <CPU/peripherals/RAMController.hpp>

int main(int argc,char ** argv){
	return Zenith80::Main::main(argc,argv);
}

ZENITH_HEADER

Main* Main::instance;

int Main::main(int argc,char **argv){
	Main z(argc,argv);
	return z.run();
}

class DummyDevice : public Z80Device{
	public:
		void out(uint16_t port,uint8_t value) override;
		uint8_t in(uint16_t port) override;
	private:
		uint16_t amount=0;
		uint8_t ports[65536];
		uint8_t tvalue=0;
		struct{
			uint8_t num;
			bool active;
		}test;
		bool t=false;
};

void DummyDevice::out(uint16_t port,uint8_t value){
	Main::instance->cpu->tstates+=4;
	ports[port]=value;
	switch(port){
		case 0:
			Main::instance->putchar((char)value);
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
		case 4:
			if(!test.active)
				test.num=value;
			else if(test.num!=value){
				Main::instance->cpu->halt();
				Main::instance->putmsg("Assertion failed! CPU halted!");
				string s="Expected value: "+to_string((int)test.num);
				s=s+", received: "+to_string((int)value);
				Main::instance->putmsg(s);
				Main::instance->putmsg("");
			}
			else{
				cout<<"Assertion passed! Program execution continuing normally."<<endl;
				cout<<"Expected value: "<<(int)test.num<<", received: "<<(int)value<<endl;
			}
			test.active=!test.active;
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
	clock_speed=4;
	unit=MHz;
	window=new RenderWindow(VideoMode(800,600),"Zenith80");
	window->setIcon(icon.width,icon.height,icon.pixel_data);
	window->setVerticalSyncEnabled(true);
	gui=new Gui(*window);
	termOut=TermOut::create();
	termOut->setPosition(0,0);
	termOut->setSize(800,570);
	termOut->setLinesStartFromTop(true);
	termOut->setLineLimit(1000);
	gui->add(termOut);
	termOut->addLine(((string)"Git revision: ")+STRINGIFY(GIT_REVISION));
	termOut->addLine("Loading...");
	background=Color(3,225,197);
	string name="zenith.bin";
	bool name_changed=false;
	cpm_emu=false;
	for(int i=1;i<argc;i++){
		string arg=argv[i];
		if(arg=="--file"){
			if(i+1==argc){
				cerr<<"No file specified! Ignoring option \"--file\"."<<endl;
				termOut->addLine("No file specified! Ignoring option \"--file\".");
				break;
			}
			if(name_changed){
				termOut->addLine(((string)"Discarding previous --file argument \"")+name.c_str()+"\".");
				cerr<<"Discarding previous --file argument \""<<name.c_str()<<"\".";
			}
			name=argv[++i];
			termOut->addLine((string)"--file specified, loading \""+name+"\"...");
			cout<<"--file specified, loading \""<<name<<"\"..."<<endl;
			name_changed=true;
		}
		else if(arg=="--clock-speed"){
			if(i+1==argc){
				cerr<<"--clock-speed requires an argument!"<<endl;
				termOut->addLine("--clock-speed requires an argument!");
				break;
			}
			clock_speed=std::stoll(argv[++i]);
		}
		else if(arg=="--cpm"){
			cpm_emu=true;
			cout<<"CP/M BDOS syscall emulation activated. Loading program into address 0x100."<<endl;
			cout<<"Make sure that you have \".fill 0x100-$\" at the top of the .asm file"<<endl;
			termOut->addLine("CP/M BDOS syscall emulation activated. Loading program into address 0x100.");
			termOut->addLine("Make sure that you have \".fill 0x100-$\" at the top of the .asm file");
		}
		else{
			cerr<<"Unrecognized option \""<<arg<<"\", ignoring..."<<endl;
			termOut->addLine(((string)"Unrecognized option \"")+arg+"\", ignoring...");
		}
	}
	if(!name_changed){
		cout<<"No --file argument received, defaulting to \"zenith.bin\"..."<<endl;
		termOut->addLine("No --file argument received, defaulting to \"zenith.bin\"...");
	}
	cpu=new Z80(new RAMController(name.c_str()),new DummyDevice());
	default_font=new Font;
	default_font->loadFromMemory(&Famirids_ttf,Famirids_ttf_len);
	termOut->addLine("");
	termOut->addLine("Starting program...");
	termOut->addLine("");
	termOut->addLine("");
}

Main::~Main(){
	delete window;
	delete cpu;
	delete default_font;
}

int Main::run(){
	while(window->isOpen()){
		window->clear(this->background);
		gui->draw();
		window->display();
		cpu->executeXInstructions(this->clock_speed*this->unit/60);
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
		gui->handleEvent(e);
	}
}

void Main::putchar(char ch){
	if(ch!='\n'){
		string c=termOut->getLine(termOut->getLineAmount()-1)+ch;
		termOut->removeLine(termOut->getLineAmount()-1);
		termOut->addLine(c);
	}
	else
		termOut->addLine("");
}

void Main::putmsg(string s){
	termOut->addLine(s);
}

ZENITH_FOOTER
