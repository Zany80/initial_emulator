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
#include <assets/fonts/Serif.hpp>

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
	format=binary;
	Main::instance=this;
	clock_speed=4;
	unit=MHz;
	window=new RenderWindow(VideoMode(256,128),"Zenith80");
	window->setIcon(80,80,icon);
	gui=new Gui(*window);
	termOut=TermOut::create();
	termOut->setPosition(0,0);
	termOut->setSize("100%","100%");
	termOut->getRenderer()->setBackgroundColor(sf::Color(0,0,0,0));
	termOut->setTextColor(sf::Color(255,255,255,255));
	termOut->setLinesStartFromTop(true);
	termOut->setLineLimit(1000);
	canvas=Canvas::create();
	canvas->setPosition(0,0);
	canvas->setSize("100%","100%");
	gui->add(canvas);
	//gui->add(termOut);
	background=Color(3,225,197);
	name="zenith.bin";
	bool name_changed=false;
	cpm_emu=phys_cart=false;
	default_font=new Font;
	default_font->loadFromMemory(&Serif_ttf,Serif_ttf_len);
	waiting=false;
	for(int i=1;i<argc;i++){
		string arg=argv[i];
		if(arg=="--file"){
			if(i+1==argc){
				cerr<<"No file specified! Ignoring option \"--file\"."<<endl;
				break;
			}
			if(name_changed){
				cerr<<"Discarding previous --file argument \""<<name<<"\".";
			}
			name=argv[++i];
			cout<<"--file specified, loading \""<<name<<"\"..."<<endl;
			name_changed=true;
		}
		else if (arg == "--real") {
			phys_cart = true;
			std::cout << "Loading as physical cart...\n";
		}
		else if (arg == "--wait") {
			waiting=true;
		}
		else if(arg=="--clock-speed"){
			if(i+1==argc){
				cerr<<"--clock-speed requires an argument!"<<endl;
				break;
			}
			clock_speed=std::stoll(argv[++i]);
		}
		else if(arg=="--cpm"){
			cpm_emu=true;
			cout<<"CP/M BDOS syscall emulation activated. Loading program into address 0x100."<<endl;
			cout<<"Make sure that you have \".fill 0x100-$\" at the top of the .asm file"<<endl;
		}
		else if(arg=="--format=png"){
			cout<<"Decrypting PNG..."<<endl;
			format=PNG;
		}
		else{
			cerr<<"Unrecognized option \""<<arg<<"\", ignoring..."<<endl;
		}
	}
	if(!name_changed){
		cout<<"No --file argument received, defaulting to \"zenith.bin\"..."<<endl;
	}
	cpu=NULL;
	initCPU();
}

void Main::initCPU(){
	if (cpu) {
		delete cpu;
		cpu=0;
	}
	bool done=false;
	RAMController *r=0;
	DeviceController *d=0;
	while (!done) {
		try {
			if(waiting) {
				wait();
			}
			r=new RAMController(name);
			d=new DeviceController();
			cpu=new Z80(r,d);
			done=true;
		}
		catch(int i) {
			if(r){
				delete r;
				r=0;
			}
			if(d){
				delete d;
				d=0;
			}
		}
	}
	gpu=new GR80(canvas,cpu,gui,window);
	putmsg(((string)"Git revision: ")+STRINGIFY(GIT_REVISION));
	termOut->addLine("");
	termOut->addLine("");
	cpu->reset();
}

void Main::wait(){
	canvas->clear();
	sf::Text insert("Insert cartridge...",*default_font,18);
	insert.setPosition(400-insert.getGlobalBounds().width/2,300-insert.getGlobalBounds().height/2);
	canvas->draw(insert);
	while (!exists(name)) {
		sf::Event e;
		while (window->pollEvent(e)) {
			switch (e.type) {				
				case sf::Event::Closed:
					exit(0);
			}
		}
		window->clear(this->background);
		gui->draw();
		window->display();
		sf::sleep(sf::seconds(0.3));
	}
}

Main::~Main(){
	delete window;
	delete cpu;
	delete gpu;
	delete default_font;
}

void Main::update(){
	this->processEvents();
	window->clear(this->background);
	gui->draw();
}

int Main::run(){
	accuracy_clock.restart();
	precision_clock.restart();
	while(window->isOpen()){
		if (!exists(name)) {
			initCPU();
		}
		update();
		cpu->executeXInstructions(this->clock_speed*this->unit*precision_clock.restart().asSeconds());
		////gpu->execute();
		window->display();
	}
	return 0;
}

void Main::processEvents(){
	Event e;
	while(this->window->pollEvent(e)){
		switch(e.type){
			case Event::Closed:
				this->shutdown();
				break;
			case Event::MouseButtonPressed:
				if(e.mouseButton.button==Mouse::Button::Left){
					if(cpu->isHalted())
						cpu->reset();
				}
				else{
					if(cpu->isHalted())
						cpu->unhalt();
				}
				break;
			case Event::KeyPressed:
				{
					uint8_t key=keyCode(e.key);
					if(key){
						//limit buffer size to 32
						if(key_buffer.size()<32){
							key_buffer.push_back(key);
						}
					}
				}
				break;
			case Event::Resized:
				//window->setView(sf::View(sf::FloatRect(0, 0, e.size.width, e.size.height)));
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
		gpu->clearText();
		return termOut->removeAllLines();
	}
	string msg="";
	if(ch!='\n'){
		msg=termOut->getLine(termOut->getLineAmount()-1)+ch;
		termOut->removeLine(termOut->getLineAmount()-1);
	}
	putmsg(msg);
	gpu->appendText((string)""+ch);
}

void Main::putint(int i){
	string msg=termOut->getLine(termOut->getLineAmount()-1);
	msg=msg+to_string(i);
	termOut->removeLine(termOut->getLineAmount()-1);
	gpu->appendText(to_string(i));
	putmsg(msg);
}

void Main::putmsg(string s){
	termOut->addLine(s);
	gpu->clear(0);
}

uint8_t Main::key(){
	if(key_buffer.empty()){
		//if there isn't anything in the key buffer, just return 0
		return 0;
	}
	//fetch the value at the front
	uint8_t v=key_buffer.front();
	//then remove it from the buffer
	key_buffer.erase(key_buffer.begin());
	//return the value
	return v;
}

void Main::resetKeyBuffer(){
	key_buffer.clear();
}

void Main::resetClock(){
	accuracy_clock.restart();
}

void Main::shutdown(){
	cpu->savePMem();
	window->close();
	cout<<endl<<cpu->getTStates()<<" tstates"<<endl;
	cout<<"Target speed: "<<clock_speed;
	{
		string s;
		if(unit==MHz)
			s="MHz";
		if(unit==Hz)
			s="Hz";
		if(unit==GHz)
			s="GHz";
		if(unit==KHz)
			s="KHz";
		cout<<s<<endl;
		uint64_t speed = ((cpu->getTStates())/accuracy_clock.getElapsedTime().asSeconds());
		cout<<"Speed: "<<((float)speed/unit)<<s<<endl;
	}
}

string hex(int a){
	static char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	return (string)""+hex[(a&0xF000)>>12]+hex[(a&0xF00)>>8]+hex[(a&0xF0)>>4]+hex[a&0x0F];
}

string hex(uint8_t a){
	static char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	return (string)""+hex[(a&0xF0)>>4]+hex[a&0x0F];
}

void Main::setTitle(string s){
	window->setTitle(s);
}

bool exists(const char *name){
	ifstream f(name);
	return f.good();
}

ZENITH_FOOTER
