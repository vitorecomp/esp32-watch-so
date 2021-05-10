#ifndef EVENT_BUS_H
#define EVENT_BUS_H

typedef unsigned char uint_8;

class EventBus {
	// static** teste() funcues[128];

  public:
	EventBus();
	void enrool(uint_8 code, char* data, uint_8 function);
	void notify(uint_8 code, char* data);
};

#endif