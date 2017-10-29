#include <CPU/peripherals/RAMController.hpp>
#include <zenith80.hpp>

#include <fstream>
using namespace std;

#include <main.hpp>
#include <program.hpp>

ZENITH_HEADER

RAMController::RAMController(const char * name){
	this->name=name;
	bank=0;
	for(int i=0;i<256;i++){
		banks.push_back(new uint8_t[0x4000]);
	}

}

uint8_t RAMController::getOpcode(uint16_t address){
	
}

uint8_t RAMController::getByte(uint16_t address){
	
}

void RAMController::setByte(uint16_t address,uint8_t value){
	
}

uint16_t RAMController::getWord(uint16_t address){
	
}

void RAMController::setWord(uint16_t address,uint16_t value){
	
}

void RAMController::savePMem(){
	
}

void RAMController::swapBanks(uint8_t from,uint8_t to){
	
}


ZENITH_FOOTER
