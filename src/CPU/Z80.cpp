#include <CPU/Z80.hpp>
#include <zenith80.hpp>

#include <cstdlib>

#include <iostream>
using std::cout;using std::endl;

ZENITH_HEADER

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
}

void Z80::executeXInstructions(int x){
	for(int i=0;i<x;i++)
		executeOneInstruction();
}

void Z80::executeOneInstruction(){
	uint8_t opcode=ram->getByte(PC.word++);
	//ld r, r'
	if((opcode & 0xC0)==0x40)
		*((*regs)[(opcode & 0x38)>>3])=*((*regs)[opcode & 0x07]);
	//ld r, n
	else if((opcode & 0xC7)==0x06)
		*((*regs)[(opcode & 0x38)>>3])=ram->getByte(PC.word++);
	//ld r, (hl)
	else if((opcode & 0xC7)==0x46)
		*((*regs)[(opcode & 0x38)>>3])=ram->getByte(HL.word);
	//extended operations
	else if((opcode==0xDD)||(opcode==0xFD)||(opcode==0xED))
		extended_operations(opcode);
	//ld (hl), r
	else if((opcode&0xF8)==0x70)
		ram->setByte(HL.word,*((*regs)[opcode&0x07]));
	//ld (hl), n
	else if(opcode==0x36)
		ram->setByte(HL.word,ram->getByte(PC.word++));
	//ld a, (bc)
	else if(opcode==0x0A)
		AF.B.h=ram->getByte(BC.word);
	//ld a, (de)
	else if(opcode==0x1A)
		AF.B.h=ram->getByte(DE.word);
	//ld a, (nn)
	else if(opcode==0x3A){
		AF.B.h=ram->getByte(ram->getWord(PC.word));
		PC.word+=2;
	}
	//ld (bc), a
	else if(opcode==0x02)
		ram->setByte(BC.word,AF.B.h);
	//ld (de), a
	else if(opcode==0x12)
		ram->setByte(DE.word,AF.B.h);
	//ld (nn), a
	else if(opcode==0x32){
		ram->setByte(ram->getWord(PC.word),AF.B.h);
		PC.word+=2;
	}
	//ld dd, nn
	else if((opcode&0xCF)==0x01){
		*((*regsDD)[(opcode&0x30)>>4])=ram->getWord(PC.word);
		PC.word+=2;
	}
	//ld hl, (nn)
	else if(opcode==0x2A){
		HL.word=ram->getWord(ram->getWord(PC.word));
		PC.word+=2;
	}
	//ld (nn), hl
	else if(opcode==0x22){
		ram->setWord(ram->getWord(PC.word),HL.word);
		PC.word+=2;
	}
	//ld sp, hl
	else if(opcode==0xF9)
		SP.word=HL.word;
	//push qq
	else if((opcode&0xCF)==0xC5){
		SP.word-=2;
		cout<<"SP: "<<SP.word<<endl;
		cout<<"HL: "<<HL.word<<endl;
		cout<<"Pushing register "<<((opcode&0x30)>>4)<<endl;
		cout<<"Pushing "<<*((*regsQQ)[(opcode&0x30)>>4])<<endl;
		ram->setWord(SP.word,*((*regsQQ)[(opcode&0x30)>>4]));
	}
	//pop qq
	else if((opcode&0xCF)==0xC1){
		uint16_t val=ram->getWord(SP.word);
		//cout<<"SP: "<<SP.word<<endl;
		//cout<<"(SP-2): "<<ram->getWord(SP.word-2)<<endl;
		//cout<<"Value being popped: "<<val<<endl;
		//cout<<"Popping into register: "<<((opcode&0x30)>>4)<<endl;
		*((*regs)[(opcode&0x30)>>4])=val;
		SP.word+=2;
	}
	//ex de, hl
	else if(opcode==0xEB){
		uint16_t t=DE.word;
		DE.word=HL.word;
		HL.word=t;
	}
	//ex af, af'
	else if(opcode==0x08){
		uint16_t t=AF2.word;
		AF2.word=AF.word;
		AF.word=AF2.word;
	}
	//exx
	else if(opcode==0xD9){
		uint16_t x=BC.word,y=DE.word,z=HL.word;
		BC.word=BC2.word;DE.word=DE2.word;HL.word=HL2.word;
		BC2.word=x;DE2.word=y;HL2.word=z;
	}
	//ex (sp), hl
	else if(opcode==0xE3){
		uint16_t t=HL.word;
		HL.word=ram->getWord(SP.word);
		ram->setWord(SP.word,t);
	}
	//out (n), a
	else if(opcode==0xD3)
		io->out(ram->getByte(PC.word++),AF.B.h);
	//nop
	else if(opcode==0);

};

void Z80::extended_operations(uint8_t p_opcode){
	uint8_t opcode=ram->getByte(PC.word++);
	//IX instructions
	if(p_opcode==0xDD){
		//ld r, (ix+d)
		if((opcode&0xC7)==0x46)
			*((*regs)[(opcode&0x38)>>3])=ram->getByte(IX.word+ram->getByte(PC.word++));
		//ld (ix+d), r
		else if((opcode&0xF8)==0x70)
			ram->setByte(IX.word+ram->getByte(PC.word++),*((*regs)[opcode&0x07]));
		//ld (ix+d), n
		else if(opcode==0x36){
			uint8_t d=ram->getByte(PC.word++);
			ram->setByte(IX.word+d,ram->getByte(PC.word++));
		}
		//ld ix, nn
		else if(opcode==0x21){
			IX.word=ram->getWord(PC.word);
			PC.word+=2;
		}
		//ld ix, (nn)
		else if(opcode==0x2A){
			IX.word=ram->getWord(ram->getWord(PC.word));
			PC.word+=2;
		}
		//ld (nn), ix
		else if(opcode==0x22){
			ram->setWord(ram->getWord(PC.word),IX.word);
			PC.word+=2;
		}
		//ld sp, ix
		else if(opcode==0xF9)
			SP.word=IX.word;
		//push ix
		else if(opcode==0xE5){
			SP.word-=2;
			ram->setWord(SP.word,IX.word);
		}
		//pop ix
		else if(opcode==0xE1){
			IX.word=ram->getWord(SP.word);
			SP.word+=2;
		}
		//ex (sp), ix
		else if(opcode==0xE3){
			uint16_t t=IX.word;
			IX.word=ram->getWord(SP.word);
			ram->setWord(SP.word,t);
		}
	}
	//IY instructions
	else if(p_opcode==0xFD){
		//ld r, (iy+d)
		if((opcode&0xC7)==0x46)
			*((*regs)[(opcode & 0x38)>>3])=ram->getByte(IY.word+ram->getByte(PC.word++));
		//ld (iy+d), r
		else if((opcode&0xF8)==0x70)
			ram->setByte(IY.word+ram->getByte(PC.word++),*((*regs)[opcode&0x07]));
		//ld (iy+d), n
		else if(opcode==0x36){
			uint8_t d=ram->getByte(PC.word++);
			ram->setByte(IY.word+d,ram->getByte(PC.word++));
		}
		//ld iy, nn
		else if(opcode==0x21)
			IY.word=ram->getWord(PC.word+=2);
		//ld iy, (nn)
		else if(opcode==0x2A)
			IY.word=ram->getWord(ram->getWord(PC.word+=2));
		//ld (nn), iy
		else if(opcode==0x22)
			ram->setWord(ram->getWord(PC.word+=2),IY.word);
		//ld sp, iy
		else if(opcode==0xF9)
			SP.word=IY.word;
		//push iy
		else if(opcode==0xE5){
			SP.word-=2;
			ram->setWord(SP.word,IY.word);
		}
		//pop iy
		else if(opcode==0xE1){
			IY.word=ram->getWord(SP.word);
			SP.word+=2;
		}
		//ex (sp), iy
		else if(opcode==0xE3){
			uint16_t t=IY.word;
			IY.word=ram->getWord(SP.word);
			ram->setWord(SP.word,t);
		}
	}
	//Extended instructions
	else if(p_opcode==0xED){
		//ld a, i
		if(opcode==0x57){
			AF.B.h=I;
			setS((I&0x80)==0x80);
			setZ(I==0);
			resetH();
			setPV(IFF2);
			resetN();
		}
		//ld a, r
		else if(opcode==0x5F){
			AF.B.h=R;
			setS((R&0x80)==0x80);
			setZ(R==0);
			resetH();
			setPV(IFF2);
			resetN();
		}
		//ld i, a
		else if(opcode==0x47)
			I=AF.B.h;
		//ld r, a
		else if(opcode==0x4F)
			R=AF.B.h;
		//ld dd, (nn)
		else if((opcode&0xCF)==0x4B){
			*((*regs)[(opcode&0x30)>>4])=ram->getWord(PC.word);
			PC.word+=2;
		}
		//ld (nn), dd
		else if((opcode&0xCF)==0x43){
			ram->setWord(ram->getWord(PC.word),*((*regs)[(opcode&0x30)>>4]));
			PC.word+=2;
		}
		//out (c), r
		else if((opcode&0xC7)==0x41){
			io->out(BC.B.l,*((*regs)[(opcode&0x38)>>3]));
			//cout<<"Value to write for out (c), r: register "<<((opcode&0x38)>>3)<<endl;
			//cout<<"Actual value: "<<(int)*((*regs)[(opcode&0x38)>>3])<<endl<<endl;
			//cout<<"HL: "<<HL.word<<endl;
			//cout<<"SP: "<<SP.word<<endl;
		}
		//ldi / ldd / ldir / lddr
		else if((opcode&0xE7)==0xA0){
			ram->setByte(DE.word,ram->getByte(HL.word));
			BC.word--;
			if(opcode&0x08){
				HL.word--;DE.word--;
			}
			else{
				HL.word++;DE.word++;
			}
			if(opcode&0x10 && BC.word!=0)
				PC.word-=2;
			resetH();
			resetN();
			if(opcode==0xB8)
				resetPV();
			else
				setPV(BC.word!=0);
		}
		//cpi
		else if(opcode==0xA1){

		}
	}
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

ZENITH_FOOTER
