/***************************************************
  This is a library for the ST7789 IPS SPI display.


 ****************************************************/

#include "config/config.hpp"
#include "system/system.hpp"

void setup() {
	Config config;
	MainSystem main(&config);
	main.init();
}

void loop() {
}
