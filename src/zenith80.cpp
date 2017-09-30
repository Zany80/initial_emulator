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
using sf::Keyboard;
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

#include <assets/icon.hpp>
#include <assets/fonts/Famirids.hpp>

#include <CPU/peripherals/RAMController.hpp>
#include <CPU/peripherals/DeviceController.hpp>
#include <CPU/peripherals/Keyboard.hpp>

int main(int argc,char ** argv){
	return Zenith80::Main::main(argc,argv);
}

ZENITH_HEADER

Main* Main::instance;

int Main::main(int argc,char **argv){
	Main z(argc,argv);
	return z.run();
}

Main::Main(int argc,char ** argv){
	Main::instance=this;
	_key=0;
	clock_speed=4;
	unit=MHz;
	window=new RenderWindow(VideoMode(800,600),"Zenith80");
	window->setIcon(80,80,icon);
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
	cpu=new Z80(new RAMController(name.c_str()),new DeviceController());
	default_font=new Font;
	default_font->loadFromMemory(&Famirids_ttf,Famirids_ttf_len);
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
		this->processEvents();
		window->clear(this->background);
		gui->draw();
		window->display();
		cpu->executeXInstructions(this->clock_speed*this->unit/60);
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
			case Event::KeyPressed:
				uint8_t key=keyCode(e.key);
				if(key){
					//limit buffer size to 32
					if(key_buffer.size()<32){
						key_buffer.push_back(key);
					}
				}
				break;
			//default:
			//	cout<<"[Event Manager] Received event of type "<<e.type<<endl;
				//break;
		}
		gui->handleEvent(e);
	}
}

void Main::putchar(char ch){
	if(ch==0){
		return termOut->removeAllLines();
	}
	string msg="";
	if(ch!='\n'){
		msg=termOut->getLine(termOut->getLineAmount()-1)+ch;
		termOut->removeLine(termOut->getLineAmount()-1);
	}
	putmsg(msg);
}

void Main::putmsg(string s){
	termOut->addLine(s);
}

uint8_t Main::key(){
	if(key_buffer.empty()){
		//if there isn't anything in the key buffer, just return 0
		return 0;
	}
	//fetch the value at the front,
	uint8_t v=key_buffer.front();
	//then remove it from the buffer
	key_buffer.erase(key_buffer.begin());
	//return the value
	return v;
}

void Main::resetKeyBuffer(){
	key_buffer.clear();
}

ZENITH_FOOTER
