#include <CPU/Z80.hpp>
#include <zenith80.hpp>

#include <cstdlib>

#include <iostream>
using std::cout;using std::endl;

ZENITH_HEADER

#include "opcodes.cpp"

Z80::Z80(Z80Memory * ram,Z80Device * io){
	this->ram=ram;
	this->io=io;
	regs=new vector<uint8_t*>{&BC.B.h,&BC.B.l,&DE.B.h,&DE.B.l,&HL.B.h,&HL.B.l,0,&AF.B.h};
	regsDD=new vector<uint16_t*>{&BC.word,&DE.word,&HL.word,&SP.word};
	regsQQ=new vector<uint16_t*>{&BC.word,&DE.word,&HL.word,&AF.word};
	initOpcodes();
	reset();
}

void Z80::initOpcodes(){
	opcodes=new opcode[256];
	opcodesDD=new opcode[256];
	opcodesFD=new opcode[256];
	opcodesED=new opcode[256];
	for(int i=0x00;i<=0xFF;i++)
		opcodes[i]=opcodesDD[i]=opcodesFD[i]=opcodesED[i]=&Z80::nop;
}

Z80::~Z80(){
	delete regs;
	delete opcodes;
	delete regsDD;
	delete regsQQ;
}

void Z80::reset(){
	AF.word=AF2.word=BC.word=BC2.word=DE.word=DE2.word=HL.word=HL2.word=IX.word=IY.word=SP.word=PC.word=0;
	BC.word=512;
	R=I=0;
	IFF1=IFF2=0;
	tstates=0;
	halted=false;
}

void Z80::executeXInstructions(int64_t x){
	int64_t frame_tstates=x;
	while(frame_tstates>0){
		int64_t p_tstates=tstates;
		executeOneInstruction();
		frame_tstates-=(tstates-p_tstates);
	}
}

void Z80::executeOneInstruction(){
	uint8_t _R = R;
	R++;
	R &= 0x7F;
	R |= (_R&0x80);
	if(halted){
		(*this.*opcodes[0x00])(0x00);
		tstates+=4;
	}
	else{
		uint8_t opcode_value=this->ram->getOpcode(PC.word++);
		(*this.*opcodes[opcode_value])(opcode_value);
	}
};

bool Z80::isHalted(){
	return halted;
}

void Z80::setC(){
	//TODO: implement
}
void Z80::setS(){
	//TODO: implement
}
void Z80::setZ(){
	//TODO: implement
}
void Z80::setN(){
	//TODO: implement
}
void Z80::setH(){
	//TODO: implement
}
void Z80::setPV(){
	//TODO: implement
}
void Z80::setC(bool b){
	//TODO: implement
}
void Z80::setS(bool b){
	//TODO: implement
}
void Z80::setZ(bool b){
	//TODO: implement
}
void Z80::setN(bool b){
	//TODO: implement
}
void Z80::setH(bool b){
	//TODO: implement
}
void Z80::setPV(bool b){
	//TODO: implement
}
void Z80::resetC(){
	//TODO: implement
}
void Z80::resetS(){
	//TODO: implement
}
void Z80::resetZ(){
	//TODO: implement
}
void Z80::resetN(){
	//TODO: implement
}
void Z80::resetH(){
	//TODO: implement
}
void Z80::resetPV(){
	//TODO: implement
}

int64_t Z80::getTStates(){
	return this->tstates;
}

void Z80::add_tstates(uint64_t tstates){
	this->tstates+=tstates;
}

ZENITH_FOOTER
