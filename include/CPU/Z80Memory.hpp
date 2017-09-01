#ifndef _ZENITH80_CPU_Z80MEMORY_HPP_
#define _ZENITH80_CPU_Z80MEMORY_HPP_

#include <zenith80.hpp>
#include <defines.hpp>

ZENITH_HEADER

class Z80Memory{
	public:
		virtual uint8_t getOpcode(uint16_t address)=0;
		virtual uint8_t getByte(uint16_t address)=0;
		virtual void setByte(uint16_t address,uint8_t value)=0;
		virtual uint16_t getWord(uint16_t address)=0;
		virtual void setWord(uint16_t address,uint16_t value)=0;
};

ZENITH_FOOTER

#endif
