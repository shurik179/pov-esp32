# General info
Arduino library for Persistence of Vision (POV) projects on ESP32-based MCUs. It supports reading image files in bitmap format and showing them, one line at a time, on a strip of addressable LEDs.  DotStar (APA102) individually addressable LED strips; NeoPixels (WS2812B) are not supported.

## Changes from earlier version

This is a fork of my own pov-library (https://github.com/shurik179/pov-library) with the following changes:

* uses LittleFS filesystem instead of FatFS

* uses Adafruit's DotStar library instead of FastLED (variety of reasons,
  including problems with hardware SPI support)


## Image format

The library is capable of showing images at the root of LittleFS system on the
MCU. It doesn't contain any tools for formatting the filesystem or uploading
the images - that must be done seprately. Below are the image requirements.

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
Experiment with colors to get some feel for it.


## Imagelist format

Library also supports reading the list of images to be shown from an image list file. This file  must contain the
 list of image files in the order you want
to use them in your show, one filename per line, including `.bmp` extension and leading slash. 
Optionally, you can also add how long the image should be shown, in seconds
(whole numbers only!), separated from  filename by one or more spaces
```
/image1.bmp 20
/image2.bmp 41
/image5.bmp 10
/image1.bmp
```
You can include some image file more than once, or not at all - it is your
choice.

## API


The library defines class *POV* which describes your POV device (LED strip,
staff, poi,...). It provides the following methods:


* `POV(uint16_t length, uint8_t DATAPIN = 255, uint8_t CLOCKPIN = 255)`: initializer.  
   Parameter `length` is number of pixels, optional parameters `DATAPIN`
   and `CLOCKPIN` are data and clockpin. If you are using hardware SPI, omit
   them, using the form `POV(length)`.

* `void begin()`: start the POV object

### Low-level pixel manipulation

* `void blank()`: clear the LED strip, setting all pixels off

* `void setBrightness(uint8_t b)`: set the brightness for all LEDs. Brightness
  ranges from 0 to 255.  


* `void setPixel(uint16_t i, uint32_t c)`: set pixel with index `i` to color `c`.
  As usual, these values are not immediately pushed to LED strip: after setting
  colors of individual pixels, you need to call `show()` function.

* `void show()`: push the pixel colors, set by `setPixel()` commands, to LED strip.

* `void blink(uint32_t color=0xFF0000)`: quickly blink each 8th LED using specified
  color, for "I am alive" indication. By default, it uses red color, but you can
  specify any  color.

* `void showValue(float v)`: show a value (0-1.0), using the LED strip as bar graph.
  Uses color gradient from red to green.

* `void showLine(byte * line, uint16_t size)`: Shows one line on LED strip. `line` should be
  pointer to array which holds  pixel colors: **3** bytes per pixel, in BGR order.
  Note: it is 3 bytes, not 4!!!  `size` should be size of array (number of pixels, not number of bytes).
  If array size is less than number of pixels on LED strip, the remaining pixels are blanked.

### Working with images and image lists

The POV object at all times maintains a list of images to be shown together
with durations (in seconds). You can add images present on LittleFS filesystem to this
list, move to next image, etc. It also keeps track of current line in the
image currently shown. These operations do not change the image files present
in the filesystem - they just modify the **list** of images to be shown.

* `void clearImageList()`:
  Clear image list, removing all images from it.

* `void addImage(char * filename, uint16_t duration=0)`:
  Read  image from file  and add it to  the end of the  imagelist, making it current
  image. Duration must be in seconds (whole number). By default, duration is 0,
  which indicates unlimited duration.

* `uint8_t addImageList(char * filename)`: read list of images from imagelist
  file and add it to the end of POVstaff  image list. Imagelist file should
  be in the format described above.

* `void nextImage()`: move to the next image, making it current. If reached
  the end of image list, continue from first image. Note: it doesn't start
  showing lines for the image yet - use `showNextLine()` for that.

* `void firstImage()`: go to the beginning of the list, making the first image
  current.  Note: it doesn't start
  showing lines for the image yet - use `showNextLine()` for that.


* `BMPimage * currentImage()`: returns pointer to current image as `BMPimage`
  object. `BMPimage` class holds properties of the image (filename, image  
  dimensions, etc); it is described in `bmpimage.h` source file.

* `uint16_t currentDuration()`: returns duration specified for current image in the
  imagelist. If no duration has been specified, returns 0.





### Showing images

* `int16_t showNextLine()`:  Show next horizontal line of current image in the  imagelist.
  Returns the index of next line to be shown  (after showing given line)
  If it returns 0, it means we have completed showing the image and
  the next call will start showing it again.   If image width  is less than
  the number of pixels on LED strip, the remaining pixels are blanked.



* `void restartImage()`: "rewinds" the current image to line 0, so that next
  `showNextLine()` command would show line 0.



* `uint32_t timeSinceUpdate()`: time since strip was last updated, in microseconds.
