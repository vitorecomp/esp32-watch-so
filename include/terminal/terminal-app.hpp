#ifndef TERMINAL_APP
#define TERMINAL_APP

#include "drivers/output/screen/screen.hpp"


class TerminalApp {
  private:
	static const int HIST_SIZE = 200;
	Screen* screen;

	int lastPosition = -1;
	int histPosition = 0;

	char **terminalMatrix;

	void printLine(int posX, int posY, const char* text);
	void printTerminal();

void index(const char * text);
	
  public:
	void init();

	void setScreen(Screen* screen);
	void clear();
	void println(const char*);
};


#endif