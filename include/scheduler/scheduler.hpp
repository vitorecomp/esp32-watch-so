#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "app/app.hpp"
#include "event-bus/event-bus.hpp"

class Scheduler {
  public:
	Scheduler(EventBus*, App**);
};

#endif