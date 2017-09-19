#include <CPU/peripherals/RAMController.hpp>
#include <zenith80.hpp>

#include <fstream>
using namespace std;

#include <main.hpp>
#include <program.hpp>

ZENITH_HEADER

RAMController::RAMController(const char * name){
	//load data from zenith.bin
	ifstream f(name, ios::in | ios::binary | ios::ate);
	if(Main::instance->cpm_emu){
		emu=new uint8_t[0x100]{
			0xC3,0x00,0x01,0x00,0x00,0xC9
		};
	}
    if (!f.is_open()){
		cout<<"Failed to load data from file, using compiled in program."<<endl;
		cout<<"Note: this usually means you don't have a file zenith.bin in the current working directory (if you don't know what that means, it's probably the folder this program is located within."<<endl;
		Main::instance->putmsg("Failed to load data from file, using compiled in program.");
		Main::instance->putmsg("Note: this usually means you don't have a file zenith.bin in the current working directory (if you don't know what that means, it's probably the folder this program is located within.");
		#ifdef PREBUILT_ASM_PROG
		memory=program;
		#else
		memory=new uint8_t[65536]{
			0x3E,0x48,0xD3,0x00,
			0x3E,0x45,0xD3,0x00,
			0x3E,0x4C,0xD3,0x00,
			0x3E,0x4C,0xD3,0x00,
			0x3E,0x4F,0xD3,0x00,
			0x3E,0x0A,0xD3,0x00,
			0x76
		};
		#endif
		//memory=new uint8_t[65536]{
			//0x00
		//};
		return;
	}
	memory=new uint8_t[65536];
	for(int i=0;i<65536;i++)
		memory[i]=0;
	cout<<"Loading program from "<<name<<endl;
	Main::instance->putmsg((string)"Loading program from "+name);
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

uint8_t RAMController::getByte(uint16_t address){
	Main::instance->cpu->tstates+=3;
	if(Main::instance->cpm_emu&&address<0x100)
		return emu[address];
	return memory[address];
}

uint8_t RAMController::getOpcode(uint16_t address){
	Main::instance->cpu->tstates+=4;
	if(Main::instance->cpm_emu&&address==5){
		uint8_t C=Main::instance->cpu->getBC().B.l;
		word DE=Main::instance->cpu->getDE();
		switch(C){
			case 0:
				cout<<"BDOS system reset - halting"<<endl;
				Main::instance->cpu->halt();
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
			return 0xC9;
		}
	}
	if(Main::instance->cpm_emu&&address<0x100)
		return emu[address];
	return memory[address];
}

uint16_t RAMController::getWord(uint16_t address){
	uint16_t lsb=getByte(address);
	uint16_t hsb=getByte((address+1));
	return (hsb<<8)+lsb;
}

void RAMController::setByte(uint16_t address,uint8_t value){
	Main::instance->cpu->tstates+=3;
	memory[address]=value;
}

void RAMController::setWord(uint16_t address,uint16_t value){
	setByte(address,value&0xFF);
	setByte(address+1,value>>8);
}

ZENITH_FOOTER
