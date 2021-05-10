#include "drivers/input/whell-button/wheel-button.hpp"

const uint8_t WheelButton::BUTTON_PRESS;
const uint8_t WheelButton::BUTTON_LEFT;
const uint8_t WheelButton::BUTTON_RIGHT;

void WheelButton::init() {
	pinMode(WheelButton::BUTTON_PRESS, INPUT_PULLUP);
	pinMode(WheelButton::BUTTON_LEFT, INPUT_PULLUP);
	pinMode(WheelButton::BUTTON_RIGHT, INPUT_PULLUP);

	// TODO start a therad
	// put it to run the run function
}


void WheelButton::loop() {
	if (digitalRead(BUTTON_LEFT) == LOW) {
		printf("pressed left");
	}
	if (digitalRead(BUTTON_RIGHT) == LOW) {
		printf("pressed rigth");
	}

	// Check if state changed from high to low (button press).
	if (digitalRead(BUTTON_PRESS) == LOW) {
		// Short delay to debounce button.
		delay(20);
		if (digitalRead(BUTTON_PRESS) == LOW) {
			printf("pressed center");
		}
	}
}
