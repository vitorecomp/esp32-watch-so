#ifndef INPUT_INTERFACE_H
#define INPUT_INTERFACE_H

#include "event-bus/event-bus.hpp"

class InputInterface {
	EventBus* eventBus;

  public:
	InputInterface();
	void setEventBus(EventBus*);

	virtual const char* getName();
	virtual void init();
};

#endif