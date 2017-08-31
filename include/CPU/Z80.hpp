#ifndef _ZENITH80_CPU_Z80_HPP_
#define _ZENITH80_CPU_Z80_HPP_

#include <zenith80.hpp>

#include "Z80Memory.hpp"
#include "Z80Device.hpp"

#include <defines.hpp>
#include <vector>
using std::vector;

ZENITH_HEADER

class Z80{
	public:
		typedef void (Z80::*opcode)(uint8_t);
		Z80(Z80Memory * ram,Z80Device * io);
		void executeOneInstruction();
		void executeXInstructions(int x);
		word getAF();
		word getAF2();
		word getBC();
		word getBC2();
		word getDE();
		word getDE2();
		word getHL();
		word getHL2();
		word getIX();
		word getIY();
		word getSP();
		word getPC();
		uint8_t getR();
		uint8_t getI();
	private:
		Z80Memory * ram;
		Z80Device * io;
		word AF;
		word AF2;
		word BC;
		word BC2;
		word DE;
		word DE2;
		word HL;
		word HL2;
		word IX;
		word IY;
		word SP;
		word PC;
		uint8_t R;
		uint8_t I;
		vector<uint8_t*>* regs;
		vector<uint16_t*>* regsDD;
		vector<uint16_t*>* regsQQ;
		bool IFF1,IFF2;
	private:
		opcode * opcodes;
		#include <CPU/opcodes.hpp>
		void setC();
		void setS();
		void setZ();
		void setN();
		void setH();
		void setPV();
		void setC(bool b);
		void setS(bool b);
		void setZ(bool b);
		void setN(bool b);
		void setH(bool b);
		void setPV(bool b);
		void resetC();
		void resetS();
		void resetZ();
		void resetN();
		void resetH();
		void resetPV();

};

ZENITH_FOOTER

#endif
