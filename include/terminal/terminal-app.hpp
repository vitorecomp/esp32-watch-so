#ifndef TERMINAL_APP
#define TERMINAL_APP

#include "drivers/output/screen/screen.hpp"


class TerminalApp {
	Screen* screen;
	int posX, posY;

	void printLine(int posX, int posY, const char* text);
  public:
	void init();

	void setScreen(Screen* screen);
	void clear();
	void println(const char*);
};


#endif