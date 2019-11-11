#include "ofApp.h"
void ofApp::setup(){
    ofDisableArbTex();
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    camWidth = 640;  // try to grab at this size.
    camHeight = 480;
    vidGrabber.setDeviceID(0);
    
    // Might get errors:
    //    [ error ] ofAvFoundationGrabber:  could not set framerate to: 60. Device supports:
    //    [ error ]   framerate range of: 1 to 30
    // But, setting to 30 results in very laggy video capture / playback 50% of the time.
    vidGrabber.setDesiredFrameRate(60);
    vidGrabber.initGrabber(camWidth, camHeight);
    ofSetVerticalSync(true);
    
    maxDelayAmount = 50; //Set buffer size
    buffer.setup(maxDelayAmount);
    
    maxNumDelays = 10;
    
    filters.resize(maxNumDelays + 1);
    for (int i = 0; i < (maxNumDelays + 1); i++) {
        filters[i] = new LookupFilter(camWidth, camHeight, "img/lookup_amatorka_green.png", 1/pow(2,i));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    vidGrabber.update();
    
    if(vidGrabber.isFrameNew()){
        buffer.pushPixels(vidGrabber.getPixels());
    }
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 255, 255);
    ofBackground(0,0,0);
    if (!buffer.isFull()) {
        ofDrawBitmapStringHighlight("Populating buffers...", 20, 20);
        return;
    }
    
    float vidScaleFactor = MIN(ofGetWidth()/vidGrabber.getWidth(), ofGetHeight()/vidGrabber.getHeight());
    int scaledWidth = floor(vidScaleFactor * vidGrabber.getWidth());
    int scaledHeight = floor(vidScaleFactor * vidGrabber.getHeight());
    int xOffset = floor((ofGetWidth() - scaledWidth) / 2);
    int yOffset = floor((ofGetHeight() - scaledHeight) / 2);
    
    ofShader &shader = filters[0]->getShader();
    filters[0]->begin();
    vidGrabber.draw(scaledWidth + xOffset, yOffset, -scaledWidth, scaledHeight);
    filters[0]->end();
    
    int baseDelayAmount = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, maxDelayAmount-1, true);
    int numDelays = ofMap(ofGetMouseY(), 0, ofGetHeight(), maxNumDelays, 0, true);
    for (int i = 0; i < numDelays; i++) {
        int delayAmount = baseDelayAmount* (i+1);
        if (delayAmount > maxDelayAmount) {
            break;
        }
        ofSetColor(255, 255, 255, 255/pow(2,i+1));
        tex.loadData(buffer.getDelayedPixels(delayAmount));
        
        filters[i+1]->begin();
        tex.draw(scaledWidth + xOffset, yOffset, -scaledWidth, scaledHeight);
        filters[i+1]->end();
    }
    
    
    ofDrawBitmapStringHighlight("num delay frames: " + ofToString(baseDelayAmount), 20, 20);
    ofDrawBitmapStringHighlight("num delays: " + ofToString(numDelays), 20, 40);
}
