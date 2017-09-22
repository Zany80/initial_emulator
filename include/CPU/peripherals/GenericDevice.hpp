#ifndef _ZENITH80_CPU_PERIPHERALS_GENERICDEVICE_HPP_
#define _ZENITH80_CPU_PERIPHERALS_GENERICDEVICE_HPP_

#include <zenith80.hpp>
#include <defines.hpp>
#include <CPU/Z80Device.hpp>

typedef void (*output_function)(uint8_t);
typedef uint8_t(*input_function)(void);

ZENITH_HEADER

class GenericDevice : public Z80Device{
	public:
		GenericDevice(output_function o,input_function i);
		void out(uint8_t value) override;
		uint8_t in() override;
	private:
		output_function o;
		input_function i;
};

ZENITH_FOOTER

#endif
