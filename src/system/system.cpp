#include "system/system.hpp"

#include <Adafruit_GFX.h>  // Core graphics library by Adafruit
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Arduino_ST7789.h>	 // Hardware-specific library for ST7789 (with or without CS pin)
#include <SPI.h>
#include <stdio.h>

#include "esp_spi_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void start();
void lookButton();

void MainSystem::init() {
	printf("Hello world!\n");

	/* Print chip information */
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);
	printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ", chip_info.cores,
		   (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
		   (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

	printf("silicon revision %d, ", chip_info.revision);

	printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
		   (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" :
														   "external");

	for (int i = 10; i >= 0; i--) {
		printf("Wait to restarting in %d seconds...\n", i);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	start();
	for (int i = 0; i < 20000; i++) {
		lookButton();
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}

	printf("Restarting now.\n");
	fflush(stdout);
	esp_restart();
}

#define TFT_DC 22
#define TFT_RST 23

#define TFT_MOSI 17	 // for hardware SPI data pin (all of available pins)
#define TFT_SCLK 16	 // for hardware SPI sclk pin (all of available pins)

#define BUTTON_PRESS 18
#define BUTTON_LEFT 5
#define BUTTON_RIGHT 19

#define PIXEL_PIN 33  // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 1

#define BUZZER 32

class Test {};

Adafruit_NeoPixel strip =
	Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// You can use different type of hardware initialization
// using hardware SPI (11, 13 on UNO; 51, 52 on MEGA; ICSP-4, ICSP-3 on DUE and
// etc) Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST); //for display
// without CS pin Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_CS);
// //for display with CS pin or you can use software SPI on all available pins
// (slow)
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI,
									TFT_SCLK);	// for display without CS pin
// Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK,
// TFT_CS); //for display with CS pin
bool oldState = HIGH;
int showType = 0;

float p = 3.1415926;

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) {
		return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170) {
		WheelPos -= 85;
		return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void testlines(uint16_t color) {
	tft.fillScreen(BLACK);
	for (int16_t x = 0; x < tft.width(); x += 6) {
		tft.drawLine(0, 0, x, tft.height() - 1, color);
	}
	for (int16_t y = 0; y < tft.height(); y += 6) {
		tft.drawLine(0, 0, tft.width() - 1, y, color);
	}

	tft.fillScreen(BLACK);
	for (int16_t x = 0; x < tft.width(); x += 6) {
		tft.drawLine(tft.width() - 1, 0, x, tft.height() - 1, color);
	}
	for (int16_t y = 0; y < tft.height(); y += 6) {
		tft.drawLine(tft.width() - 1, 0, 0, y, color);
	}

	tft.fillScreen(BLACK);
	for (int16_t x = 0; x < tft.width(); x += 6) {
		tft.drawLine(0, tft.height() - 1, x, 0, color);
	}
	for (int16_t y = 0; y < tft.height(); y += 6) {
		tft.drawLine(0, tft.height() - 1, tft.width() - 1, y, color);
	}

	tft.fillScreen(BLACK);
	for (int16_t x = 0; x < tft.width(); x += 6) {
		tft.drawLine(tft.width() - 1, tft.height() - 1, x, 0, color);
	}
	for (int16_t y = 0; y < tft.height(); y += 6) {
		tft.drawLine(tft.width() - 1, tft.height() - 1, 0, y, color);
	}
}

void testdrawtext(char *text, uint16_t color) {
	tft.setCursor(0, 0);
	tft.setTextColor(color);
	tft.setTextWrap(true);
	tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
	tft.fillScreen(BLACK);
	for (int16_t y = 0; y < tft.height(); y += 5) {
		tft.drawFastHLine(0, y, tft.width(), color1);
	}
	for (int16_t x = 0; x < tft.width(); x += 5) {
		tft.drawFastVLine(x, 0, tft.height(), color2);
	}
}

void testdrawrects(uint16_t color) {
	tft.fillScreen(BLACK);
	for (int16_t x = 0; x < tft.width(); x += 6) {
		tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x,
					 color);
	}
}

void testfillrects(uint16_t color1, uint16_t color2) {
	tft.fillScreen(BLACK);
	for (int16_t x = tft.width() - 1; x > 6; x -= 6) {
		tft.fillRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x,
					 color1);
		tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x,
					 color2);
	}
}

void testfillcircles(uint8_t radius, uint16_t color) {
	for (int16_t x = radius; x < tft.width(); x += radius * 2) {
		for (int16_t y = radius; y < tft.height(); y += radius * 2) {
			tft.fillCircle(x, y, radius, color);
		}
	}
}

void testdrawcircles(uint8_t radius, uint16_t color) {
	for (int16_t x = 0; x < tft.width() + radius; x += radius * 2) {
		for (int16_t y = 0; y < tft.height() + radius; y += radius * 2) {
			tft.drawCircle(x, y, radius, color);
		}
	}
}

void testtriangles() {
	tft.fillScreen(BLACK);
	int color = 0xF800;
	int t;
	int w = tft.width() / 2;
	int x = tft.height() - 1;
	int y = 0;
	int z = tft.width();
	for (t = 0; t <= 15; t++) {
		tft.drawTriangle(w, y, y, x, z, x, color);
		x -= 4;
		y += 4;
		z -= 4;
		color += 100;
	}
}

void testroundrects() {
	tft.fillScreen(BLACK);
	int color = 100;
	int i;
	int t;
	for (t = 0; t <= 4; t += 1) {
		int x = 0;
		int y = 0;
		int w = tft.width() - 2;
		int h = tft.height() - 2;
		for (i = 0; i <= 16; i += 1) {
			tft.drawRoundRect(x, y, w, h, 5, color);
			x += 2;
			y += 3;
			w -= 4;
			h -= 6;
			color += 1100;
		}
		color += 100;
	}
}

void tftPrintTest() {
	tft.setTextWrap(false);
	tft.fillScreen(BLACK);
	tft.setCursor(0, 30);
	tft.setTextColor(RED);
	tft.setTextSize(1);
	tft.println("Hello DSTIKE");
	tft.setTextColor(YELLOW);
	tft.setTextSize(2);
	tft.println("Hello DSTIKE");
	tft.setTextColor(GREEN);
	tft.setTextSize(3);
	tft.println("Hello DSTIKE");
	tft.setTextColor(BLUE);
	tft.setTextSize(4);
	tft.print(1234.567);
	delay(1500);
	tft.setCursor(0, 0);
	tft.fillScreen(BLACK);
	tft.setTextColor(WHITE);
	tft.setTextSize(0);
	tft.println("DSTIKE!");
	tft.setTextSize(1);
	tft.setTextColor(GREEN);
	tft.print(p, 6);
	tft.println(" Want pi?");
	tft.println(" ");
	tft.print(8675309, HEX);  // print 8,675,309 out in HEX!
	tft.println(" Print HEX!");
	tft.println(" ");
	tft.setTextColor(WHITE);
	tft.println("Sketch has been");
	tft.println("running for: ");
	tft.setTextColor(MAGENTA);
	tft.print(millis() / 1000);
	tft.setTextColor(WHITE);
	tft.print(" seconds.");
}

void mediabuttons() {
	// play
	tft.fillScreen(BLACK);
	tft.fillRoundRect(25, 10, 78, 60, 8, WHITE);
	tft.fillTriangle(42, 20, 42, 60, 90, 40, RED);
	delay(500);
	// pause
	tft.fillRoundRect(25, 90, 78, 60, 8, WHITE);
	tft.fillRoundRect(39, 98, 20, 45, 5, GREEN);
	tft.fillRoundRect(69, 98, 20, 45, 5, GREEN);
	delay(500);
	// play color
	tft.fillTriangle(42, 20, 42, 60, 90, 40, BLUE);
	delay(50);
	// pause color
	tft.fillRoundRect(39, 98, 20, 45, 5, RED);
	tft.fillRoundRect(69, 98, 20, 45, 5, RED);
	// play color
	tft.fillTriangle(42, 20, 42, 60, 90, 40, GREEN);
	tft.setCursor(60, 200);
	tft.setTextColor(RED);
	tft.setTextSize(4);
	tft.println("DSTIKE");
}

void soundone() {
	unsigned char i;

	for (i = 0; i < 5; i++)

	{
		for (i = 0; i < 200; i++)

		{
			digitalWrite(BUZZER, HIGH);

			delay(1);  // Change this could adjust voice

			digitalWrite(BUZZER, LOW);

			delay(1);
		}

		for (i = 0; i < 100; i++)

		{
			digitalWrite(BUZZER, HIGH);

			delay(2);

			digitalWrite(BUZZER, LOW);

			delay(2);
		}
	}
}
void soundtwo() {
	unsigned char i;

	for (i = 0; i < 5; i++)

	{
		for (i = 0; i < 100; i++)

		{
			digitalWrite(BUZZER, HIGH);

			delay(2);  // Change this could adjust voice

			digitalWrite(BUZZER, LOW);

			delay(2);
		}

		for (i = 0; i < 200; i++)

		{
			digitalWrite(BUZZER, HIGH);

			delay(1);

			digitalWrite(BUZZER, LOW);

			delay(1);
		}
	}
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
	for (uint16_t i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, c);
		strip.show();
		delay(wait);
	}
}

void rainbow(uint8_t wait) {
	uint16_t i, j;

	for (j = 0; j < 256; j++) {
		for (i = 0; i < strip.numPixels(); i++) {
			strip.setPixelColor(i, Wheel((i + j) & 255));
		}
		strip.show();
		delay(wait);
	}
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
	uint16_t i, j;

	for (j = 0; j < 256 * 5; j++) {	 // 5 cycles of all colors on wheel
		for (i = 0; i < strip.numPixels(); i++) {
			strip.setPixelColor(
				i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
		}
		strip.show();
		delay(wait);
	}
}

// Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
	for (int j = 0; j < 10; j++) {	// do 10 cycles of chasing
		for (int q = 0; q < 3; q++) {
			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(i + q, c);	// turn every third pixel on
			}
			strip.show();

			delay(wait);

			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(i + q, 0);	// turn every third pixel off
			}
		}
	}
}

// Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
	for (int j = 0; j < 256; j++) {	 // cycle all 256 colors in the wheel
		for (int q = 0; q < 3; q++) {
			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(
					i + q, Wheel((i + j) % 255));  // turn every third pixel on
			}
			strip.show();

			delay(wait);

			for (int i = 0; i < strip.numPixels(); i = i + 3) {
				strip.setPixelColor(i + q, 0);	// turn every third pixel off
			}
		}
	}
}

void startShow(int i) {
	switch (i) {
		case 0:
			colorWipe(strip.Color(0, 0, 0), 50);  // Black/off
			break;
		case 1:
			colorWipe(strip.Color(255, 0, 0), 50);	// Red
			break;
		case 2:
			colorWipe(strip.Color(0, 255, 0), 50);	// Green
			break;
		case 3:
			colorWipe(strip.Color(0, 0, 255), 50);	// Blue
			break;
		case 4:
			theaterChase(strip.Color(127, 127, 127), 50);  // White
			break;
		case 5:
			theaterChase(strip.Color(127, 0, 0), 50);  // Red
			break;
		case 6:
			theaterChase(strip.Color(0, 0, 127), 50);  // Blue
			break;
			/*
		case 7: rainbow(20);
				break;
		case 8: rainbowCycle(20);
				break;
		case 9: theaterChaseRainbow(50);
				break;*/
	}
}

void start() {
	//   Serial.begin(9600);
	pinMode(BUTTON_PRESS, INPUT_PULLUP);
	pinMode(BUTTON_LEFT, INPUT_PULLUP);
	pinMode(BUTTON_RIGHT, INPUT_PULLUP);
	pinMode(BUZZER, OUTPUT);

	//   Serial.print("Hello! ST7789 TFT Test");

	tft.init(240, 240);	 // initialize a ST7789 chip, 240x240 pixels

	//   Serial.println("Initialized");

	uint16_t time = millis();
	tft.fillScreen(BLACK);
	time = millis() - time;

	//   Serial.println(time, DEC);
	delay(500);

	// large block of text
	tft.fillScreen(BLACK);
	testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
				 "Curabitur adipiscing ante sed nibh tincidunt feugiat. "
				 "Maecenas enim massa, fringilla sed malesuada et, malesuada "
				 "sit amet turpis. Sed porttitor neque ut ante pretium vitae "
				 "malesuada nunc bibendum. Nullam aliquet ultrices massa eu "
				 "hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor "
				 "imperdiet posuere. ",
				 WHITE);
	delay(1000);

	// tft print function
	tftPrintTest();
	delay(2000);

	// a single pixel
	tft.drawPixel(tft.width() / 2, tft.height() / 2, GREEN);
	delay(500);

	// line draw test
	testlines(YELLOW);
	delay(500);

	// optimized lines
	testfastlines(RED, BLUE);
	delay(500);

	testdrawrects(GREEN);
	delay(500);

	testfillrects(YELLOW, MAGENTA);
	delay(500);

	tft.fillScreen(BLACK);
	testfillcircles(10, BLUE);
	testdrawcircles(10, WHITE);
	delay(500);

	testroundrects();
	delay(500);

	testtriangles();
	delay(500);

	mediabuttons();
	delay(500);

	tft.invertDisplay(true);
	delay(500);

	// tft.invertDisplay(false);
	// delay(500);

	//   Serial.println("done");
	delay(500);
	//   strip.begin();
	//   strip.show(); // Initialize all pixels to 'off'
}

void lookButton() {
	if (digitalRead(BUTTON_LEFT) == LOW) {
		soundone();
	}
	if (digitalRead(BUTTON_RIGHT) == LOW) {
		soundtwo();
	}
	// Get current button state.
	bool newState = digitalRead(BUTTON_PRESS);

	// Check if state changed from high to low (button press).
	if (newState == LOW && oldState == HIGH) {
		// Short delay to debounce button.
		delay(20);
		// Check if button is still low after debounce.
		newState = digitalRead(BUTTON_PRESS);
		if (newState == LOW) {
		}
	}
	//   showType++;
	// if (showType > 6)
	// 	showType=0;
	// startShow(showType);

	// Set the last button state to the old state.
	oldState = newState;
}