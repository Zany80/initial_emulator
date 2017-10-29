#ifndef _ZENITH_RAM_CONTROLLER_HPP_
#define _ZENITH_RAM_CONTROLLER_HPP_

#include <zenith80.hpp>
#include <CPU/Z80Memory.hpp>

#include <string>
using std::string;

#include <vector>
using std::vector;

ZENITH_HEADER

class RAMController : public Z80Memory{
	public:
		RAMController(const char * name);
		uint8_t getOpcode(uint16_t address) override;
		uint8_t getByte(uint16_t address) override;
		void setByte(uint16_t address,uint8_t value) override;
		uint16_t getWord(uint16_t address) override;
		void setWord(uint16_t address,uint16_t value) override;
		void savePMem() override;
		void swapBanks(uint8_t from,uint8_t to) override;
	private:
		vector<uint8_t *> banks;
		uint8_t bank;
		string name;
};

ZENITH_FOOTER

#endif
