/*
 * This file is part of POV-ESP32 library by Alexander Kirillov <shurik179@gmail.com>
 * See github.com/shurik179/pov-esp32 for details
 * Distributed under the terms of MIT license; see LICENSE file in the repository for details.
 *
 *  Requires the following libraries:
 *  Adafruit_Dotstar
 *
 *
 * This is a simple test of the POV-ESP32 library. It will just keep showing the same image,
 * hardcoded in the code (IMAGE). Note that it assumes that you have already created LittleFS
 * filesystem on your MCU and uploaded the file with name IMAGE to the root directory
 *
 * Before uploading the sketch to the staff, make sure to change the #define'd values to match your setup:
 *  NUM_PIXELS,  LINES_PER_SEC, IMAGE
 */
#include <LittleFS.h>
#include <pov-esp32.h>


//number of pixels in your strip/wand
#define NUM_PIXELS 30
// frame rate
#define LINES_PER_SEC 150.0f
uint32_t interval=1000000/LINES_PER_SEC; //interval between lines of image, in microseconds
//File name.
//Note that for LittleFS  we need to include leading slash
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
    //initilaize staff
    staff.begin();
    staff.blink(0x0000FF); //blink blue
    // Open LittleFS file system on the flash
    if ( !LittleFS.begin(false) ) {
        Serial.println("Error: filesystem doesn't not exist. Please format LittleFS filesystem");
        while(1) {
            staff.blink();//blink red
        }
    }
    //check if  file exists
    if (! LittleFS.exists(IMAGE)) {
        Serial.println("Error: file doesn't exist");
        while(1) {
            staff.blink(0x8000FF); //blink purple
        }
    }
    //otherwise, add the image to POV staff imagelist
    staff.addImage(IMAGE);
}
void loop(){
    if (staff.timeSinceUpdate()>interval) {
        staff.showNextLine();
    }
}
