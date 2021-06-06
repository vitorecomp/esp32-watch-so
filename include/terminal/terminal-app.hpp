#ifndef TERMINAL_APP
#define TERMINAL_APP

#include "drivers/output/screen/screen.hpp"


class TerminalApp {
	Screen* screen;

  public:
	void init();

	void setScreen(Screen* screen);
	void clear();
	void printf(const char*);
};


#endif