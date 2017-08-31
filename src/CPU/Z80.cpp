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
	AF.word=AF2.word=BC.word=BC2.word=DE.word=DE2.word=HL.word=HL2.word=IX.word=IY.word=SP.word=PC.word=0;
	BC.word=8412;
	R=I=0;
	IFF1=IFF2=0;
	opcodes=new opcode[256];
	opcodes[0]=&Z80::nop;
	opcodes[1]=&Z80::ldDDNN;
}

void Z80::executeXInstructions(int x){
	for(int i=0;i<x;i++)
		executeOneInstruction();
}

void Z80::executeOneInstruction(){
	uint8_t opcode_value=this->ram->getByte(PC.word++);
	opcode a=opcodes[opcode_value];
	(*this.*a)(opcode_value);
};

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

ZENITH_FOOTER
