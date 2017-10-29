#include <debugger/main.hpp>

#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;

#include <TGUI/TGUI.hpp>
using tgui::Gui;

int main(int argc,char ** argv){
	RenderWindow window(VideoMode(800,600),"Zenith80 Debugger");
	while(window.isOpen()){
		processEvents();
	}
}

void processEvents(){

}
