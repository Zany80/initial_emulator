#include <CPU/Z80.hpp>
#include <zenith80.hpp>

#include <cstdlib>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <config.h>

#include <string>
using std::string;

#include <main.hpp>

ZENITH_HEADER

#include "opcodes.cpp"

Z80::Z80(Z80Memory * ram,DeviceController * io){
	this->ram=ram;
	this->io=io;
	regs=vector<uint8_t*>{&BC.B.h,&BC.B.l,&DE.B.h,&DE.B.l,&HL.B.h,&HL.B.l,0,&AF.B.h};
	regsDD=vector<uint16_t*>{&BC.word,&DE.word,&HL.word,&SP.word};
	regsQQ=vector<uint16_t*>{&BC.word,&DE.word,&HL.word,&AF.word};
	r=vector<char>{'B','C','D','E','H','L',0,'A'};
	dd=vector<string>{"BC","DE","HL","SP"};
	initOpcodes();
	reset();
	halted=true;
}

void Z80::initOpcodes(){
	#include "opcodes/main.cpp"
	#include "opcodes/DD.cpp"
	#include "opcodes/ED.cpp"
	#include "opcodes/FD.cpp"
	#include "opcodes/CB.cpp"
	#include "opcodes/DDCB.cpp"
	#include "opcodes/FDCB.cpp"
}

Z80::~Z80(){
	delete opcodes;
	delete opcodesDD;
	delete opcodesFD;
	delete opcodesED;
	delete opcodesCB;
}

void Z80::reset(){
	AF.word=AF2.word=BC.word=BC2.word=DE.word=DE2.word=HL.word=HL2.word=IX.word=IY.word=SP.word=PC.word=0;
	R=I=0;
	IFF1=IFF2=0;
	tstates=0;
	Main::instance->resetClock();
	halted=false;
	Main::instance->resetKeyBuffer();
}

void Z80::executeXInstructions(int64_t x){
	int64_t frame_tstates=x;
	while(frame_tstates>0){
		int64_t p_tstates=tstates;
		executeOneInstruction();
		frame_tstates-=(tstates-p_tstates);
	}
}

string hex(uint8_t a){
	static char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	string h="";
	h+=hex[(a&0xF0)>>4];
	h+=hex[a&0x0F];
	return h;
}

void Z80::executeOneInstruction(){
	uint8_t _R = R;
	R++;
	R &= 0x7F;
	R |= (_R&0x80);
	if(halted)
		tstates+=4;
	else{
		uint8_t opcode_value=this->ram->getOpcode(PC.word++);
		#ifdef SUPERDEBUGMODE
		cerr<<"["<<PC.word<<"] Executing opcode 0x"<<hex(opcode_value);
		#endif
		(*this.*opcodes[opcode_value])(opcode_value);
	}
};

bool Z80::isHalted(){
	return halted;
}

void Z80::setC(){
	AF.B.l |= 0x01;
}
void Z80::setS(){
	AF.B.l |= 0x80;
}
void Z80::setZ(){
	AF.B.l |= 0x40;
}
void Z80::setN(){
	AF.B.l |= 0x02;
}
void Z80::setH(){
	AF.B.l |= 0x10;
}
void Z80::setPV(){
	AF.B.l |= 0x04;
}
bool Z80::getC(){
	return (AF.B.l & 0x01)==0x01;
}
bool Z80::getS(){
	return (AF.B.l & 0x80)==0x80;
}
bool Z80::getZ(){
	return (AF.B.l & 0x40)==0x40;
}
bool Z80::getN(){
	return (AF.B.l & 0x02)==0x02;
}
bool Z80::getH(){
	return (AF.B.l & 0x10)==0x10;
}
bool Z80::getPV(){
	return (AF.B.l & 0x04)==0x04;
}
void Z80::setC(bool b){
	b?setC():resetC();
}
void Z80::setS(bool b){
	b?setS():resetS();
}
void Z80::setZ(bool b){
	b?setZ():resetZ();
}
void Z80::setN(bool b){
	b?setN():resetN();
}
void Z80::setH(bool b){
	b?setH():resetH();
}
void Z80::setPV(bool b){
	b?setPV():resetPV();
}
void Z80::resetC(){
	AF.B.l &= 0xFE;
}
void Z80::resetS(){
	AF.B.l &= 0x7F;
}
void Z80::resetZ(){
	AF.B.l &= 0xBF;
}
void Z80::resetN(){
	AF.B.l &= 0xFD;
}
void Z80::resetH(){
	AF.B.l &= 0xEF;
}
void Z80::resetPV(){
	AF.B.l &= 0xFB;
}

int64_t Z80::getTStates(){
	return this->tstates;
}

void Z80::add_tstates(uint64_t tstates){
	this->tstates+=tstates;
}

word Z80::getBC(){
	return this->BC;
}

word Z80::getDE(){
	return this->DE;
}

void Z80::halt(){
	halted=true;
}

ZENITH_FOOTER
