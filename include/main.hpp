#ifndef _ZENITH80_MAIN_HPP_
#define _ZENITH80_MAIN_HPP_

#include "zenith80.hpp"
#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::Color;

#include <CPU/Z80.hpp>

ZENITH_HEADER

class Main{
	public:
		Main();
		~Main();
		static int main(int argc,char** argv);
		int run(int argc,char ** argv);
		static Main * instance;
		Z80 * cpu;
	private:
		RenderWindow * window;
		Color background;
		void processEvents();
};

Main* Main::instance;

ZENITH_FOOTER

#endif
