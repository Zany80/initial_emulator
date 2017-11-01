#include <GPU/GR80.hpp>

#include <SFML/Graphics.hpp>
using sf::Color;
using sf::Texture;
using sf::Image;

#include <main.hpp>

#include <iostream>

ZENITH_HEADER

GR80::GR80(tgui::Canvas::Ptr canvas,Z80* cpu){
	this->disable_sprites=false;
	this->canvas=canvas;
	this->cpu=cpu;
	palette=new Color[16]{
		{0,0,0},{255,255,255},{255,0,0},{0,255,0},{0,0,255},{255,255,0},{255,0,255},{0,255,255},
		{3,224,187},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
	};
	texture_map=new Texture[512];
	sprite_map=new sf::Sprite[512];
	for(int i=0;i<512;i++){
		if(!texture_map[i].create(8,8)){
			std::cerr<<"Failed to create texture "<<i<<"! Disabling sprites..."<<std::endl;
			disable_sprites=true;
			break;
		}
		sprite_map[i].setTexture(texture_map[i]);
	}
	//TODO: should probably free the created textures when sprite initialization fails, just to be nice to other programs.
	canvas_y=0;
}

GR80::~GR80(){
	delete[] palette;
	delete[] sprite_map;
	delete[] texture_map;
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

void GR80::uploadSprite(uint16_t index,uint8_t * sprite,uint8_t transparent_color){
	std::cout<<"[GPU] Sprite uploaded. Index: "<<index<<", sprite contents: \"";
	for(int i=0;i<32;i++){
		std::cout<<hex((int)*(sprite+i));
	}
	std::cout<<"\"...\n";
	//sprite structure: 8 pixel rows (4 bytes) follow one after another in 8 rows, giving an 8x8 sprite and taking 32 bytes in the ROM.
	//The upper half of each byte is the first member of the pixel-pair. The lower half is the second pixel.
	transparent_color=transparent_color%16;
	uint8_t alpha=palette[transparent_color].a;
	palette[transparent_color].a=0;
	sf::Image image;
	image.create(8,8);
	for(int i=0;i<8;i++){
		for(int j=0;j<4;j++){
			uint8_t byte=*(sprite+(4*i)+j);
			image.setPixel(j*2,i,palette[(byte&0xF0)>>4]);
			image.setPixel(j*2+1,i,palette[byte&0x0F]);
		}
	}
	palette[transparent_color].a=alpha;
	texture_map[index%512].update(image);
}

void GR80::drawSprite(uint16_t index,uint16_t x,uint16_t y){
	std::cout<<"[GPU] Drawing sprite.. Index: "<<index<<", x: "<<x<<", y: "<<y<<"...\n";
	index=index%512;
	sf::Sprite * sprite=&sprite_map[index];
	sprite->setPosition(x*(float)800/(float)256,y*(float)500/(float)128);
	sprite->setScale(800/256,(float)500/(float)128);
	canvas->draw(*sprite);
}

////void GR80::execute(){
	
////}

ZENITH_FOOTER
