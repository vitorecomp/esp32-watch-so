#ifndef CONFIG_H
#define CONFIG_H

#include "app/app.hpp"
#include "driver/connection/connection-interface.hpp"
#include "driver/input/input-interface.hpp"
#include "driver/output/output-interface.hpp"

typedef unsigned char uint_8;

class Config {
  public:
	Config();
	InputInterface** getInputInterfaces();
	uint_8 getInputInterfacesAmount();

	OutputInterface** getOutputInterfaces();
	uint_8 getOutputInterfacesAmount();

	ConnectionInterface** getConnectionInterfaces();
	uint_8 getConnectionInterfacesAmount();

	App** getApps();
	uint_8 getAppAmount();
};

#endif