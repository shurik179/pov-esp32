#include "fileread.h"
#include "config.h"


/*
 *    General file access functions
 */

uint16_t read16(File &f) {
    uint16_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
}

uint32_t read32(File &f) {
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
}

int readLine(File &f, char  l[]){
    int pos=0;
    char c;
    while (f.available()&&(pos<MAX_LINE_LENGTH)) {
        c=f.read();//read next character
        if (c=='\n') {
            l[pos]=NULL; //terminate string
            return pos;
        }
        if (pos<MAX_LINE_LENGTH) { l[pos++]=c;}
    }
    //if we are here and not yet exited, it means we reached end of file or end of buffer before meeting \n
    l[pos]=NULL;
    return pos;
}
