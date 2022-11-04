#include "terminal/terminal-app.hpp"

void TerminalApp::setScreen(Screen *screen) {
	this->screen = screen;
}

void TerminalApp::clear() {
	this->screen->fillScreen(BLACK);
}

void TerminalApp::printf(const char *text) {
	// TODO control the posic here
	// if the posic is bigger than screen, must move the screen down
	screen->setCursor(0, 0);
	screen->setTextColor(screen->WHITE_COLOR);
	screen->setTextSize(1);
	screen->println(text);
}
