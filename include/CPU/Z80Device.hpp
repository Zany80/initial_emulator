#ifndef _ZENITH80_CPU_Z80DEVICE_HPP_
#define _ZENITH80_CPU_Z80DEVICE_HPP_

#include <zenith80.hpp>
#include <defines.hpp>

ZENITH_HEADER

class Z80Device{
	public:
		virtual void out(uint8_t value)=0;
		virtual uint8_t in()=0;
};

ZENITH_FOOTER

#endif
