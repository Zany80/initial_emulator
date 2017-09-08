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

#include <CPU/Z80.hpp>

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
		void put_char(char c);
	private:
		RenderWindow * window;
		Color background;
		Text * git_revision;
		Text * terminal;
		string terminal_string;
		Font * default_font;
		void processEvents();
};

Main* Main::instance;

ZENITH_FOOTER

#endif
