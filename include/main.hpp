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
using tgui::Canvas;
typedef tgui::ChatBox TermOut;

#include <SFML/System.hpp>
using sf::Clock;

typedef const char * cstring;

ZENITH_HEADER

class Main{
	public:
		Main(int argc,char ** argv);
		~Main();
		static int main(int argc,char** argv);
		void shutdown();
		int run();
		static Main * instance;
		Z80 * cpu;
		bool cpm_emu;
		void putchar(char c);
		void putint(int c);
		void putmsg(string s);
		uint8_t key();
		void resetKeyBuffer();
		void resetClock();
		void drawText(string text,float x,float y,float size,uint8_t color);
		void clear(uint8_t color);
		void display();
		void drawRect(int x,int y,int w,int h, uint8_t color);
	private:
		Color * palette;
		Clock accuracy_clock;
		vector<uint8_t> key_buffer;
		uint64_t unit;
		uint64_t clock_speed;
		RenderWindow * window;
		Gui * gui;
		TermOut::Ptr termOut;
		Canvas::Ptr canvas;
		Color background;
		Font * default_font;
		void processEvents();
		int canvas_y=0;
		sf::Text lastMsg;

};

ZENITH_FOOTER

#endif
