#ifndef _ZENITH80_MAIN_HPP_
#define _ZENITH80_MAIN_HPP_

#include "zenith80.hpp"
#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::Color;
using sf::Text;
using sf::Font;

#include <string>
using std::string;
#include <vector>
using std::vector;

#include <CPU/Z80.hpp>

#include <defines.hpp>

#include <TGUI/TGUI.hpp>
using tgui::Gui;
typedef tgui::ChatBox TermOut;

typedef const char * cstring;

ZENITH_HEADER

class Main{
	public:
		Main(int argc,char ** argv);
		~Main();
		static int main(int argc,char** argv);
		int run();
		static Main * instance;
		Z80 * cpu;
		bool cpm_emu;
		void putchar(char c);
		void putmsg(string s);
		uint8_t key();
		void resetKeyBuffer();
	private:
		vector<uint8_t> key_buffer;
		uint8_t _key;
		uint64_t unit;
		uint64_t clock_speed;
		RenderWindow * window;
		Gui * gui;
		TermOut::Ptr termOut;
		Color background;
		Font * default_font;
		void processEvents();
};

ZENITH_FOOTER

#endif
