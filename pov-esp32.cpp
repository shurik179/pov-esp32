#include "pov-esp32.h"

POV::POV(uint16_t n,  uint8_t datapin, uint8_t clockpin )
    :numPixels(n), paused(false) {
        if (datapin == 255) {
          strip = new Adafruit_DotStar(n, DOTSTAR_BGR);
        } else {
          strip = new Adafruit_DotStar(n,datapin, clockpin, DOTSTAR_BGR);
        }
    }

void POV::begin(){
    //Serial.println("Starting...")

    strip->begin();
    strip->setBrightness(127);
    strip->show();
}

void POV::blank(){
    strip->clear();
    strip->show();
}

void POV::setBrightness(uint8_t b){
    strip->setBrightness(b);
}

void POV::setPixel(uint16_t i, uint32_t c){
    strip->setPixelColor(i,c);
}

void POV::show(){
    strip->show();
    lastLineUpdate=micros();
}
//FIXME
void POV::showValue(float v){
    uint16_t level=0;
    uint8_t i, delta;
    strip->clear();
    if (v<0.0) {
      v=0.0;
    } else if (v>1.0) {
      v=1.0;
    }
    level = v*numPixels;
    if (level>numPixels) level=numPixels;
    //do gradient fill of the strip
    float step=250.0/numPixels;
    for (i=0; i<level; i++){
        delta = i*step;
        strip->setPixelColor(i, 255-delta, delta, 0);
    }

    strip->show();
    lastLineUpdate=micros();
}






void POV::showLine(byte * line, uint16_t size){
    uint16_t i,pos;
    uint8_t r,g,b;
    for (i=0; i<numPixels; i++) {
        if (i<size) {
            pos=3*i;
            //using bgr order
            b=line[pos++];
            g=line[pos++];
            r=line[pos];
            strip->setPixelColor(i,r,g,b);
        } else {
           strip->setPixelColor(i,0x00);
        }
    }
    strip->show();
    lastLineUpdate=micros();
}

void POV::blink(uint32_t color){
    uint16_t i;

    //repeat twice
    for (uint8_t j=0; j<2; j++){
        strip->clear();
        strip->show();
        delay(500);
        for(i=0; 8*i<numPixels; i++) {
            strip->setPixelColor(8*i,color);
        }
        strip->show();
        delay(500);
    }
    strip->clear();
    strip->show();
}

void POV::addImage(char * filename, uint16_t duration){
    BMPimage * ptr;
    ptr=imageList.addImage(filename,duration);
    imageList.current()->load();
    currentLine=0;
}

uint8_t POV::addImageList(char * filename){
    currentLine=0;
    uint8_t count=imageList.addFromFile(filename);
    if (count) {
        //load first image in buffer
        imageList.current()->load();
    }
    return count;

}

void POV::clearImageList(){
    imageList.reset();
    currentLine=0;
}

void POV::firstImage(){
    imageList.current()->unload();
    imageList.first();
    imageList.current()->load();
    currentLine=0;
}

void POV::nextImage(){
    if (paused) return;
    imageList.current()->unload();
    imageList.next();
    imageList.current()->load();
    currentLine=0;
}

int16_t POV::showNextLine(){
    BMPimage * currentImg=imageList.current();
    if (currentImg==NULL) return 0;
    if (paused) return currentLine;
    //move to next line
    showLine(currentImg->line(currentLine), currentImg->width());
    currentLine++;
    if (currentLine == currentImg->height()) {currentLine=0;}
    return currentLine;
}
