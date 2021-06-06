#include "config/config.hpp"

InputInterface** Config::getInputInterfaces() {
	return nullptr;
}


OutputInterface** Config::getOutputInterfaces() {
	return nullptr;
}


ConnectionInterface** Config::getConnectionInterfaces() {
	return nullptr;
}

Screen* Config::getScreen() {
	OutputInterface** interfaces = getOutputInterfaces();
	for (uint_8 i = 0; i < getOutputInterfacesAmount(); i++) {
		if (interfaces[i]->getName() == SCREEN_NAME) {
			return (Screen*)interfaces[i];
		}
	}
	return nullptr;
}


App** Config::getApps() {
	return nullptr;
}
