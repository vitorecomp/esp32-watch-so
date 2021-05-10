#ifndef WHEEL_BUTTON
#define WHEEL_BUTTON

#include <Arduino.h>

#include "driver/input/input-interface.hpp"

typedef unsigned char uint8_t;

class WheelButton : InputInterface {
	static const uint8_t BUTTON_PRESS = 18;
	static const uint8_t BUTTON_LEFT = 5;
	static const uint8_t BUTTON_RIGHT = 19;

	void loop();

  public:
	void init();
};


#endif