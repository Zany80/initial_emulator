#pragma once

#include <zenith80.hpp>
#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>

#include <CPU/Z80.hpp>

ZENITH_HEADER

class GR80{
public:
	GR80(tgui::Canvas::Ptr canvas,Z80 *cpu);
	~GR80();
	void uploadSprite(uint16_t index,uint8_t * sprite,uint8_t transparent_color=0);
	void drawSprite(uint16_t index,uint16_t x,uint16_t y);
	////void execute();
	void drawText(string text,float x,float y,float size,uint8_t color);
	void appendText(string s);
	void clearText();
	void clear(uint8_t color);
private:
	bool disable_sprites;
	tgui::Canvas::Ptr canvas;
	Z80 *cpu;
	sf::Color * palette;
	//text printing
	int canvas_y;
	sf::Text lastMsg;
	//sprites
	sf::Texture *texture_map;
	sf::Sprite *sprite_map;
};

ZENITH_FOOTER
