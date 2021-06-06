#ifndef SCREEN
#define SCREEN

// Hardware-specific library for ST7789 (with or without CS pin)
#include <Arduino_ST7789.h>

#include "driver/output/output-interface.hpp"


#define SCREEN_NAME "SCREEN"


#define TFT_DC 22
#define TFT_RST 23

#define TFT_MOSI 17	 // for hardware SPI data pin (all of available pins)
#define TFT_SCLK 16	 // for hardware SPI sclk pin (all of available pins)


class Screen : public OutputInterface, public Arduino_ST7789 {
  public:
	Screen()
	  : OutputInterface()
	  , Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK){};
	void init();
	const char* getName();

	static const uint16_t WHITE_COLOR;
};

const uint16_t Screen::WHITE_COLOR = WHITE;


#endif