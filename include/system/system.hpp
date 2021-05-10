#ifndef SYSTEM_H
#define SYSTEM_H

#include "config/config.hpp"
#include "event-bus/event-bus.hpp"
#include "scheduler/scheduler.hpp"
class MainSystem {
  private:
	Config *config;
	EventBus *eventBus;
	Scheduler *scheduler;

  private:
	void printSystemInfo();

  public:
	MainSystem(Config *);
	void init();
};

#endif