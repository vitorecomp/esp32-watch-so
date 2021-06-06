#ifndef OUTPUT_INTERFACE_H
#define OUTPUT_INTERFACE_H

class OutputInterface {
  public:
	virtual const char* getName();
	virtual void init();
};

#endif