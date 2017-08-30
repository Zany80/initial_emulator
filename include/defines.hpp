#ifndef _ZENITH80_DEFINES_HPP_
#define _ZENITH80_DEFINES_HPP_

#include <cstdint>

typedef union{
	uint16_t word;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	struct{
		uint8_t l,h;
	} B;
#else
	struct{
		uint8_t h,l;
	} B;
#endif
} word;

#endif
