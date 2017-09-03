//prefixes

void Z80::DD(uint8_t opcode){
	opcode=ram->getOpcode(PC.word++);
	(*this.*(opcodes_dd[opcode]))(opcode);
}

void Z80::FD(uint8_t opcode){
	opcode=ram->getOpcode(PC.word++);
	(*this.*(opcodes_fd[opcode]))(opcode);
}

void Z80::ED(uint8_t opcode){
	opcode=ram->getOpcode(PC.word++);
	(*this.*(opcodes_ed[opcode]))(opcode);
}

//8-bit load group

void Z80::nop(uint8_t opcode){
	//cout<<"Nop"<<endl;
}

void Z80::ldRRx(uint8_t opcode){
	*((*regs)[(opcode&0x38)>>3])=*((*regs)[opcode&0x07]);
}

void Z80::ldRN(uint8_t opcode){
	*((*regs)[(opcode&0x38)>>3])=ram->getByte(PC.word++);
}

void Z80::ldR_HL_(uint8_t opcode){
	*((*regs)[(opcode&0x38)>>3])=ram->getByte(HL.word);
}

void Z80::ldR_IXd_(uint8_t opcode){
	uint8_t * reg=(*regs)[(opcode&0x38)>>3];
	int8_t d=(int8_t)ram->getByte(PC.word++);
	tstates+=5;
	*reg=ram->getByte(IX.word+d);
}

void Z80::ldR_IYd_(uint8_t opcode){
	uint8_t * reg=(*regs)[(opcode&0x38)>>3];
	int8_t d=(int8_t)ram->getByte(PC.word++);
	tstates+=5;
	*reg=ram->getByte(IY.word+d);
}

void Z80::ld_HL_R(uint8_t opcode){
	ram->setByte(HL.word,*((*regs)[opcode&0x07]));
}

void Z80::ld_IXd_R(uint8_t opcode){
	uint8_t * reg=(*regs)[opcode&0x07];
	int8_t d=(int8_t)ram->getByte(PC.word++);
	tstates+=5;
	ram->setByte(IX.word+d,*reg);
}

void Z80::ld_IYd_R(uint8_t opcode){
	uint8_t * reg=(*regs)[opcode&0x07];
	int8_t d=(int8_t)ram->getByte(PC.word++);
	tstates+=5;
	ram->setByte(IY.word+d,*reg);
}

void Z80::ld_HL_N(uint8_t opcode){
	ram->setByte(HL.word,ram->getByte(PC.word++));
}

void Z80::ld_IXd_N(uint8_t opcode){
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
	tstates+=2;
	int8_t d=(int8_t)ram->getByte(PC.word++);
	ram->setByte(IY.word+d,ram->getByte(PC.word++));
}

void Z80::ldA_BC_(uint8_t opcode){
	AF.B.h=ram->getByte(BC.word);
}

void Z80::ldA_DE_(uint8_t opcode){
	AF.B.h=ram->getByte(DE.word);
}

void Z80::ldA_NN_(uint8_t opcode){
	AF.B.h=ram->getByte(ram->getWord(PC.word));
	PC.word+=2;
}

void Z80::ld_BC_A(uint8_t opcode){
	ram->setByte(BC.word,AF.B.h);
}

void Z80::ld_DE_A(uint8_t opcode){
	ram->setByte(DE.word,AF.B.h);
}

void Z80::ld_NN_A(uint8_t opcode){
	ram->setByte(ram->getWord(PC.word),AF.B.h);
	PC.word+=2;
}

void Z80::ldAI(uint8_t opcode){
	tstates+=5;
	AF.B.h=I;
	setS((I&0x80)==0x80);
	setZ(I==0);
	resetH();
	setPV(IFF2);
	resetN();
}

void Z80::ldAR(uint8_t opcode){
	tstates+=5;
	AF.B.h=R;
	setS((R&0x80)==0x80);
	setZ(R==0);
	resetH();
	setPV(IFF2);
	resetN();
}

void Z80::ldIA(uint8_t opcode){
	tstates+=5;
	I=AF.B.h;
}

void Z80::ldRA(uint8_t opcode){
	tstates+=5;
	R=AF.B.h;
}

//16-bit load group

void Z80::ldDDNN(uint8_t opcode){
	*((*regsDD)[(opcode&0x30)>>4])=ram->getWord(PC.word);
	PC.word+=2;
}

void Z80::ldIXNN(uint8_t opcode){
	IX.word=ram->getWord(PC.word);
	PC.word+=2;
}

void Z80::ldIYNN(uint8_t opcode){
	IY.word=ram->getWord(PC.word);
	PC.word+=2;
}

void Z80::ldHL_NN_(uint8_t opcode){
	HL.word=ram->getWord(ram->getWord(PC.word));
	PC.word+=2;
}

void Z80::ldDD_NN_(uint8_t opcode){
	*((*regsDD)[(opcode&0x30)>>4])=ram->getWord(ram->getWord(PC.word));
	PC.word+=2;
}

void Z80::ldIX_NN_(uint8_t opcode){
	IX.word=ram->getWord(ram->getWord(PC.word));
	PC.word+=2;
}

void Z80::ldIY_NN_(uint8_t opcode){
	IY.word=ram->getWord(ram->getWord(PC.word));
	PC.word+=2;
}

void Z80::ld_NN_HL(uint8_t opcode){
	ram->setWord(ram->getWord(PC.word),HL.word);
	HL.word+=2;
}

void Z80::ld_NN_DD(uint8_t opcode){
	ram->setWord(ram->getWord(PC.word),*((*regsDD)[(opcode&0x30)>>4]));
	PC.word+=2;
}

void Z80::ld_NN_IX(uint8_t opcode){
	ram->setWord(ram->getWord(PC.word),IX.word);
	PC.word+=2;
}

void Z80::ld_NN_IY(uint8_t opcode){
	ram->setWord(ram->getWord(PC.word),IY.word);
	PC.word+=2;
}

void Z80::ldSPHL(uint8_t opcode){
	//takes 6 tstates for some reason. Since 4 are needed for opcode fetch, add 2
	tstates+=2;
	SP.word=HL.word;
}

void Z80::ldSPIX(uint8_t opcode){
	//takes 6 tstates for some reason. Since 4 are needed for opcode fetch, add 2
	tstates+=2;
	SP.word=IX.word;
}

void Z80::ldSPIY(uint8_t opcode){
	//takes 6 tstates for some reason. Since 4 are needed for opcode fetch, add 2
	tstates+=2;
	SP.word=IY.word;
}

void Z80::pushQQ(uint8_t opcode){
	//takes 11 tstates. opcode fetch=4, push=ram->setWord=2x ram->setByte=6, add 1
	tstates++;
	push(*((*regsQQ)[(opcode&0x30)>>4]));
}

void Z80::pushIX(uint8_t opcode){
	//takes 15 tstates. 2x opcode fetch=8, push=ram->setWord=2x ram->setByte=6, add 1
	tstates++;
	push(IX.word);
}

void Z80::pushIY(uint8_t opcode){
	//takes 15 tstates. 2x opcode fetch=8, push=ram->setWord=2x ram->setByte=6, add 1
	tstates++;
	push(IY.word);
}

void Z80::popQQ(uint8_t opcode){
	*((*regsQQ)[(opcode&0x30)>>4])=pop();
}

void Z80::popIX(uint8_t opcode){
	IX.word=pop();
}

void Z80::popIY(uint8_t opcode){
	IY.word=pop();
}

//exchange, block transfer, and search group

void Z80::exDEHL(uint8_t opcode){
	uint16_t t=DE.word;
	DE.word=HL.word;
	HL.word=t;
}

void Z80::exAFAF2(uint8_t opcode){
	uint16_t t=AF.word;
	AF.word=AF2.word;
	AF2.word=t;
}

void Z80::exx(uint8_t opcode){
	uint16_t t1=BC.word,t2=DE.word,t3=HL.word;
	BC.word=BC2.word;DE.word=DE2.word;HL.word=HL2.word;
	HL2.word=t1;DE2.word=t2;HL2.word=t3;
}

void Z80::ex_SP_HL(uint8_t opcode){
	word t=HL;
	HL.word=ram->getWord(SP.word);
	ram->setWord(SP.word,t.word);
	tstates+=3;
}

void Z80::ex_SP_IX(uint8_t opcode){
	word t=IX;
	IX.word=ram->getWord(SP.word);
	ram->setWord(SP.word,t.word);
	tstates+=3;
}

void Z80::ex_SP_IY(uint8_t opcode){
	word t=IY;
	IY.word=ram->getWord(SP.word);
	ram->setWord(SP.word,t.word);
	tstates+=3;
}

void Z80::ldi(uint8_t opcode){
	ram->setByte(DE.word,ram->getByte(HL.word));
	DE.word++;HL.word++;
	BC.word--;
	tstates+=2;
	resetH();
	setPV(BC.word!=0);
	resetN();
}

void Z80::ldir(uint8_t opcode){
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
	ram->setByte(DE.word,ram->getByte(HL.word));
	DE.word--;HL.word--;
	BC.word--;
	tstates+=2;
	resetH();
	setPV(BC.word!=0);
	resetN();
}

void Z80::lddr(uint8_t opcode){
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
	cp(ram->getByte(HL.word++));
	setPV((BC.word--)!=0);
	tstates+=5;
}

void Z80::cpir(uint8_t opcode){
	uint8_t v=ram->getByte(HL.word++);
	cp(v);
	setPV((BC.word--)!=0);
	tstates+=5;
	if(BC.word!=0 && !(AF.B.l&0x40)){
		PC.word-=2;
		tstates+=5;
	}
}

void Z80::cpd(uint8_t opcode){
	cp(ram->getByte(HL.word--));
	setPV((BC.word--)!=0);
	tstates+=5;
}

void Z80::cpdr(uint8_t opcode){
	uint8_t v=ram->getByte(HL.word--);
	cp(v);
	setPV((BC.word--)!=0);
	tstates+=5;
	if(BC.word!=0 && !(AF.B.l&0x40)){
		PC.word-=2;
		tstates+=5;
	}
}

/*
 * regular CP:
 * setPV((AF.B.h&0x80)!=(value&0x80)&&(AF.B.h&0x80)!=(r&0x80));
 * setC(((AF.B.h-value)&0x100)==0x100);
 */

//input and output group

void Z80::out_N_A(uint8_t opcode){
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

inline void Z80::cp(uint8_t value){
	uint8_t r=AF.B.h-value;
	setS(r&0x80==0x80);
	setZ(r==0);
	setN();
	setH((((AF.B.h&0x0F)-(value&0x0F))&0x10)==0x10);
}
