#ifndef APP_H
#define APP_H

#include "driver/connection/connection-interface.hpp"
#include "driver/output/output-interface.hpp"
#include "event-bus/event-bus.hpp"

class App {
  public:
	virtual void init();
	virtual void startThread();

	virtual void setConnections(ConnectionInterface**);

	virtual void enrollActiveEvents(EventBus*);
	virtual void enrollBackgroundEvents(EventBus*);

	virtual void setOutputInterface(OutputInterface**);
};

#endif