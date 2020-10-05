#ifndef CONNECTION_INTERFACE_H
#define CONNECTION_INTERFACE_H

#include "event-bus/event-bus.hpp"

class ConnectionInterface {
  public:
	virtual void init();
	virtual void setEvents(EventBus*);
};

#endif