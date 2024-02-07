/*
 * This file is part of POV-ESP32 library by Alexander Kirillov <shurik179@gmail.com>
 * See github.com/shurik179/pov-esp32 for details
 * Distributed under the terms of MIT license; see LICENSE file in the repository for details.
 *
 *  Requires the following libraries:
 *  Adafruit_Dotstar
 *
 *
 *
 * This is a multiimage test of the POV-ESP32  library. It reads the imagelist file
 * at the root of file system and starts showing images from that list
 * showing each image for the duration written in the imagelist file.
 * Imagelist file should be in the following format: one line per image file,
 * containing fiename (up to 30 symbols), including extension, optionally
 * followed by number of seconds (whole numbers only). E.g.
 * /filename1.bmp   20
 * /filename2.bmp   30
 * /filename1.bmp
 * IMPORTANT: filenames should include leading slash !!
 * Each image will be shown for specified number of seconds. If number of seonds
 * it will be shown indefinoitley
 *    The frame rate (i.e. how many lines to show per second) is determined
 *    by value of LINES_PER_SEC below
 *
 *
 * Before uploading the sketch to the staff, make sure to change the #define'd
 * values to match your setup:
 *  NUM_PIXELS,  LINES_PER_SEC, IMAGELIST
 *  Finally it is assumed that you have already created the LittleFS filesystem on your
 *  flash memory, and uploaded the file with name IMAGE to the root directory
 */


#include <LittleFS.h>
#include <pov-esp32.h>

//number of pixels in your strip/wand
#define NUM_PIXELS 30
// frame rate
#define LINES_PER_SEC 150.0f
uint32_t interval=1000000/LINES_PER_SEC; //interval between lines of image, in microseconds

// name of image list file. Not that we need to include the leading slash
#define IMAGELIST "/imagelist.txt"

//Data and clock pins -  if NOT  using hardware SPI, change values as needed and uncomment
// #define DATAPIN 2
// #define CLOCKPIN 3


/* Global Variables */

POV staff(NUM_PIXELS);
//if NOT using SPI, use this instead
// POV staff(NUM_PIXELS, DATAPIN, CLOCKPIN)
uint32_t nextImageChange=0;

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
    if (! LittleFS.exists(IMAGELIST)) {
        Serial.println("Error: file doesn't exist");
        while(1) {
            staff.blink(0x8000FF); //blink purple
        }
    }
    // if everything is OK, let's add image from imagelist file to POV staff
    staff.addImageList(IMAGELIST);
    Serial.println("imagelist added");
    staff.blink(0x00FF00); //blink green

    //determine first image change time
    if (staff.currentDuration() == 0) {
        //this happens when no duration was specified in image list file
        nextImageChange = ULONG_MAX; //maximal possible unsigned 32-bit int
    } else {
        nextImageChange=millis()+staff.currentDuration()*1000;
    }
}

void loop(){
    if (millis()>nextImageChange){
        //time to switch to next image
        staff.nextImage();
        Serial.println("New image");
        //determine when we will need to change the image again 
        if (staff.currentDuration() == 0) {
            nextImageChange = ULONG_MAX; //maximal possible unsigned 32-bit int
        } else {
            nextImageChange=millis()+staff.currentDuration()*1000;
        }
    }
    //check if it time to show next line
    if (staff.timeSinceUpdate()>interval) {
        staff.showNextLine();
    }
}
