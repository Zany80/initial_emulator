#include <CPU/peripherals/GenericDevice.hpp>

ZENITH_HEADER

GenericDevice::GenericDevice(output_function o,input_function i){
	this->o=o;this->i=i;
}

void GenericDevice::out(uint8_t value){
	if(o!=nullptr)
		o(value);
}

uint8_t GenericDevice::in(){
	if(i!=nullptr)
		return i();
}

ZENITH_FOOTER
