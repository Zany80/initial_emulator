#include <config.h>

//prefixes

void Z80::DD(uint8_t opcode){
	uint8_t secondary_opcode=ram->getOpcode(PC.word++);
	SUPERDEBUG("(DD prefix). Secondary opcode: "<<hex(secondary_opcode));
	(*this.*(opcodesDD[secondary_opcode]))(secondary_opcode);
}

void Z80::FD(uint8_t opcode){
	uint8_t secondary_opcode=ram->getOpcode(PC.word++);
	SUPERDEBUG("(FD prefix). Secondary opcode: "<<hex(secondary_opcode));
	(*this.*(opcodesFD[secondary_opcode]))(secondary_opcode);
}

void Z80::ED(uint8_t opcode){
	uint8_t secondary_opcode=ram->getOpcode(PC.word++);
	SUPERDEBUG("(ED prefix). Secondary opcode: "<<hex(secondary_opcode));
	(*this.*(opcodesED[secondary_opcode]))(secondary_opcode);
}

//8-bit load group

void Z80::ldRRx(uint8_t opcode){
	SUPERDEBUG("(`ld "<<r[int(opcode&0x07)]<<", "<<r[(opcode&0x38)>>3]<<"`). Value: "<<(int)*((*regs)[opcode&0x07])<<endl);
	*((*regs)[(opcode&0x38)>>3])=*((*regs)[opcode&0x07]);
}

void Z80::ldRN(uint8_t opcode){
	SUPERDEBUG(endl);
	uint8_t n=ram->getByte(PC.word++);
	*((*regs)[(opcode&0x38)>>3])=n;
}

void Z80::ldR_HL_(uint8_t opcode){
	SUPERDEBUG(endl);
	*((*regs)[(opcode&0x38)>>3])=ram->getByte(HL.word);
}

void Z80::ldR_IXd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=(int8_t)ram->getByte(PC.word++);
	*(*regs)[(opcode&0x38)>>3]=ram->getByte(IX.word+d);
	tstates+=5;
}

void Z80::ldR_IYd_(uint8_t opcode){
	SUPERDEBUG(endl);
	uint8_t * reg=(*regs)[(opcode&0x38)>>3];
	int8_t d=(int8_t)ram->getByte(PC.word++);
	tstates+=5;
	*reg=ram->getByte(IY.word+d);
}

void Z80::ld_HL_R(uint8_t opcode){
	SUPERDEBUG(endl);
	ram->setByte(HL.word,*((*regs)[opcode&0x07]));
}

void Z80::ld_IXd_R(uint8_t opcode){
	SUPERDEBUG(endl);
	uint8_t * reg=(*regs)[opcode&0x07];
	int8_t d=(int8_t)ram->getByte(PC.word++);
	tstates+=5;
	ram->setByte(IX.word+d,*reg);
}

void Z80::ld_IYd_R(uint8_t opcode){
	SUPERDEBUG(endl);
	uint8_t * reg=(*regs)[opcode&0x07];
	int8_t d=(int8_t)ram->getByte(PC.word++);
	tstates+=5;
	ram->setByte(IY.word+d,*reg);
}

void Z80::ld_HL_N(uint8_t opcode){
	SUPERDEBUG(endl);
	ram->setByte(HL.word,ram->getByte(PC.word++));
}

void Z80::ld_IXd_N(uint8_t opcode){
	SUPERDEBUG(endl);
	//19 tstates for this opcode - 4 for the DD fetch, 4 for the 36 fetch, and another 11
	//getByte adds 3, as does setByte. Those are called 3 times in total -or 9 tstates
	//this leaves 2, and frankly I'm confused. The docs say 4,4,3,5,3 t states - 4 for the first two fetches, 3 for the read,
	//5 for ???, and 3 for the write
	//^ maybe that's 3 for read + 2 for math?
	//repeat above comments for ld_IYd_N
	tstates+=2;
	int8_t d=(int8_t)ram->getByte(PC.word++);
	ram->setByte(IX.word+d,ram->getByte(PC.word++));
}

void Z80::ld_IYd_N(uint8_t opcode){
	SUPERDEBUG(endl);
	tstates+=2;
	int8_t d=(int8_t)ram->getByte(PC.word++);
	ram->setByte(IY.word+d,ram->getByte(PC.word++));
}

void Z80::ldA_BC_(uint8_t opcode){
	SUPERDEBUG(endl);
	AF.B.h=ram->getByte(BC.word);
}

void Z80::ldA_DE_(uint8_t opcode){
	SUPERDEBUG(endl);
	AF.B.h=ram->getByte(DE.word);
}

void Z80::ldA_NN_(uint8_t opcode){
	SUPERDEBUG(endl);
	AF.B.h=ram->getByte(ram->getWord(PC.word));
	PC.word+=2;
}

void Z80::ld_BC_A(uint8_t opcode){
	SUPERDEBUG(endl);
	ram->setByte(BC.word,AF.B.h);
}

void Z80::ld_DE_A(uint8_t opcode){
	SUPERDEBUG(endl);
	ram->setByte(DE.word,AF.B.h);
}

void Z80::ld_NN_A(uint8_t opcode){
	SUPERDEBUG(endl);
	ram->setByte(ram->getWord(PC.word),AF.B.h);
	PC.word+=2;
}

void Z80::ldAI(uint8_t opcode){
	SUPERDEBUG(endl);
	tstates+=5;
	AF.B.h=I;
	setS((I&0x80)==0x80);
	setZ(I==0);
	resetH();
	setPV(IFF2);
	resetN();
}

void Z80::ldAR(uint8_t opcode){
	SUPERDEBUG(endl);
	tstates+=5;
	AF.B.h=R;
	setS((R&0x80)==0x80);
	setZ(R==0);
	resetH();
	setPV(IFF2);
	resetN();
}

void Z80::ldIA(uint8_t opcode){
	SUPERDEBUG(endl);
	tstates+=5;
	I=AF.B.h;
}

void Z80::ldRA(uint8_t opcode){
	SUPERDEBUG(endl);
	tstates+=5;
	R=AF.B.h;
}

//16-bit load group

void Z80::ldDDNN(uint8_t opcode){
	SUPERDEBUG(endl);
	uint16_t val=ram->getWord(PC.word);
	PC.word+=2;
	*((*regsDD)[(opcode&0x30)>>4])=val;
}

void Z80::ldIXNN(uint8_t opcode){
	SUPERDEBUG(endl);
	IX.word=ram->getWord(PC.word);
	PC.word+=2;
}

void Z80::ldIYNN(uint8_t opcode){
	SUPERDEBUG(endl);
	IY.word=ram->getWord(PC.word);
	PC.word+=2;
}

void Z80::ldHL_NN_(uint8_t opcode){
	SUPERDEBUG(endl);
	HL.word=ram->getWord(ram->getWord(PC.word));
	PC.word+=2;
}

void Z80::ldDD_NN_(uint8_t opcode){
	SUPERDEBUG(endl);
	*((*regsDD)[(opcode&0x30)>>4])=ram->getWord(ram->getWord(PC.word));
	PC.word+=2;
}

void Z80::ldIX_NN_(uint8_t opcode){
	SUPERDEBUG(endl);
	IX.word=ram->getWord(ram->getWord(PC.word));
	PC.word+=2;
}

void Z80::ldIY_NN_(uint8_t opcode){
	SUPERDEBUG(endl);
	IY.word=ram->getWord(ram->getWord(PC.word));
	PC.word+=2;
}

void Z80::ld_NN_HL(uint8_t opcode){
	SUPERDEBUG(endl);
	ram->setWord(ram->getWord(PC.word),HL.word);
	PC.word+=2;
}

void Z80::ld_NN_DD(uint8_t opcode){
	SUPERDEBUG(endl);
	ram->setWord(ram->getWord(PC.word),*((*regsDD)[(opcode&0x30)>>4]));
	PC.word+=2;
}

void Z80::ld_NN_IX(uint8_t opcode){
	SUPERDEBUG(endl);
	ram->setWord(ram->getWord(PC.word),IX.word);
	PC.word+=2;
}

void Z80::ld_NN_IY(uint8_t opcode){
	SUPERDEBUG(endl);
	ram->setWord(ram->getWord(PC.word),IY.word);
	PC.word+=2;
}

void Z80::ldSPHL(uint8_t opcode){
	SUPERDEBUG(endl);
	//takes 6 tstates for some reason. Since 4 are needed for opcode fetch, add 2
	tstates+=2;
	SP.word=HL.word;
}

void Z80::ldSPIX(uint8_t opcode){
	SUPERDEBUG(endl);
	//takes 6 tstates for some reason. Since 4 are needed for opcode fetch, add 2
	tstates+=2;
	SP.word=IX.word;
}

void Z80::ldSPIY(uint8_t opcode){
	SUPERDEBUG(endl);
	//takes 6 tstates for some reason. Since 4 are needed for opcode fetch, add 2
	tstates+=2;
	SP.word=IY.word;
}

void Z80::pushQQ(uint8_t opcode){
	SUPERDEBUG(endl);
	//takes 11 tstates. opcode fetch=4, push=ram->setWord=2x ram->setByte=6, add 1
	tstates++;
	push(*((*regsQQ)[(opcode&0x30)>>4]));
}

void Z80::pushIX(uint8_t opcode){
	SUPERDEBUG(endl);
	//takes 15 tstates. 2x opcode fetch=8, push=ram->setWord=2x ram->setByte=6, add 1
	tstates++;
	push(IX.word);
}

void Z80::pushIY(uint8_t opcode){
	SUPERDEBUG(endl);
	//takes 15 tstates. 2x opcode fetch=8, push=ram->setWord=2x ram->setByte=6, add 1
	tstates++;
	push(IY.word);
}

void Z80::popQQ(uint8_t opcode){
	SUPERDEBUG(endl);
	*((*regsQQ)[(opcode&0x30)>>4])=pop();
}

void Z80::popIX(uint8_t opcode){
	SUPERDEBUG(endl);
	IX.word=pop();
}

void Z80::popIY(uint8_t opcode){
	SUPERDEBUG(endl);
	IY.word=pop();
}

//exchange, block transfer, and search group

void Z80::exDEHL(uint8_t opcode){
	SUPERDEBUG(endl);
	uint16_t t=DE.word;
	DE.word=HL.word;
	HL.word=t;
}

void Z80::exAFAF2(uint8_t opcode){
	SUPERDEBUG(endl);
	uint16_t t=AF.word;
	AF.word=AF2.word;
	AF2.word=t;
}

void Z80::exx(uint8_t opcode){
	SUPERDEBUG(endl);
	uint16_t t1=BC.word,t2=DE.word,t3=HL.word;
	BC.word=BC2.word;DE.word=DE2.word;HL.word=HL2.word;
	HL2.word=t1;DE2.word=t2;HL2.word=t3;
}

void Z80::ex_SP_HL(uint8_t opcode){
	SUPERDEBUG(endl);
	word t=HL;
	HL.word=ram->getWord(SP.word);
	ram->setWord(SP.word,t.word);
	tstates+=3;
}

void Z80::ex_SP_IX(uint8_t opcode){
	SUPERDEBUG(endl);
	word t=IX;
	IX.word=ram->getWord(SP.word);
	ram->setWord(SP.word,t.word);
	tstates+=3;
}

void Z80::ex_SP_IY(uint8_t opcode){
	SUPERDEBUG(endl);
	word t=IY;
	IY.word=ram->getWord(SP.word);
	ram->setWord(SP.word,t.word);
	tstates+=3;
}

void Z80::ldi(uint8_t opcode){
	SUPERDEBUG(endl);
	ram->setByte(DE.word,ram->getByte(HL.word));
	DE.word++;HL.word++;
	BC.word--;
	tstates+=2;
	resetH();
	setPV(BC.word!=0);
	resetN();
}

void Z80::ldir(uint8_t opcode){
	SUPERDEBUG("(`ldir`)."<<endl);
	ram->setByte(DE.word,ram->getByte(HL.word));
	DE.word++;HL.word++;
	BC.word--;
	if(BC.word!=0)
		PC.word-=2;
	tstates+=7;
	resetH();
	setPV(BC.word!=0);
	resetN();
}

void Z80::ldd(uint8_t opcode){
	SUPERDEBUG(endl);
	ram->setByte(DE.word,ram->getByte(HL.word));
	DE.word--;HL.word--;
	BC.word--;
	tstates+=2;
	resetH();
	setPV(BC.word!=0);
	resetN();
}

void Z80::lddr(uint8_t opcode){
	SUPERDEBUG(endl);
	ram->setByte(DE.word,ram->getByte(HL.word));
	DE.word--;HL.word--;
	BC.word--;
	if(BC.word!=0)
		PC.word-=2;
	tstates+=7;
	resetH();
	resetPV();
	resetN();
}

void Z80::cpi(uint8_t opcode){
	SUPERDEBUG("(`cpi`)."<<endl);
	SUPERDEBUG(endl);
	cp(ram->getByte(HL.word++),false);
	setPV((BC.word--)!=0);
	tstates+=5;
}

void Z80::cpir(uint8_t opcode){
	SUPERDEBUG("(`cpir`)."<<endl);
	uint8_t v=ram->getByte(HL.word++);
	cp(v,false);
	setPV((BC.word--)!=0);
	tstates+=5;
	if(BC.word!=0 && !(AF.B.l&0x40)){
		PC.word-=2;
		tstates+=5;
	}
}

void Z80::cpd(uint8_t opcode){
	SUPERDEBUG("(`cpd`)."<<endl);
	SUPERDEBUG(endl);
	cp(ram->getByte(HL.word--),false);
	setPV((BC.word--)!=0);
	tstates+=5;
}

void Z80::cpdr(uint8_t opcode){
	SUPERDEBUG("(`cpdr`)."<<endl);
	SUPERDEBUG(endl);
	uint8_t v=ram->getByte(HL.word--);
	cp(v,false);
	setPV((BC.word--)!=0);
	tstates+=5;
	if(BC.word!=0 && !(AF.B.l&0x40)){
		PC.word-=2;
		tstates+=5;
	}
}

//8-bit arithmetic group

void Z80::addAR(uint8_t opcode){
	SUPERDEBUG(endl);
	addA(*((*regs)[opcode&0x07]));
}

void Z80::addAN(uint8_t opcode){
	SUPERDEBUG(endl);
	addA(ram->getByte(PC.word++));
}

void Z80::addA_HL_(uint8_t opcode){
	SUPERDEBUG(endl);
	addA(ram->getByte(HL.word));
}

void Z80::addA_IXd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=(int8_t)ram->getByte(PC.word++);
	addA(ram->getByte(IX.word+d));
}

void Z80::addA_IYd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=(int8_t)ram->getByte(PC.word++);
	addA(ram->getByte(IY.word+d));
}

void Z80::adcAR(uint8_t opcode){
	SUPERDEBUG(endl);
	addA(*((*regs)[opcode&0x07])+(AF.B.l&0x01));
}

void Z80::adcAN(uint8_t opcode){
	SUPERDEBUG(endl);
	addA(ram->getByte(PC.word++)+(AF.B.l&0x01));
}

void Z80::adcA_HL_(uint8_t opcode){
	SUPERDEBUG(endl);
	addA(ram->getByte(HL.word)+(AF.B.l&0x01));
}

void Z80::adcA_IXd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=(int8_t)ram->getByte(PC.word++);
	addA(ram->getByte(IX.word+d)+(AF.B.l&0x01));
}

void Z80::adcA_IYd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=(int8_t)ram->getByte(PC.word++);
	addA(ram->getByte(IY.word+d)+(AF.B.l&0x01));
}


void Z80::subR(uint8_t opcode){
	SUPERDEBUG("(`sub r`). Subtracting "<<(int)*((*regs)[opcode&0x07])<<" from register A."<<endl);
	subA(*((*regs)[opcode&0x07]));
}

void Z80::subN(uint8_t opcode){
	uint8_t N=ram->getByte(PC.word++);
	SUPERDEBUG("(`sub N`). Subtracting "<<N<<" from register A."<<endl);
	subA(N);
}

void Z80::sub_HL_(uint8_t opcode){
	uint8_t _HL_=ram->getByte(HL.word);
	SUPERDEBUG("(`sub (HL)`). Subtracting "<<_HL_<<" from register A."<<endl);
	subA(_HL_);
}

void Z80::sub_IXd_(uint8_t opcode){
	int8_t d=(int8_t)ram->getByte(PC.word++);
	uint8_t _IXd_=ram->getByte(IX.word+d);
	SUPERDEBUG("(`sub (IX+d)`). Subtracting "<<_IXd_<<" from register A."<<endl);
	subA(_IXd_);
}

void Z80::sub_IYd_(uint8_t opcode){
	int8_t d=(int8_t)ram->getByte(PC.word++);
	uint8_t _IYd_=ram->getByte(IY.word+d);
	SUPERDEBUG("(`sub (IY+d)`). Subtracting "<<_IYd_<<" from register A."<<endl);
	subA(_IYd_);
}

void Z80::sbcAR(uint8_t opcode){
	SUPERDEBUG("(`sbc r`). Subtracting "<<(int)(*((*regs)[opcode&0x07])+(AF.B.l&0x01))<<" from register A."<<endl);
	subA(*((*regs)[opcode&0x07])+(AF.B.l&0x01));
}

void Z80::sbcAN(uint8_t opcode){
	uint8_t N=ram->getByte(PC.word++);
	SUPERDEBUG("(`sbc N`). Subtracting "<<N+(AF.B.l&0x01)<<" from register A."<<endl);
	subA(N);
}

void Z80::sbcA_HL_(uint8_t opcode){
	uint8_t _HL_=ram->getByte(HL.word);
	SUPERDEBUG("(`sbc (HL)`). Subtracting "<<_HL_+(AF.B.l&0x01)<<" from register A."<<endl);
	subA(_HL_+(AF.B.l&0x01));
}

void Z80::sbcA_IXd_(uint8_t opcode){
	int8_t d=(int8_t)ram->getByte(PC.word++);
	uint8_t _IXd_=ram->getByte(IX.word+d);
	SUPERDEBUG("(`sbc (IX+d)`). Subtracting "<<_IXd_+(AF.B.l&0x01)<<" from register A."<<endl);
	subA(_IXd_+(AF.B.l&0x01));
}

void Z80::sbcA_IYd_(uint8_t opcode){
	int8_t d=(int8_t)ram->getByte(PC.word++);
	uint8_t _IYd_=ram->getByte(IY.word+d);
	SUPERDEBUG("(`sbc (IY+d)`). Subtracting "<<_IYd_+(AF.B.l&0x01)<<" from register A."<<endl);
	subA(_IYd_+(AF.B.l&0x01));
}

void Z80::andR(uint8_t opcode){
	SUPERDEBUG(endl);
	andA(*((*regs)[opcode&0x07]));
}

void Z80::andN(uint8_t opcode){
	SUPERDEBUG(endl);
	andA(ram->getByte(PC.word++));
}

void Z80::and_HL_(uint8_t opcode){
	SUPERDEBUG(endl);
	andA(ram->getByte(HL.word));
}

void Z80::and_IXd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=ram->getByte(PC.word++);
	andA(ram->getByte(IX.word+d));
}

void Z80::and_IYd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=ram->getByte(PC.word++);
	andA(ram->getByte(IY.word+d));
}

void Z80::orR(uint8_t opcode){
	SUPERDEBUG(endl);
	orA(*((*regs)[opcode&0x07]));
}

void Z80::orN(uint8_t opcode){
	SUPERDEBUG(endl);
	orA(ram->getByte(PC.word++));
}

void Z80::or_HL_(uint8_t opcode){
	SUPERDEBUG(endl);
	orA(ram->getByte(HL.word));
}

void Z80::or_IXd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=ram->getByte(PC.word++);
	orA(ram->getByte(IX.word+d));
}

void Z80::or_IYd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=ram->getByte(PC.word++);
	orA(ram->getByte(IY.word+d));
}

void Z80::xorR(uint8_t opcode){
	SUPERDEBUG(endl);
	xorA(*((*regs)[opcode&0x07]));
}

void Z80::xorN(uint8_t opcode){
	SUPERDEBUG(endl);
	xorA(ram->getByte(PC.word++));
}

void Z80::xor_HL_(uint8_t opcode){
	SUPERDEBUG(endl);
	xorA(ram->getByte(HL.word));
}

void Z80::xor_IXd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=ram->getByte(PC.word++);
	xorA(ram->getByte(IX.word+d));
}

void Z80::xor_IYd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=ram->getByte(PC.word++);
	xorA(ram->getByte(IY.word+d));
}

void Z80::cpR(uint8_t opcode){
	SUPERDEBUG(endl);
	cp(*((*regs)[opcode&0x07]),true);
}

void Z80::cpN(uint8_t opcode){
	uint8_t N=ram->getByte(PC.word++);
	SUPERDEBUG("(`cp N`). Comparing to "<<(int)N<<"..."<<endl);
	cp(N,true);
}

void Z80::cp_HL_(uint8_t opcode){
	SUPERDEBUG(endl);
	uint8_t _HL_=ram->getByte(HL.word);
	cp(_HL_,true);
}

void Z80::cp_IXd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=ram->getByte(PC.word++);
	uint8_t _IXd_=ram->getByte(IX.word+d);
	cp(_IXd_,true);
}

void Z80::cp_IYd_(uint8_t opcode){
	SUPERDEBUG(endl);
	int8_t d=ram->getByte(PC.word++);
	uint8_t _IYd_=ram->getByte(IY.word+d);
	cp(_IYd_,true);
}

void Z80::incR(uint8_t opcode){
	SUPERDEBUG("(`inc r`). Incrementing register "<<((opcode&0x38)>>3)<<"."<<endl);
	uint8_t old_val=*((*regs)[(opcode&0x38)>>3]);
	uint8_t new_val=old_val+1;
	*((*regs)[(opcode&0x38)>>3])=new_val;
	setS((new_val&0x80)==0x80);
	setZ(new_val==0);
	//if bit 3 was set and isn't now, that means there was a carry from bit 3, so set H, else reset H
	setH((old_val&0x08)==0x08 && (new_val&0x08)==0);
	setPV(old_val==0x7F);
	resetN();
}

void Z80::inc_HL_(uint8_t opcode){
	SUPERDEBUG(endl);
	tstates++;
	uint8_t old_val=ram->getByte(HL.word);
	uint8_t new_val=old_val+1;
	ram->setByte(HL.word,new_val);
	setS((new_val&0x80)==0x80);
	setZ(new_val==0);
	//if bit 3 was set and isn't now, that means there was a carry from bit 3, so set H, else reset H
	setH((old_val&0x08)==0x08 && (new_val&0x08)==0);
	setPV(old_val==0x7F);
	resetN();
}

void Z80::inc_IXd_(uint8_t opcode){
	SUPERDEBUG(endl);
	tstates+=6;
	int8_t d=ram->getByte(PC.word++);
	uint8_t old_val=ram->getByte(IX.word+d);
	uint8_t new_val=old_val+1;
	ram->setByte(IX.word+d,new_val);
	setS((new_val&0x80)==0x80);
	setZ(new_val==0);
	//if bit 3 was set and isn't now, that means there was a carry from bit 3, so set H, else reset H
	setH((old_val&0x08)==0x08 && (new_val&0x08)==0);
	setPV(old_val==0x7F);
	resetN();
}

void Z80::inc_IYd_(uint8_t opcode){
	SUPERDEBUG(endl);
	tstates+=6;
	int8_t d=ram->getByte(PC.word++);
	uint8_t old_val=ram->getByte(IY.word+d);
	uint8_t new_val=old_val+1;
	ram->setByte(IY.word+d,new_val);
	setS((new_val&0x80)==0x80);
	setZ(new_val==0);
	//if bit 3 was set and isn't now, that means there was a carry from bit 3, so set H, else reset H
	setH((old_val&0x08)==0x08 && (new_val&0x08)==0);
	setPV(old_val==0x7F);
	resetN();
}

void Z80::decR(uint8_t opcode){
	SUPERDEBUG("(`dec r`). Decrementing register "<<((opcode&0x38)>>3)<<"."<<endl);
	uint8_t old_val=*((*regs)[(opcode&0x38)>>3]);
	uint8_t new_val=old_val-1;
	*((*regs)[(opcode&0x38)>>3])=new_val;
	setS((new_val&0x80)==0x80);
	setZ(new_val==0);
	//if bit 3 was set and isn't now, that means there was a carry from bit 3, so set H, else reset H
	setH((old_val&0x08)==0x08 && (new_val&0x08)==0);
	setPV(old_val==0x7F);
	setN();
}

void Z80::dec_HL_(uint8_t opcode){
	SUPERDEBUG(endl);
	tstates++;
	uint8_t old_val=ram->getByte(HL.word);
	uint8_t new_val=old_val-1;
	ram->setByte(HL.word,new_val);
	setS((new_val&0x80)==0x80);
	setZ(new_val==0);
	//if bit 3 was set and isn't now, that means there was a carry from bit 3, so set H, else reset H
	//...though H should also be set if bit 3 wasn't set but is now, so that needs to be fixed...
	setH((old_val&0x08)==0x08 && (new_val&0x08)==0);
	setPV(old_val==0x7F);
	setN();
}

void Z80::dec_IXd_(uint8_t opcode){
	SUPERDEBUG(endl);
	tstates+=6;
	int8_t d=ram->getByte(PC.word++);
	uint8_t old_val=ram->getByte(IX.word+d);
	uint8_t new_val=old_val-1;
	ram->setByte(IX.word+d,new_val);
	setS((new_val&0x80)==0x80);
	setZ(new_val==0);
	//if bit 3 was set and isn't now, that means there was a carry from bit 3, so set H, else reset H
	setH((old_val&0x08)==0x08 && (new_val&0x08)==0);
	setPV(old_val==0x7F);
	setN();
}

void Z80::dec_IYd_(uint8_t opcode){
	SUPERDEBUG(endl);
	tstates+=6;
	int8_t d=ram->getByte(PC.word++);
	uint8_t old_val=ram->getByte(IY.word+d);
	uint8_t new_val=old_val-1;
	ram->setByte(IY.word+d,new_val);
	setS((new_val&0x80)==0x80);
	setZ(new_val==0);
	//if bit 3 was set and isn't now, that means there was a carry from bit 3, so set H, else reset H
	setH((old_val&0x08)==0x08 && (new_val&0x08)==0);
	setPV(old_val==0x7F);
	setN();
}

//General-purpose arithmetic group

void Z80::daa(uint8_t opcode){
	SUPERDEBUG(endl);
	uint8_t old=AF.B.h;
	uint8_t v=0;
	if((AF.B.h&0xF)>9 || getH())
		v+=0x06;
	if(((AF.B.h+v)>>4)>9 || getC() || (((uint16_t)(AF.B.h+v))&0x100)==0x100)
		v+=0x60;
	if(getN()){
		AF.B.h-=v;
		setH((((old & 0xf) - (v & 0xf)) & 0x10)==0x10);
	}
	else{
		AF.B.h+=v;
		setH((((old & 0xf)+(v & 0xf)) & 0x10)==0x10);
	}
	setS((AF.B.h&0x80)==0x80);
	setZ(AF.B.h==0);
	setPV(!parity(AF.B.h));
	setC(v>=0x60);
}

void Z80::cpl(uint8_t opcode){
	SUPERDEBUG(endl);
	AF.B.h ^= 0xFF;
	setH();
	setN();
}

void Z80::neg(uint8_t opcode){
	SUPERDEBUG(endl);
	uint8_t old=AF.B.h;
	AF.B.h=-old;
	setS((AF.B.h&0x80)==0x80);
	setZ(AF.B.h==0);
	setPV(old==0x80);
	setN();
	setC(old!=0);
}

void Z80::ccf(uint8_t opcode){
	SUPERDEBUG(endl);
	setH(getC());
	setC(!getC());
	resetN();
}

void Z80::scf(uint8_t opcode){
	SUPERDEBUG(endl);
	resetH();
	resetN();
	setC();
}


//CPU control group

void Z80::nop(uint8_t opcode){
	if(halted)
		return;
	SUPERDEBUG("(nop). Opcode: "<<(int)opcode<<endl);
}

void Z80::halt(uint8_t opcode){
	SUPERDEBUG(endl);
	halted=true;
}

void Z80::di(uint8_t opcode){
	IFF1=IFF2=false;
}

void Z80::ei(uint8_t opcode){
	IFF1=IFF2=true;
}

void Z80::im(uint8_t opcode){
	if((opcode&0x18)==0x10)
		mode=IM1;
	else if((opcode&0x18)==0x18)
		mode=IM2;
	else
		mode=IM0;
}

//16-bit arithmetic group

void Z80::addHLSS(uint8_t opcode){
	uint16_t SS=*((*regsDD)[(opcode&0x30)>>4]);
	uint32_t result=SS+HL.word;
	setH((((SS&0x0FFF)+(HL.word&0x0FFF))&0x1000)==0x1000);
	resetN();
	setC((result&0x10000)==0x10000);
	HL.word+=SS;
	tstates+=7;
}

void Z80::adcHLSS(uint8_t opcode){
	uint16_t SSC=*((*regsDD)[(opcode&0x30)>>4])+getC()?1:0;
	uint32_t result=SSC+HL.word;
	uint16_t r=result&0xFFFF;
	setS((r&0x8000)==0x8000);
	setZ(r==0);
	setH((((SSC&0x0FFF)+(HL.word&0x0FFF))&0x1000)==0x1000);
	setPV(((SSC&0x8000)==(HL.word&0x8000))&&((SSC&0x8000)!=(r&0x8000)));
	resetN();
	setC((result&0x10000)==0x10000);
	tstates+=7;
	HL.word=r;
}

void Z80::sbcHLSS(uint8_t opcode){
	uint16_t SSC=*((*regsDD)[(opcode&0x30)>>4])+getC()?1:0;
	uint32_t result=SSC-HL.word;
	uint16_t r=result&0xFFFF;
	setS((r&0x8000)==0x8000);
	setZ(r==0);
	setH((((SSC&0x0FFF)-(HL.word&0x0FFF))&0x1000)==0x1000);
	setPV(((SSC&0x8000)==(HL.word&0x8000))&&((SSC&0x8000)!=(r&0x8000)));
	setN();
	setC((result&0x10000)==0x10000);
	tstates+=7;
	HL.word=r;
}

void Z80::addIXPP(uint8_t opcode){
	(*regsDD)[2]=&IX.word;
	uint16_t SS=*((*regsDD)[(opcode&0x30)>>4]);
	uint32_t result=SS+IX.word;
	setH((((SS&0x0FFF)+(IX.word&0x0FFF))&0x1000)==0x1000);
	resetN();
	setC((result&0x10000)==0x10000);
	IX.word+=SS;
	tstates+=7;
	(*regsDD)[2]=&HL.word;
}

void Z80::addIYRR(uint8_t opcode){
	(*regsDD)[2]=&IY.word;
	uint16_t SS=*((*regsDD)[(opcode&0x30)>>4]);
	uint32_t result=SS+IY.word;
	setH((((SS&0x0FFF)+(IY.word&0x0FFF))&0x1000)==0x1000);
	resetN();
	setC((result&0x10000)==0x10000);
	IY.word+=SS;
	tstates+=7;
	(*regsDD)[2]=&HL.word;
}

void Z80::incSS(uint8_t opcode){
	SUPERDEBUG("(`inc ss`). Incrementing register "<<((opcode&0x30)>>4)<<" which currently contains "<<*((*regsDD)[(opcode&0x30)>>4])<<"."<<endl);
	(*((*regsDD)[(opcode&0x30)>>4]))++;
	tstates+=2;
}

void Z80::incIX(uint8_t opcode){
	IX.word++;
	tstates+=2;
}

void Z80::incIY(uint8_t opcode){
	IY.word++;
	tstates+=2;
}

void Z80::decSS(uint8_t opcode){
	(*((*regsDD)[(opcode&0x30)>>4]))--;
	tstates+=2;
}

void Z80::decIX(uint8_t opcode){
	IX.word--;
	tstates+=2;
}

void Z80::decIY(uint8_t opcode){
	IY.word--;
	tstates+=2;
}

//rotate/shift group

void Z80::rlca(uint8_t opcode){
	uint16_t t=AF.B.h<<1;
	setC(t&0x100);
	resetH();
	resetN();
	if(t&0x100)
		t|=0x01;
	AF.B.h=t&0xFF;
}

void Z80::rla(uint8_t opcode){
	uint16_t t=AF.B.h<<1;
	if(getC())
		t|=0x01;
	setC(t&0x100);
	AF.B.h=t&0xFF;
	resetH();
	resetN();
}

void Z80::rrca(uint8_t opcode){
	uint16_t t=AF.B.h>>1;
	setC(AF.B.h&0x01);
	if(getC())
		t|=0x80;
	AF.B.h=t&0xFF;
	resetH();
	resetN();
}

void Z80::rra(uint8_t opcode){
	resetH();
	resetN();
	uint16_t t=AF.B.h>>1;
	if(getC())
		t|=0x80;
	setC(AF.B.h&0x01);
	AF.B.h=t&0xFF;
}

void Z80::rlcR(uint8_t opcode){
	uint8_t * r=(*regs)[opcode&0x07];
	uint16_t v=(*r)<<1;;
	setC(v&0x100);
	if(v&0x100)
		v|=0x01;
	*r=v&0xFF;
	setS(((*r)&0x80)==0x80);
	setZ((*r)==0);
	resetH();
	setPV(!parity(*r));
	resetN();
}

void Z80::rlc_HL_(uint8_t opcode){
	uint8_t _hl_=ram->getByte(HL.word);
	uint16_t v=_hl_<<1;
	setC(v&0x100);
	if(v&0x100)
		v|=0x01;
	ram->setByte(HL.word,v&0xFF);
	uint8_t r=v&0xFF;
	setS(((r)&0x80)==0x80);
	setZ((r)==0);
	resetH();
	setPV(!parity(r));
	resetN();
}

void Z80::rlc_IXd_(uint8_t opcode){
	uint16_t address=IX.word;
	address+=((int8_t)ram->getByte(PC.word++));
	uint8_t _IXd_=ram->getByte(address);
	uint16_t v=_IXd_<<1;
	setC(v&0x100);
	if(v&0x100)
		v|=0x01;
	ram->setByte(address,v&0xFF);
}

void Z80::rlc_IYd_(uint8_t opcode){
	uint16_t address=IY.word;
	address+=((int8_t)ram->getByte(PC.word++));
	uint8_t _IYd_=ram->getByte(address);
	uint16_t v=_IYd_<<1;
	setC(v&0x100);
	if(v&0x100)
		v|=0x01;
	ram->setByte(address,v&0xFF);
}

void Z80::rlR(uint8_t opcode){

}

void Z80::rl_HL_(uint8_t opcode){

}

void Z80::rl_IXd_(uint8_t opcode){

}

void Z80::rl_IYd_(uint8_t opcode){

}

void Z80::rrcR(uint8_t opcode){

}

void Z80::rrc_HL_(uint8_t opcode){

}

void Z80::rrc_IXd_(uint8_t opcode){

}

void Z80::rrc_IYd_(uint8_t opcode){

}

void Z80::rrR(uint8_t opcode){

}

void Z80::rr_HL_(uint8_t opcode){

}

void Z80::rr_IXd_(uint8_t opcode){

}

void Z80::rr_IYd_(uint8_t opcode){

}

void Z80::slaR(uint8_t opcode){

}

void Z80::sla_HL_(uint8_t opcode){

}

void Z80::sla_IXd_(uint8_t opcode){

}

void Z80::sla_IYd_(uint8_t opcode){

}

void Z80::sraR(uint8_t opcode){

}

void Z80::sra_HL_(uint8_t opcode){

}

void Z80::sra_IXd_(uint8_t opcode){

}

void Z80::sra_IYd_(uint8_t opcode){

}

void Z80::srlR(uint8_t opcode){

}

void Z80::srl_HL_(uint8_t opcode){

}

void Z80::srl_IXd_(uint8_t opcode){

}

void Z80::srl_IYd_(uint8_t opcode){

}

void Z80::rld(uint8_t opcode){

}

void Z80::rrd(uint8_t opcode){

}

//jump group

void Z80::jpNN(uint8_t opcode){
	uint16_t address=ram->getWord(PC.word);
	SUPERDEBUG("(jp NN). Jumping to address "<<address);
	PC.word=address;
}

void Z80::jpCCNN(uint8_t opcode){
	SUPERDEBUG("(`jp cc, nn`).");
	uint16_t address=ram->getWord(PC.word);
	PC.word+=2;
	uint8_t c=(opcode&0x38)>>3;
	SUPERDEBUG("Condition: "<<(int)c<<".");
	if(
		(c==0&&!getZ())
		|| (c==1 && getZ())
		|| (c==2 && !getC())
		|| (c==3 && getC())
		|| (c==4 && !getPV())
		|| (c==5 && getPV())
		|| (c==6 && !getS())
		|| (c==7 && getS())
	){
		PC.word=address;
		SUPERDEBUG("Jumping to "<<address<<"...");
	}
	SUPERDEBUG(endl);
}

void Z80::djnzE(uint8_t opcode){
	tstates++;
	int8_t displacement=ram->getByte(PC.word++);
	if(--BC.B.h!=0){
		PC.word+=displacement;
		tstates+=5;
	}
}

//call and return group

void Z80::callNN(uint8_t opcode){
	SUPERDEBUG(endl);
	uint16_t address=ram->getWord(PC.word);
	PC.word+=2;
	push(PC.word);
	PC.word=address;
	tstates++;
}

void Z80::callCCNN(uint8_t opcode){
	SUPERDEBUG(endl);
	uint16_t address=ram->getWord(PC.word);
	PC.word+=2;
	uint8_t c=(opcode&0x38)>>3;
	if(
		(c==0&&(AF.B.l&0x40)!=0x40)
		|| (c==1&&((AF.B.l&0x40)==0x40))
		|| (c==2&&((AF.B.l&0x01)!=0x01))
		|| (c==3&&((AF.B.l&0x01)==0x01))
		|| (c==4&&((AF.B.l&0x04)!=0x04))
		|| (c==5&&((AF.B.l&0x04)==0x04))
		|| (c==6&&((AF.B.l&0x80)!=0x80))
		|| (c==7&&((AF.B.l&0x80)==0x80))
	){
		push(PC.word);
		PC.word=address;
		tstates++;
	}
}

void Z80::ret(uint8_t opcode){
	SUPERDEBUG(endl);
	PC.word=pop();
}

void Z80::retCC(uint8_t opcode){
	SUPERDEBUG(endl);
	tstates++;
	uint8_t c=(opcode&0x38)>>3;
	if(
		(c==0&&(AF.B.l&0x40)!=0x40)
		|| (c==1&&((AF.B.l&0x40)==0x40))
		|| (c==2&&((AF.B.l&0x01)!=0x01))
		|| (c==3&&((AF.B.l&0x01)==0x01))
		|| (c==4&&((AF.B.l&0x04)!=0x04))
		|| (c==5&&((AF.B.l&0x04)==0x04))
		|| (c==6&&((AF.B.l&0x80)!=0x80))
		|| (c==7&&((AF.B.l&0x80)==0x80))
	){
		PC.word=pop();
	}
}

//input and output group

void Z80::out_N_A(uint8_t opcode){
	SUPERDEBUG(endl);
	io->out(ram->getByte(PC.word++),AF.B.h);
}

//convenience functions

inline void Z80::push(uint16_t value){
	SP.word-=2;
	ram->setWord(SP.word,value);
}

inline uint16_t Z80::pop(){
	uint16_t v=ram->getWord(SP.word);
	SP.word+=2;
	return v;
}

inline void Z80::cp(uint8_t value,bool b){
	uint16_t result=AF.B.h-value;
	uint8_t r=result&0x00FF;
	setS(r&0x80==0x80);
	setZ(r==0);
	setN();
	setH((((AF.B.h&0x0F)-(value&0x0F))&0x10)==0x10);
	if(b){
		setPV((AF.B.h&0x80)!=(value&0x80)&&(AF.B.h&0x80)!=(r&0x80));
		setC((result&0x0100)==0x0100);
	}
}

inline void Z80::addA(uint8_t value){
	uint16_t result=AF.B.h+value;
	uint8_t r=result&0x00FF;
	setS((r&0x80)==0x80);
	setZ(r==0);
	setH((((AF.B.h&0x0F)+(value&0x0F))&0x10)==0x10);
	setPV((AF.B.h&0x80)==(value&0x80)&&(value&0x80)!=(r&0x80));
	resetN();
	setC((result&0x0100)==0x0100);
	AF.B.h=r;
}

inline void Z80::subA(uint8_t value){
	uint16_t result=AF.B.h-value;
	uint8_t r=result&0x00FF;
	setS((r&0x80)==0x80);
	setZ(r==0);
	setH((((AF.B.h&0x0F)-(value&0x0F))&0x10)==0x10);
	setPV((AF.B.h&0x80)==(value&0x80)&&(value&0x80)!=(r&0x80));
	setN();
	setC((result&0x0100)==0x0100);
	AF.B.h=r;
}

inline void Z80::andA(uint8_t value){
	uint8_t result=AF.B.h&value;
	setS((result&0x80)==0x80);
	setZ(result==0);
	setH();
	setPV(!parity(result));
	resetN();
	resetC();
}

inline void Z80::orA(uint8_t value){
	uint8_t result=AF.B.h|value;
	setS((result&0x80)==0x80);
	setZ(result==0);
	resetH();
	setPV(!parity(result));
	resetN();
	resetC();
}

inline void Z80::xorA(uint8_t value){
	uint8_t result=AF.B.h ^ value;
	setS((result&0x80)==0x80);
	setZ(result==0);
	resetH();
	setPV(!parity(result));
	resetN();
	resetC();
}

inline int Z80::parity(uint8_t x){
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;
	return x & 1;
}
