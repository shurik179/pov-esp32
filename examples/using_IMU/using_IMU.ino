/*
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
 * hardcoded in the code (IMAGE).
 * Note that it assumes that you have already created LittleFS
 * filesystem on your MCU and uploaded the file with name IMAGE to the root directory
 *
 * It uses the IMU to detect if the staff is at rest (horizontal, not moving). If it is,
 * staff is blanked. Otherwise, it will show the selected image; the frame rate
 * (i.e. how many lines to show per second) is adjusted depending on the rotation speed,
 * trying to keep number of lines per degree of rotation constant.
 *
 * The current example assumes that your IMU is an STmicroelectronics LSM6DS33 or
 * LSM6DSO, connected via I2C bus Wire; we use a modified Pololu LSM6 library code
 * to access the IMU. For other IMU types, this code should be changed, and an
 * appropriate library used.

 */

 #include "LSM6.h"
 #include <LittleFS.h>
 #include <pov-esp32.h>
 //number of pixels in your strip/wand
 #define NUM_PIXELS 30

 // frame rate. Instead of using constant frame rate per second, we will adjust
 // depending on rotation speed

 // how many degrees of staff turn between successive lines?
 #define DEG_PER_LINE 1.0f

 #define IMAGE "/rg-lines.bmp"

 LSM6 imu;
 POV staff(NUM_PIXELS);
 uint32_t lastIMUcheck = 0; //when did we last check IMU speed, in ms
 float speed=0.0;           //staff rotation speed, in deg/s

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

     staff.addImage(IMAGE);
     //now, start and initialize IMU
     Wire.begin();
     if (!imu.init()) {
         Serial.println("Failed to detect and initialize IMU!");
         while (1) {
             staff.blink(0xFFFF00);
       }
     }
     Serial.println("IMU Enabled");
     imu.enableDefault();
 }

void loop(){
    if (millis()-lastIMUcheck > 50 ) {
        //let's check if staff is at rest. To avoid overloading the MCU, we only do it 20 times/sec.
        lastIMUcheck = millis();
        imu.read();
        //also, get  rotation speed (in deg/s)
        speed = imu.getSpeed();
        staff.paused = (imu.isHorizontal() && (speed < 30));
        if (staff.paused) staff.blank();
    }
    float rotAngle = speed * staff.timeSinceUpdate() * 0.000001;

    if ( (!staff.paused) && (rotAngle>DEG_PER_LINE)) {
        staff.showNextLine();
    }
}
