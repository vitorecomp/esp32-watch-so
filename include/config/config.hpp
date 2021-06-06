#ifndef CONFIG_H
#define CONFIG_H

#include "app/app.hpp"
#include "driver/connection/connection-interface.hpp"
#include "driver/input/input-interface.hpp"
#include "driver/output/output-interface.hpp"
#include "drivers/output/screen/screen.hpp"

typedef unsigned char uint_8;

class Config {
  public:
	Config();
	InputInterface** getInputInterfaces();
	inline uint_8 getInputInterfacesAmount();

	OutputInterface** getOutputInterfaces();
	inline uint_8 getOutputInterfacesAmount();
	Screen* getScreen();

	ConnectionInterface** getConnectionInterfaces();
	inline uint_8 getConnectionInterfacesAmount();

	App** getApps();
	inline uint_8 getAppAmount();
};

inline uint_8 Config::getInputInterfacesAmount() {
	return 0;
}

inline uint_8 Config::getOutputInterfacesAmount() {
	return 0;
}


inline uint_8 Config::getConnectionInterfacesAmount() {
	return 0;
}

inline uint_8 Config::getAppAmount() {
	return 0;
}


#endif