#ifndef _ZENITH_DEVICE_CONTROLLER_HPP_
#define _ZENITH_DEVICE_CONTROLLER_HPP_

#include <zenith80.hpp>
#include <CPU/Z80Device.hpp>
#include <vector>
using std::vector;

ZENITH_HEADER

class DeviceController{
	public:
		DeviceController();
		void out(uint16_t port,uint8_t value);
		uint8_t in(uint16_t port);
	private:
		vector<Z80Device*> * devices;
};

ZENITH_FOOTER

#endif
