#ifndef _FILEREAD_H
#define _FILEREAD_H
#include <Arduino.h>
#include <SPI.h>
#include "FS.h"
#include <LittleFS.h>


//general file access functions
//read 16-bit (2 byte) unsigned int from file
uint16_t read16(File &f);
//read 32-bit (4 byte) unsigned int from file
uint32_t read32(File &f);
//read line (until \n ) from file and save it to l
//skips all whitespace (including CR and tab)
int readLine(File &f, char  l[]);

#endif
