#ifndef INPUT_INTERFACE_H
#define INPUT_INTERFACE_H

#include "event-bus/event-bus.hpp"

class InputInterface {
  public:
	virtual void init();
	virtual void setEvents(EventBus*);
};

#endif