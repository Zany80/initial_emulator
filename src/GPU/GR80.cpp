#include <GPU/GR80.hpp>
#include <main.hpp>

ZENITH_HEADER

GR80::GR80(tgui::Canvas::Ptr canvas,Z80* cpu){
	this->canvas=canvas;
	this->cpu=cpu;
	palette=new Color[16]{
		{255,255,255},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
		{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
	};
	canvas_y=0;
}

GR80::~GR80(){
	delete[] palette;
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

////void GR80::execute(){
	
////}

ZENITH_FOOTER
