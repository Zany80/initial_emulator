#include <CPU/peripherals/RAMController.hpp>
#include <zenith80.hpp>

#include <fstream>
#include <iostream>
using namespace std;

#include <cstring>
#include <cstdlib>

#include <main.hpp>
#include <program.hpp>

ZENITH_HEADER

RAMController::RAMController(const char * name){
	this->name=name;
	for(int i=0;i<4;i++){
		banks[i]=i;
	}
	ifstream f;
	if (Main::instance->phys_cart) {
		system("rm .zenith.png -f");
		string s="dd if=";
		s+=name;
		s+=" of=.zenith.png count=8000";
		system(s.c_str());
		f = ifstream(".zenith.png",ios::binary | ios::ate);
	}
	else {
		f = ifstream(name,ios::binary | ios::ate);
	}
	if(f.is_open()){
		streampos size=f.tellg();
		if (size > 0x800000) {
			// Make sure there's enough room for the cart, but not so much it can crash - image size has to be lower than 4MB
			size = 0x800000;
		}
		f.seekg(0);
		if(Main::instance->format==PNG){
			char *contents=new char[size];
			f.read(contents,size);
			f.close();
			cout<<"Searching image for Zenith80 ROM header...\n";
			int start=-1;
			for(int i=0;i<size;i++){
				if(strncmp("ZENITH",contents+i,6)==0){
					start=i;
					break;
				}
			}
			if (start>-1) {
				metadata_t* metadata=(metadata_t*)(contents+start);
				cout<<"ROM found! Title: \"";
				cout<<(const char*)(contents+start+metadata->title)<<"\". Loading...\n";
				Main *m = Main::instance;
				m->canvas->clear();
				sf::Text title((const char*)(contents+start+metadata->title),*m->default_font,18);
				title.setPosition(400-title.getGlobalBounds().width/2,150-title.getGlobalBounds().height/2);
				sf::Text should_load("Load this cart? ('Y' to load, 'N' to shut down)",*m->default_font,14);
				should_load.setPosition(400-should_load.getGlobalBounds().width/2,330-should_load.getGlobalBounds().height/2);
				sf::Texture icon;
				sf::Sprite icon_sprite;
				if (m->format==PNG) {
					if (icon.loadFromMemory(contents,start)) {
						icon_sprite.setTexture(icon);
						while (icon_sprite.getGlobalBounds().width < 128) {
							sf::Vector2f s = icon_sprite.getScale();
							icon_sprite.setScale(s.x + 0.25, s.y);
						}
						while (icon_sprite.getGlobalBounds().height < 128) {
							sf::Vector2f s = icon_sprite.getScale();
							icon_sprite.setScale(s.x, s.y + 0.25);
						}
						while (icon_sprite.getGlobalBounds().width > 160) {
							sf::Vector2f s = icon_sprite.getScale();
							icon_sprite.setScale(s.x - 0.1, s.y);
						}
						while (icon_sprite.getGlobalBounds().height > 160) {
							sf::Vector2f s = icon_sprite.getScale();
							icon_sprite.setScale(s.x, s.y - 0.1);
						}
						icon_sprite.setPosition(400-icon_sprite.getGlobalBounds().width/2,title.getPosition().y+title.getGlobalBounds().height);
						m->canvas->draw(icon_sprite);
					}
				}
				else {
					sf::Text no_image("(no icon provided)",*m->default_font,14);
					no_image.setPosition(400-no_image.getGlobalBounds().width/2,title.getPosition().y+title.getGlobalBounds().height);
					m->canvas->draw(no_image);
				}
				m->canvas->draw(title);
				m->canvas->draw(should_load);
				while (true) {
					m->window->clear(m->background);
					m->gui->draw();
					m->window->display();
					sf::Event e;
					while (m->window->pollEvent(e)) {
						switch (e.type) {
							case sf::Event::Closed:
								exit(0);
								return;
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
						sf::Event e;
						while(m->window->pollEvent(e)){
							/* Make sure that "y" isn't added to the key buffer */
						}
						break;
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
						m->canvas->clear();
						sf::Text bye("Okay then. Shutting down...",*m->default_font,18);
						bye.setPosition(400-bye.getGlobalBounds().width/2,150-bye.getGlobalBounds().height/2);
						m->canvas->draw(bye);
						m->update();
						sf::sleep(sf::seconds(3));
						exit(0);
					}
				}
				Main::instance->resetKeyBuffer();
				int length=(int)size-(int)start;
				memory=new uint8_t[0x400000];
				for(int i=0;i<length;i++){
					memory[i]=contents[start+i];
				}
				ifstream pmem;
				if (Main::instance->phys_cart) {
					string command = "dd if=";
					command += name;
					command += " of=.zenith.pmem skip=8000 count=32";
					system(command.c_str());
					std::cout << "Executing `"<<command<<"`...\n";
					pmem = ifstream(".zenith.pmem",ios::binary|ios::ate);
				}
				else {
					pmem = ifstream(this->name+".pmem",ios::binary|ios::ate);
				}
				if(pmem.is_open()){
					streampos size=pmem.tellg();
					if(size>0x4000)
						size=0x4000;
					pmem.seekg(0);
					pmem.read((char*)memory+0x3FC000,size);
					pmem.close();
				}
			}
			else{
				cout<<"Unable to load ROM! Falling back to embedded program.\n";
				#ifdef PREBUILT_ASM_PROG
				PREBUILT_ASM_PROG
				#else
				#warning PREBUILT_ASM_PROG not defined!
				memory=new uint8_t[0x400000]{0x06,0x02,0x00,0x76,0x18,0xFD,'H','e','l','l','o',' ','W','o','r','l','d','!',0x00};
				#endif
			}
			delete[] contents;
		}
		else{
			memory=new uint8_t[0x400000];
			if(size>0x3FC000)
				size=0x3FC000;
			f.read((char*)memory,size);
			ifstream pmem;
			if (Main::instance->phys_cart) {
				string command = "dd if=";
				command += name;
				command += " of=.zenith.pmem skip=8000 count=32";
				system(command.c_str());
				std::cout << "Executing `"<<command<<"`...\n";
				pmem = ifstream(".zenith.pmem",ios::binary|ios::ate);
			}
			else {
				pmem = ifstream(this->name+".pmem",ios::binary|ios::ate);
			}
			if(pmem.is_open()){
				streampos size=pmem.tellg();
				if(size>0x4000)
					size=0x4000;
				pmem.seekg(0);
				pmem.read((char*)memory+0x3FC000,size);
				pmem.close();
			}
			f.close();
		}
	}
	else{
		cerr<<"Failed to load file!"<<endl;
		////memory=new uint8_t[256]{
			////0x3E,0x00,0xD3,0x00,0x3E,'!',0xD3,0x00,0x76,0x18,0xFD
		////};
		#ifdef PREBUILT_ASM_PROG
		PREBUILT_ASM_PROG
		#else
		#warning PREBUILT_ASM_PROG not defined!
		memory=new uint8_t[0x400000]{0x06,0x02,0x00,0x76,0x18,0xFD,'H','e','l','l','o',' ','W','o','r','l','d','!',0x00};
		#endif
	}
}

RAMController::~RAMController(){
	delete[] memory;
}

void RAMController::parseMetadata(){
	metadata_t *metadata=(metadata_t*)getBankFromAddress(0x0000);
	Main::instance->setTitle((string)"["+(const char*)(getBankFromAddress(metadata->title)+(metadata->title%0x4000))+"] - Zenith80");
	Main::instance->cpu->start_address=metadata->start_address;
}

uint8_t *RAMController::getBankByIndex(uint8_t index){
	return memory+(banks[index]*0x4000);
}

uint8_t *RAMController::getBankFromAddress(uint16_t virt_address){
	uint8_t bank;
	if(virt_address<0x4000)
		bank=0;
	else if(virt_address<0x8000)
		bank=1;
	else if(virt_address<0xC000)
		bank=2;
	else
		bank=3;
	return getBankByIndex(bank);
}

uint8_t RAMController::getOpcode(uint16_t address){
	Main::instance->cpu->tstates++;
	return getByte(address);
}

uint8_t RAMController::getByte(uint16_t address){
	Main::instance->cpu->tstates+=3;
	uint8_t bank;
	if(address<0x4000)
		bank=0;
	else if(address<0x8000)
		bank=1;
	else if(address<0xC000)
		bank=2;
	else
		bank=3;
	return getBankByIndex(bank)[address%0x4000];
}

void RAMController::setByte(uint16_t address,uint8_t value){
	uint8_t bank;
	if(address<0x4000)
		bank=0;
	else if(address<0x8000)
		bank=1;
	else if(address<0xC000)
		bank=2;
	else
		bank=3;
	getBankByIndex(bank)[address%0x4000]=value;
}

uint16_t RAMController::getWord(uint16_t address){
	uint16_t lsb=getByte(address);
	uint16_t hsb=getByte((address+1));
	return (hsb<<8)+lsb;
}

void RAMController::setWord(uint16_t address,uint16_t value){
	setByte(address,value&0xFF);
	setByte(address+1,value>>8);
}

void RAMController::savePMem(){
	ofstream pmem_file;
	if (Main::instance->phys_cart) {
		pmem_file = ofstream(".zenith.pmem",ios::binary | ios::ate);
	}
	else {
		pmem_file = ofstream(name+".pmem",ios::binary | ios::ate);
	}
	if(pmem_file.is_open()){
		char *pmem=(char*)memory+(255*0x4000);
		pmem_file.write(pmem,0x4000);
		pmem_file.close();
		if (Main::instance->phys_cart) {
			string command = "dd if=.zenith.pmem of=";
			command += name;
			command += " seek=8000";
			system(command.c_str());
			std::remove(".zenith.pmem");
		}
	}
	else{
		cerr<<"[MMU] Couldn't save persistent memory!"<<endl;
	}
}

void RAMController::swapBanks(uint8_t index,uint8_t to){
	if(index<4){
		cout<<"[RAM] Swapping bank "<<(int)index<<"to "<<(int)to<<"...\n";
		banks[index]=to;
		Main::instance->cpu->HL.word=0;
	}
	else{
		Main::instance->cpu->HL.word=1;
	}
}

ZENITH_FOOTER
