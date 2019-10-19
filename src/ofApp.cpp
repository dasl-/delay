#include "ofApp.h"
void ofApp::setup(){
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
    
    nDelayFrames = 30; //Set buffer size
    buffer.setup(nDelayFrames);
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
    if (!buffer.isFull()) {
        ofDrawBitmapStringHighlight("Populating buffers...", 20, 20);
        return;
    }
    
    ofSetColor(255, 255, 255, 255);
    vidGrabber.draw(vidGrabber.getWidth(), 0, -vidGrabber.getWidth(), vidGrabber.getHeight());

    ofSetColor(255, 255, 255, 255/2);
    int ind = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, nDelayFrames-1, true);
    tex.loadData(buffer.getDelayedPixels(ind));
    tex.draw(vidGrabber.getWidth(), 0, -vidGrabber.getWidth(), vidGrabber.getHeight());

    ofSetColor(255,255,255,255);
    vidGrabber.draw(vidGrabber.getWidth(), camHeight+20, -vidGrabber.getWidth(), vidGrabber.getHeight());
    
    ofDrawBitmapStringHighlight("num delay frames: " + ofToString(ind), 20, camHeight+15);
}
