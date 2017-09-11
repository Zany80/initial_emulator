#include <CPU/Z80.hpp>
#include <zenith80.hpp>

#include <cstdlib>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <config.h>

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
	for(int i=0x00;i<256;i++)
		opcodes[i]=opcodesDD[i]=opcodesFD[i]=opcodesED[i]=&Z80::nop;
	opcodes[0xED]=&Z80::ED;
	opcodes[0xDD]=&Z80::DD;
	opcodes[0xFD]=&Z80::FD;
	opcodes[0xD3]=&Z80::out_N_A;
	opcodes[0x36]=&Z80::ld_HL_N;
	opcodesDD[0x36]=&Z80::ld_IXd_N;
	opcodesFD[0x36]=&Z80::ld_IYd_N;
	opcodes[0x0A]=&Z80::ldA_BC_;
	opcodes[0x1A]=&Z80::ldA_DE_;
	opcodes[0x3A]=&Z80::ldA_NN_;
	opcodes[0x02]=&Z80::ld_BC_A;
	opcodes[0x12]=&Z80::ld_DE_A;
	opcodes[0x32]=&Z80::ld_NN_A;
	opcodesED[0x57]=&Z80::ldAI;
	opcodesED[0x5F]=&Z80::ldAR;
	opcodesED[0x47]=&Z80::ldIA;
	opcodesED[0x4F]=&Z80::ldRA;
	opcodesDD[0x21]=&Z80::ldIXNN;
	opcodesFD[0x21]=&Z80::ldIYNN;
	opcodes[0x2A]=&Z80::ldHL_NN_;
	opcodesDD[0x2A]=&Z80::ldIX_NN_;
	opcodesFD[0x2A]=&Z80::ldIY_NN_;
	opcodes[0x22]=&Z80::ld_NN_HL;
	opcodesDD[0x22]=&Z80::ld_NN_IX;
	opcodesFD[0x22]=&Z80::ld_NN_IY;
	opcodes[0xF9]=&Z80::ldSPHL;
	opcodesDD[0xF9]=&Z80::ldSPIX;
	opcodesFD[0xF9]=&Z80::ldSPIY;
	opcodesDD[0xE5]=&Z80::pushIX;
	opcodesFD[0xE5]=&Z80::pushIY;
	opcodesDD[0xE1]=&Z80::popIX;
	opcodesFD[0xE1]=&Z80::popIY;
	opcodes[0xEB]=&Z80::exDEHL;
	opcodes[0x08]=&Z80::exAFAF2;
	opcodes[0xD9]=&Z80::exx;
	opcodes[0xE3]=&Z80::ex_SP_HL;
	opcodesDD[0xE3]=&Z80::ex_SP_IX;
	opcodesFD[0xE3]=&Z80::ex_SP_IY;
	opcodesED[0xA0]=&Z80::ldi;
	opcodesED[0xB0]=&Z80::ldir;
	opcodesED[0xA8]=&Z80::ldd;
	opcodesED[0xA8]=&Z80::lddr;
	opcodesED[0xA1]=&Z80::cpi;
	opcodesED[0xB1]=&Z80::cpir;
	opcodesED[0xA9]=&Z80::cpd;
	opcodesED[0xA9]=&Z80::cpdr;
	opcodes[0xC6]=&Z80::addAN;
	opcodes[0x86]=&Z80::addA_HL_;
	opcodesDD[0x86]=&Z80::addA_IXd_;
	opcodesFD[0x86]=&Z80::addA_IYd_;
	opcodes[0xCE]=&Z80::adcAN;
	opcodes[0x8E]=&Z80::adcA_HL_;
	opcodesDD[0x8E]=&Z80::adcA_IXd_;
	opcodesFD[0x8E]=&Z80::adcA_IYd_;
	opcodes[0xD6]=&Z80::subN;
	opcodes[0x96]=&Z80::sub_HL_;
	opcodesDD[0x96]=&Z80::sub_IXd_;
	opcodesFD[0x96]=&Z80::sub_IYd_;
	opcodes[0xDE]=&Z80::sbcAN;
	opcodes[0x9E]=&Z80::sbcA_HL_;
	opcodesDD[0x9E]=&Z80::sbcA_IXd_;
	opcodesFD[0x9E]=&Z80::sbcA_IYd_;
	opcodes[0xE6]=&Z80::andN;
	opcodes[0xA6]=&Z80::and_HL_;
	opcodesDD[0xA6]=&Z80::and_IXd_;
	opcodesFD[0xA6]=&Z80::and_IYd_;
	opcodes[0xF6]=&Z80::orN;
	opcodes[0xB6]=&Z80::or_HL_;
	opcodesDD[0xB6]=&Z80::or_IXd_;
	opcodesFD[0xB6]=&Z80::or_IYd_;
	opcodes[0xEE]=&Z80::xorN;
	opcodes[0xAE]=&Z80::xor_HL_;
	opcodesDD[0xAE]=&Z80::xor_IXd_;
	opcodesFD[0xAE]=&Z80::xor_IYd_;
	opcodes[0xFE]=&Z80::cpN;
	opcodes[0xBE]=&Z80::cp_HL_;
	opcodesDD[0xBE]=&Z80::cp_IXd_;
	opcodesFD[0xBE]=&Z80::cp_IYd_;
	opcodes[0x34]=&Z80::inc_HL_;
	opcodesDD[0x34]=&Z80::inc_IXd_;
	opcodesFD[0x34]=&Z80::inc_IYd_;
	opcodes[0x35]=&Z80::dec_HL_;
	opcodesDD[0x35]=&Z80::dec_IXd_;
	opcodesFD[0x35]=&Z80::dec_IYd_;
	opcodes[0x27]=&Z80::daa;
	opcodes[0x2F]=&Z80::cpl;
	opcodesED[0x44]=&Z80::neg;
	opcodes[0x3F]=&Z80::ccf;
	opcodes[0x37]=&Z80::scf;
	opcodes[0x76]=&Z80::halt;
	opcodes[0xF3]=&Z80::di;
	opcodes[0xFB]=&Z80::ei;
	opcodesED[0x46]=opcodesED[0x56]=opcodesED[0x5E]=&Z80::im;
	opcodesDD[0x23]=&Z80::incIX;
	opcodesFD[0x23]=&Z80::incIY;
	opcodesDD[0x2B]=&Z80::decIX;
	opcodesFD[0x2B]=&Z80::decIY;
	opcodes[0x07]=&Z80::rlca;
	opcodes[0xC3]=&Z80::jpNN;
	opcodes[0x10]=&Z80::djnzE;
	opcodes[0xCD]=&Z80::callNN;
	opcodes[0xC9]=&Z80::ret;
	for(int i=0;i<256;i++){
		if((i&0xC0)==0x40
			&& ((i&0x38)>>3)!=6
			&& (i&0x07)!=6)
			opcodes[i]=&Z80::ldRRx;
		if((i&0xC7)==0x06
			&& ((i&0x38)>>3)!=6)
			opcodes[i]=&Z80::ldRN;
		if((i&0xC7)==0x46
			&& ((i&0x38)>>3)!=6){
			opcodes[i]=&Z80::ldR_HL_;
			opcodesDD[i]=&Z80::ldR_IXd_;
			opcodesFD[i]=&Z80::ldR_IYd_;
		}
		if((i&0xF8)==0x70
			&& (i&0x07)!=6){
			opcodes[i]=&Z80::ld_HL_R;
			opcodesDD[i]=&Z80::ld_IXd_R;
			opcodesFD[i]=&Z80::ld_IYd_R;
		}
		if((i&0xCF)==0x01)
			opcodes[i]=&Z80::ldDDNN;
		if((i&0xCF)==0x4B)
			opcodesED[i]=&Z80::ldDD_NN_;
		if((i&0xCF)==0x43)
			opcodesED[i]=&Z80::ld_NN_DD;
		if((i&0xCF)==0xC5)
			opcodes[i]=&Z80::pushQQ;
		if((i&0xCF)==0xC1)
			opcodes[i]=&Z80::popQQ;
		if((i&0xF8)==0x80
			&& (i&0x07)!=6)
			opcodes[i]=&Z80::addAR;
		if((i&0xF8)==0x88
			&& (i&0x07)!=6)
			opcodes[i]=&Z80::adcAR;
		if((i&0xF8)==0x90
			&& (i&0x07)!=6)
			opcodes[i]=&Z80::subR;
		if((i&0xF8)==0x98
			&& (i&0x07)!=6)
			opcodes[i]=&Z80::sbcAR;
		if((i&0xF8)==0xA0
			&& (i&0x07)!=6)
			opcodes[i]=&Z80::andR;
		if((i&0xF8)==0xB0
			&& (i&0x07)!=6)
			opcodes[i]=&Z80::orR;
		if((i&0xF8)==0xA1
			&& (i&0x07)!=6)
			opcodes[i]=&Z80::xorR;
		if((i&0xC7)==0x04
			&& ((i&0x38)>>3)!=6)
			opcodes[i]=&Z80::incR;
		if((i&0xC7)==0x05
			&& ((i&0x38)>>3)!=6)
			opcodes[i]=&Z80::decR;
		if((i&0xC7)==0xC2)
			opcodes[i]=&Z80::jpCCNN;
		if((i&0xC7)==0xC4)
			opcodes[i]=&Z80::callCCNN;
		if((i&0xC7)==0xC0)
			opcodes[i]=&Z80::retCC;
		if((i&0xCF)==0x09){
			opcodes[i]=&Z80::addHLSS;
			opcodesDD[i]=&Z80::addIXPP;
			opcodesFD[i]=&Z80::addIYRR;
		}
		if((i&0xCF)==0x4A)
			opcodesED[i]=&Z80::adcHLSS;
		if((i&0xCF)==0x42)
			opcodesED[i]=&Z80::sbcHLSS;
		if((i&0xCF)==0x03)
			opcodes[i]=&Z80::incSS;
		if((i&0xCF)==0x0B)
			opcodes[i]=&Z80::decSS;
	}
}

Z80::~Z80(){
	delete opcodes;
	delete opcodesDD;
	delete opcodesFD;
	delete opcodesED;
	delete regs;
	delete regsDD;
	delete regsQQ;
}

void Z80::reset(){
	AF.word=AF2.word=BC.word=BC2.word=DE.word=DE2.word=HL.word=HL2.word=IX.word=IY.word=SP.word=PC.word=0;
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
		#ifdef SUPERDEBUGMODE
		cerr<<"Executing opcode "<<(int)opcode_value;
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
