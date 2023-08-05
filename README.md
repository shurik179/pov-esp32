# POV Library
Arduino library for Persistence of Vision (POV) projects on ESP32-based MCUs
This is a fork of my own pov-librbary (https://github.com/shurik179/pov-library) with the following changes:

* uses LittleFS filesystem instead of FatFS

* uses Adafruit's DotStar library instead of FastLED (variety of reasons, including problems with hardware SPI support)

This is currently in early stages of development, not ready for public use.

Below is description of original library.


This library is for creating Persistence of Vision displays using NeoPixel (WS2812B)
or DotStar (APA102) individually addressable LED strips. For smooth effects,
DotStars are strongly preferred, as they have much higher refresh rates.

It supports the following operations:

* reading a bitmap image file from filesystem and showing it (sending to LED strip)
  one line at a time

* reading a text file with a list of image files, and showing these files, one after another, one line at a time

There is no support in this library for creating/adding image files - that has to be done separately.

## Supported boards

This library should work with all flavors of ESP32 boards (orgiginal ESP32, ESP32-S2, ...)

## Image format

* Images must be in bitmap (BMP) format, with 24 bit color depth. Use any
  software you like to convert images in other formats to bmp - e.g. you can use
  Microsoft Paint 3D.

* images are shown one **horizontal** line at a time. Thus, image width must
  match the number of LEDs on your strip. If necessary, rotate the image in software.

* Maximal image size is 21000 pixels (e.g., it is enough for  72x288 or a 144x144
  pixel image).

* Image filenames must be at most 30 symbols long and can only contain letters,
  numbers, dashes and underscores. No spaces or special symbols!

You can create your own images or search for existing ones.  A good source for
POV image patterns is [Visual  POI Zone](https://visualpoi.zone/patterns/).
You will  need to rotate images 90 degrees.


Please note that the same color (i.e. the same RGB values) can look quite
different on your computer monitor  and on LED strip.


## Imagelist format

Library allows you to add individual images to the list of images to be shown,
or do the batch add from an image list file. This file  must contain the
 list of image files in the order you want
to use them in your show, one filename per line, including `.bmp` extension.
Optionally, you can also add how long the image should be shown, in seconds
(whole numbers only!), separated from  filename by one or more spaces
```
image1.bmp 20
image2.bmp 41
image5.bmp 10
image1.bmp
```
You can include some image file more than once, or not at all - it is your
choice.

## API

Work in Progress 
