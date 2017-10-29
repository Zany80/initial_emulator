#pragma once

#include <zenith80.hpp>
#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
using sf::Color;

#include <CPU/Z80.hpp>

ZENITH_HEADER

class GR80{
public:
	GR80(tgui::Canvas::Ptr canvas,Z80 *cpu);
	~GR80();
	void uploadSprite(uint8_t index,uint8_t * sprite,uint8_t x,uint8_t y);
	void drawSprite(uint8_t index);
	////void execute();
	void drawText(string text,float x,float y,float size,uint8_t color);
	void appendText(string s);
	void clearText();
	void clear(uint8_t color);
private:
	tgui::Canvas::Ptr canvas;
	Z80 *cpu;
	Color * palette;
	int canvas_y;
	sf::Text lastMsg;
};

ZENITH_FOOTER
