#include "terminal/terminal-app.hpp"

void TerminalApp::setScreen(Screen *screen) {
	this->screen = screen;
	this->terminalMatrix = (char**)malloc(screen->height() * sizeof(char*));
	for(int i = 0; i < this->HIST_SIZE; i++){
		this->terminalMatrix[i] = (char*)malloc(screen->width() * sizeof(char));
		this->terminalMatrix[i][0] = '\0';
	}
}

void TerminalApp::clear() {
	this->screen->fillScreen(BLACK);
}

void TerminalApp::printLine(int posX, int posY, const char *text) {
	screen->setCursor(posX, posY);
	screen->setTextColor(screen->WHITE_COLOR);
	screen->setTextSize(1);
	screen->println(text);
}

//IMPROVE: print only the lines that changed
void TerminalApp::printTerminal() {
	clear();
	for(int i = 0; i < screen->height(); i++){
		printLine(0, i, terminalMatrix[(lastPosition - i)%TerminalApp::HIST_SIZE]);
	}
}

void TerminalApp::index(const char *text) {
	//add the text to the terminal matrix
	lastPosition++;
	//copy string
	if(strlen(text) < screen->width())
		strcpy(terminalMatrix[lastPosition%TerminalApp::HIST_SIZE], text);
	else
		strncpy(terminalMatrix[lastPosition%TerminalApp::HIST_SIZE], text, screen->width());
	
}


void TerminalApp::println(const char *text) {
	//break the text in multiple lines if bigger than screen width
	int init = 0, end = screen->width();
	while(end < strlen(text)){
		//add the line to the line matrix
		index(text + init);
		init = end;
		end += screen->width();
	}

	//add the last part of the text
	index(text + init);

	//print the terminal matrix
	
	printTerminal();
}
