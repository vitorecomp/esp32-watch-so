#include "driver/input/input-interface.hpp"

InputInterface::InputInterface() {
	this->eventBus = nullptr;
}

void InputInterface::setEventBus(EventBus* eventBus) {
	this->eventBus = eventBus;
}