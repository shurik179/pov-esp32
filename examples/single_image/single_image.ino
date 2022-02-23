/*
 * This file is part of POV Staff project by Alexander Kirillov <shurik179@gmail.com>
 * See github.com/shurik179/pov-library for details
 * Distributed under the terms of MIT license; see LICENSE file in the repository for details.
 *
 *  Requires the following libraries:
 *  FastLED
 *  Adafruit_TinyUSB
 *  Adafruit_SPIFlash
 *  Sd_Fat (adafruit fork)
 *
 *
 * This is a simple test of the POV library. It functions as follows:
 *  - if at startup pin PIN_MODE_SELECT (defined below) is pulled low, it puts the staff in
 *    image upload mode; if the staff is connected to the computer by USB cable, it appears
 *    as an external drive so you can drag and drop your BMP images to it
 *
 *  - otherwise, the staff goes into usual show mode, showing just one image (to
 *    select the image edit the line #define IMAGE below. The frame rate (i.e. how many
 *    lines to show per second) is determined by value of LINES_PER_SEC below
 *
 *
 * Before uploading the sketch to the staff, make sure to change the #define'd values to match your setup:
 *  NUM_PIXELS, LED_TYPE, COLOR_ORDER, PIN_MODE_SELECT, LINES_PER_SEC, IMAGE
 *  Also, for M4 and RP2040 based boards, make sure that in your Arduino IDE you have selected
 *  Tools->USB stack: TinyUSB
 *  Finally it is assumed that you have already created the FAT filesystem on your
 *  flash memory, using SdFat_format example sketch from Sd_Fat library (Adafruit fork)
 */
#include <FastLED.h>
#include "staff.h"
//number of pixels in your strip/wand
#define NUM_PIXELS 30
// Strip type. Common options are DOTSTAR (APA102, SK9822) and NEOPIXEL (WS2812B, SK6812 and
// compatible). For other options, see FastLED documentation
#define LED_TYPE DOTSTAR
// color order. For DOTSTAR (APA102), common order is BGR
// For NeoPixel (WS2812B), most common is  GRB
#define COLOR_ORDER BGR

/*Mode selection pin
  If at startup this pin is pulled low, staff goes into image upload mode;
  otherwise, it goes into usual (show)  mode
*/
#define PIN_MODE_SELECT 5

// frame rate
#define LINES_PER_SEC 150.0f
uint32_t interval=1000000/LINES_PER_SEC; //interval between lines of image, in microseconds

#define IMAGE "rg-lines.bmp"


/* Global Variables */
CRGB leds[NUM_PIXELS];
POVstaff staff(NUM_PIXELS, leds);





void setup(){
// If using hardware SPI, use this version
    FastLED.addLeds<LED_TYPE, COLOR_ORDER>(leds, NUM_PIXELS);
//If NOT using hardware SPI, comment the previous line. Instead,
// use one of the versions below,
// replacing DATA_PIN and CLOCK_PIN by correct pin numbers
// FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_PIXELS);
// FastLED.addLeds<NEOPIXEL, DATA_PIN, COLOR_ORDER>(leds, NUM_PIXELS);

    pinMode(PIN_MODE_SELECT, INPUT_PULLUP);

    //if pin is pulled low, go into upload mode!
    //note: in this case, there should be no Serial.begin() before this, and no delay()
    if (digitalRead(PIN_MODE_SELECT)==LOW) {
        staff.begin(MODE_UPLOAD);
    } else {
        //otherwise, regular show
        staff.begin(MODE_SHOW);
        // blink to indicate that staff is alive and working.
        // You can use any of predefined CRGB colors: https://github.com/FastLED/FastLED/wiki/Pixel-reference#predefined-colors-list
        // You can also omit the color; in this case, it will default to red.
        staff.blink(CRGB::Red);
        staff.addImage(IMAGE);
    }


}

void loop(){
    if ((staff.mode()==MODE_SHOW) && (staff.timeSinceUpdate()>interval)) {
        staff.showNextLine();
    }
}
