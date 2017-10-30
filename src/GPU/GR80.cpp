#include <GPU/GR80.hpp>
#include <main.hpp>

#include <iostream>
using std::cerr;
using std::endl;

ZENITH_HEADER

GR80::GR80(tgui::Canvas::Ptr canvas,Z80* cpu){
	this->canvas=canvas;
	this->cpu=cpu;
	palette=new Color[16]{
		{255,255,255},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
		{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
	};
	//vram structure: sprites follow one after another. Each one is 8x8, which at 2 pixels per byte takes 32 bytes. As there are 16KB of VRAM, there can be up to 512 sprites.
	vram=new uint8_t[0x4000];
	canvas_y=0;
}

GR80::~GR80(){
	delete[] palette;
	delete[] vram;
}

void GR80::clear(uint8_t color){
	canvas->clear(palette[color]);
}

void GR80::drawText(string text,float x,float y,float size, uint8_t color){
	sf::Text message(text,*Main::instance->default_font,size);
	lastMsg=message;
	message.setFillColor(palette[color]);
	message.setPosition(x,y);
	message.setString(tgui::Text::wordWrap(512,message.getString(),*Main::instance->default_font,size,false));
	canvas->draw(message);
}

void GR80::appendText(string s){
	drawText(lastMsg.getString()+s,0,canvas_y,15,1);
}

void GR80::clearText(){
	lastMsg.setString("");
}

void GR80::uploadSprite(uint16_t index,uint8_t * sprite){
	uint8_t *sprite_address=vram+(index*32);
	for(int i=0;i<32;i++){
		*(sprite_address+i)=*(sprite+i);
	}
}

void GR80::drawSprite(uint16_t index,float x,float y,uint8_t transparent_color){
	
}

////void GR80::execute(){
	
////}

ZENITH_FOOTER
