/*
 * This file is part of POV-ESP32 library by Alexander Kirillov <shurik179@gmail.com>
 * See github.com/shurik179/pov-esp32 for details
 * Distributed under the terms of MIT license; see LICENSE file in the repository for details.
 *
 *  Requires the following libraries:
 *  Adafruit_Dotstar
 *
 *
 * This is a test of the POV library using a Hall effect sensor or a similar
 *    input as a trigger. It functions as follows:
 *
 *  - the staff starts  in show mode, showing just one image (to
 *    select the image edit the line #define IMAGE below. The frame rate (i.e. how many
 *    lines to show pere second) is determined by value of LINES_PER_SEC below
 *    It shows the image once and then waits until pin PIN_TRIGGER is pulled low
 *    after which it restarts the image.
 *
 *    This is common method for rotating displays: PIN_TRIGGER would be
 *    connected to a sensor (e.g. a Hall effect sensor) which is triggered as
 *    the staff rotates to a certain position.  To gurarantee that each activation
 *    of the trigger pin only is counted once, we add some blackout period for
 *    debouncing
 *
 *
 *
 *  Before uploading the sketch to the staff, make sure to change the #define'd values to match your setup:
 *  NUM_PIXELS,   PIN_TRIGGER, LINES_PER_SEC, IMAGE
 *  Finally it is assumed that you have already created the LittleFS filesystem on your
 *  flash memory
 */
#include <FastLED.h>
#include <pov.h>
//number of pixels in your strip/wand
#define NUM_PIXELS 30

// Trigger pin -- pull low to restart showing the image
#define PIN_TRIGGER 6

// Blackout time, in ms
#define BLACKOUT_TIME 150

// frame rate
#define LINES_PER_SEC 150.0f
uint32_t interval=1000000/LINES_PER_SEC; //interval between lines of image, in microseconds

#define IMAGE "/rg-lines.bmp"


/* Global Variables */
POV staff(NUM_PIXELS);
uint32_t blackoutEnd=0;   //for debouncing trigger reading; time to start checking again, in ms

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
        checkTrigger();
        updateStaff();
}

void checkTrigger(){
    if ( (millis()>blackoutEnd) && (digitalRead(PIN_TRIGGER)== LOW)  ){
        //trigger acivated
        //start blackout, for debouncing
        blackoutEnd=millis()+BLACKOUT_TIME;
        staff.restartImage();
        staff.paused=false;
    }
}

void updateStaff(){
    int16_t lineNumber;
    if ( !staff.paused   && (staff.timeSinceUpdate()>interval)  ) {
        lineNumber = staff.showNextLine();
        if (lineNumber == 0) {
            //we complted showing hte image; let's pause 
            staff.paused = true;
            staff.blank();
        }
    }
}
