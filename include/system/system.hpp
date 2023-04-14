#ifndef SYSTEM_H
#define SYSTEM_H

#include "config/config.hpp"
#include "event-bus/event-bus.hpp"
#include "scheduler/scheduler.hpp"
#include "terminal/terminal-app.hpp"

class MainSystem {
  private:
	Config *config;
	EventBus *eventBus;
	TerminalApp *terminalApp;
	Scheduler *scheduler;

	char outputBuffer[256];
  private:
	void printSystemInfo();

  public:
	MainSystem(Config *);
	void init();
};

#endif