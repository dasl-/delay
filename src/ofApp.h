#pragma once
#include "ofMain.h"
#include "ofxPostProcessing.h"

class circularPixelBuffer{
public:
    circularPixelBuffer(){
        currentIndex = 0;
    }
    void setup(int numFrames){
        frames.resize(numFrames);
        currentIndex = 0;
    }
    void pushPixels(ofPixels& pix){
        currentIndex--;
        if (currentIndex < 0) {
            currentIndex = frames.size() - 1;
        }
        frames[currentIndex] = pix;
        if (currentIndex == 0) {
            _isFull = true;
        }
    }
    
    ofPixels& getDelayedPixels(size_t delay){
        if(delay < frames.size()){
            return frames[ofWrap(delay + currentIndex, 0, frames.size())];
        }
        return frames[0];
    }
    
    bool isFull() {
        return _isFull;
    }
    
private:
    int currentIndex;
    vector<ofPixels> frames;
    bool _isFull = false;
};

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    
private:
    ofVideoGrabber vidGrabber;
    int camWidth;
    int camHeight;
    
    int nDelayFrames;
    ofTexture tex;
    
    circularPixelBuffer buffer;
    
};
