/*
 * This file is part of POV library by Alexander Kirillov <shurik179@gmail.com>
 * See github.com/shurik179/pov-library for details
 * Distributed under the terms of MIT license; see LICENSE file in the repository for details.
 *
 *  Requires the following libraries:
 *  Adafruit_Dotstar
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
 *  NUM_PIXELS,  LINES_PER_SEC, IMAGE
 *  Finally it is assumed that you have already created the LittleFS filesystem on your
 *  flash memory, and uploaded the file with name IMAGE to the root directory
 */
#include <LittleFS.h>
#include <pov.h>
//number of pixels in your strip/wand
#define NUM_PIXELS 30

// frame rate
#define LINES_PER_SEC 150.0f
uint32_t interval=1000000/LINES_PER_SEC; //interval between lines of image, in microseconds
//File name.
//Note that we need to include leading slash
#define IMAGE "/rg-lines.bmp"

//Data and clock pins -  if NOT  using hardware SPI, change values as needed and uncomment
// #define DATAPIN 2
// #define CLOCKPIN 3


/* Global Variables */

POV staff(NUM_PIXELS);
//if NOT using SPI, use this instead
// POV staff(NUM_PIXELS, DATAPIN, CLOCKPIN)





void setup(){
    Serial.begin(9600);
    // Open LittleFS file system on the flash
    if ( !LittleFS.begin(false) ) {
        Serial.println("Error: filesystem doesn't not exist. Please format LittleFS filesystem");
        while(1) yield();
    }
    //check if  file exists
    if (! LittleFS.exists(IMAGE)) {
        Serial.println("Error: file doesn't exist");
        while(1) yield();
    }
    staff.begin();
    staff.blink();
    staff.addImage(IMAGE);
}
void loop(){
    if (staff.timeSinceUpdate()>interval) {
        staff.showNextLine();
    }
}
