#include <CPU/peripherals/RAMController.hpp>
#include <zenith80.hpp>

#include <fstream>
using namespace std;

#include <main.hpp>
#include <program.hpp>

ZENITH_HEADER

RAMController::RAMController(const char * name){
	this->name=name;
	for(int i=0;i<4;i++){
		banks[i]=i;
	}
	ifstream f(name,ios::binary|ios::ate);
	if(f.is_open()){
		memory=new uint8_t[0x400000];
		streampos size=f.tellg();
		if(size>0x3FC000)
			size=0x3FC000;
		f.seekg(0);
		f.read((char*)memory,size);
		ifstream pmem(this->name+".pmem",ios::binary|ios::ate);
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
	else{
		Main::instance->putmsg("Failed to load file!");
		////memory=new uint8_t[256]{
			////0x3E,0x00,0xD3,0x00,0x3E,'!',0xD3,0x00,0x76,0x18,0xFD
		////};
		#ifdef PREBUILT_ASM_PROG
		PREBUILT_ASM_PROG
		#else
		#warning PREBUILT_ASM_PROG not defined!
		memory=new uint8_t[0x400000]{0x76,0x18,0xFD};
		#endif
	}
}

uint8_t *RAMController::getBank(uint8_t index){
	return memory+(banks[index]*0x4000);
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
	return getBank(bank)[address%0x4000];
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
	getBank(bank)[address%0x4000]=value;
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
	ofstream pmem_file(name+".pmem",ios::binary);
	if(pmem_file.is_open()){
		char *pmem=(char*)memory+(255*0x4000);
		pmem_file.write(pmem,0x4000);
		pmem_file.close();
	}
	else{
		cerr<<"[MMU] Couldn't save persistent memory!"<<endl;
	}
}

void RAMController::swapBanks(uint8_t index,uint8_t to){
	if(index<4){
		banks[index]=to;
		Main::instance->cpu->AF.B.h=0;
	}
	else{
		Main::instance->cpu->AF.B.h=1;
	}
}

ZENITH_FOOTER
