#include "terminal/terminal-app.hpp"

void TerminalApp::setScreen(Screen *screen) {
	this->screen = screen;
}

void TerminalApp::clear() {
	this->screen->fillScreen(BLACK);
}

void TerminalApp::printLine(int posX, int posY, const char *text) {
	screen->setCursor(posX, posY);
	screen->setTextColor(screen->WHITE_COLOR);
	screen->setTextSize(1);
	if(strlen(text) > screen->width())
		screen->println(text[0, screen->width()]);
	else
		screen->println(text);
}

void TerminalApp::println(const char *text) {
	//break the text in multiple lines if bigger than screen width
	int init = 0, end = screen->width();
	while(end < strlen(text)){
		printLine(this->posX, this->posY++, text + init);
		init = end;
		end += screen->width();
	}
	printLine(this->posX, this->posY++, text + init);
}
