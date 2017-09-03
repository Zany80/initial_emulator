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
	reset();
	opcodes=new opcode[256];
	opcodes_dd=new opcode[256];
	opcodes_fd=new opcode[256];
	opcodes_ed=new opcode[256];
	for(int i=0x00;i<=0xFF;i++)
		opcodes[i]=opcodes_dd[i]=opcodes_fd[i]=opcodes_ed[i]=&Z80::nop;
	opcodes[0x00]=&Z80::nop;
	opcodes[0xDD]=&Z80::DD;
	opcodes[0xFD]=&Z80::FD;
	opcodes[0xED]=&Z80::ED;
	opcodes[0x00]=&Z80::nop;
	opcodes[0x36]=&Z80::ld_HL_N;
	opcodes[0x0A]=&Z80::ldA_BC_;
	opcodes[0x1A]=&Z80::ldA_DE_;
	opcodes[0x3A]=&Z80::ldA_NN_;
	opcodes[0x02]=&Z80::ld_BC_A;
	opcodes[0x12]=&Z80::ld_DE_A;
	opcodes[0x32]=&Z80::ld_NN_A;
	opcodes_dd[0x36]=&Z80::ld_IXd_N;
	opcodes_fd[0x36]=&Z80::ld_IYd_N;
	opcodes_ed[0x57]=&Z80::ldAI;
	opcodes_ed[0x5F]=&Z80::ldAR;
	opcodes_ed[0x47]=&Z80::ldIA;
	opcodes_ed[0x4F]=&Z80::ldRA;
	opcodes_dd[0x21]=&Z80::ldIXNN;
	opcodes_fd[0x21]=&Z80::ldIYNN;
	opcodes[0x2A]=&Z80::ldHL_NN_;
	opcodes_dd[0x2A]=&Z80::ldIX_NN_;
	opcodes_fd[0x2A]=&Z80::ldIY_NN_;
	opcodes[0x22]=&Z80::ld_NN_HL;
	opcodes_dd[0x22]=&Z80::ld_NN_IX;
	opcodes_fd[0x22]=&Z80::ld_NN_IY;
	opcodes[0xF9]=&Z80::ldSPHL;
	opcodes_dd[0xF9]=&Z80::ldSPIX;
	opcodes_fd[0xF9]=&Z80::ldSPIY;
	opcodes_dd[0xE5]=&Z80::pushIX;
	opcodes_fd[0xE5]=&Z80::pushIY;
	opcodes_dd[0xE1]=&Z80::popIX;
	opcodes_fd[0xE1]=&Z80::popIY;
	opcodes[0xEB]=&Z80::exDEHL;
	opcodes[0x08]=&Z80::exAFAF2;
	opcodes[0xD9]=&Z80::exx;
	opcodes[0xE3]=&Z80::ex_SP_HL;
	opcodes_dd[0xE3]=&Z80::ex_SP_IX;
	opcodes_fd[0xE3]=&Z80::ex_SP_IY;
	opcodes_ed[0xA0]=&Z80::ldi;
	opcodes_ed[0xB0]=&Z80::ldir;
	opcodes_ed[0xA8]=&Z80::ldd;
	opcodes_ed[0xB8]=&Z80::lddr;
	opcodes_ed[0xA1]=&Z80::cpi;
	opcodes_ed[0xB1]=&Z80::cpir;
	opcodes_ed[0xA9]=&Z80::cpd;
	opcodes_ed[0xB9]=&Z80::cpdr;
	opcodes[0x76]=&Z80::halt;
	opcodes[0xD3]=&Z80::out_N_A;
	for(int i=0;i<255;i++){
		if((i&0xC0)==0x40
			&& ((i&38)>>3)!=6
			&& (i&0x07)!=6)
			opcodes[i]=&Z80::ldRRx;
		if((i&0xC7)==0x06
			&& ((i&0x38)>>3)!=6)
			opcodes[i]=&Z80::ldRN;
		if((i&0xC7)==0x46
			&& ((i&0x38)>>3)!=6){
			opcodes[i]=&Z80::ldR_HL_;
			opcodes_dd[i]=&Z80::ldR_IXd_;
			opcodes_fd[i]=&Z80::ldR_IYd_;
		}
		if((i&0xF8)==0x70
			&& (i&0x07)!=6){
			opcodes[i]=&Z80::ld_HL_R;
			opcodes_dd[i]=&Z80::ld_IXd_R;
			opcodes_fd[i]=&Z80::ld_IYd_R;
		}
		if((i&0xCF)==0x01)
			opcodes[i]=&Z80::ldDDNN;
		if((i&0xCF)==0x4B)
			opcodes_ed[i]=&Z80::ldDD_NN_;
		if((i&0xCF)==0x43)
			opcodes_ed[i]=&Z80::ld_NN_DD;
		if((i&0xCF)==0xC5)
			opcodes[i]=&Z80::pushQQ;
		if((i&0xCF)==0xC1)
			opcodes[i]=&Z80::popQQ;
	}
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

void Z80::executeXInstructions(int x){
	for(int i=0;i<x;i++){
		executeOneInstruction();
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

uint64_t Z80::getTStates(){
	return this->tstates;
}

void Z80::add_tstates(uint64_t tstates){
	this->tstates+=tstates;
}

ZENITH_FOOTER
