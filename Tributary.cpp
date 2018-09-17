//
//  tributary.cpp
//  ledInstall
//
//  Created by Jenna deBoisblanc on 6/30/17.
//
//

#include <stdio.h>
#include "Tributary.h"

Tributary::Tributary(){}

Tributary::Tributary(int i, int k, ofPolyline p, float numInStrip, int dir){
    id = i;
    originalOrder = k;
    repeat = 50;
    speed = 5;
    packetSize = 40;
    pulseIndex = 0;
    group = 0;
    setup(p, numInStrip);
    lastChecked = ofGetElapsedTimeMillis();
    glitchColor = ofColor(0, 0, 0);
    transitioning = false;
    direction = dir;
    counter = 0;
}

void Tributary::setup(ofPolyline p, float numInStrip) {
    //////////////
    // make sure there are the correct number of pixels on the length of the tributary
    len = numInStrip;
    int diodesPerPixel = 7;
    pixelsOnLine = len;
    diodesOnLine = pixelsOnLine * diodesPerPixel;
    totalPoints = diodesOnLine;
    
    
    for (int j = 0; j < totalPoints; j+=7){
        vector<ofPoint> diodes;
        for (int i = 0; i < 7; i++) {
            // add a point based on the index/ percent of the way through/
            ofPoint vert = p.getPointAtPercent(1.0*(j+i)/(totalPoints-1));
            vert.set(vert.x*2, vert.y*2);
            diodes.push_back(vert);
        }
        Pixel p = Pixel(diodes);
        pixels.push_back(p);
    }
    
    //////////////
    
}

void Tributary::addTributaryEnd(Tributary t) {
    len += t.len;
    totalPoints = int(len * .56);
    
    for (int i = 0; i < t.pixels.size(); i++) {
        pixels.push_back(t.pixels[i]);
    }
}

void Tributary::setGroup(int g) {
    group = g;
}


void Tributary::setRandomPacket(int ps, int separation){
    repeat = packetSize+separation;
    packetSize = ps;
    pulseIndex = int(ofRandom(repeat)+1);
}

void Tributary::update(){

}

void Tributary::updatePacket(){
    if (ofGetElapsedTimeMillis() - counter > 100) {
        counter = ofGetElapsedTimeMillis();
        pulseIndex += direction;
    }
//    if (counter == INT_MAX) counter = 0;
//    if (counter %3 == 0)
    
//    pulseIndex++;
//    if (pulseIndex > pixels.size()) pulseIndex = -pixels.size();
    
}

void Tributary::updatePacket(int speed){
    if (ofGetElapsedTimeMillis() - counter > speed) {
        counter = ofGetElapsedTimeMillis();
        pulseIndex += direction;
    }
    //    if (counter == INT_MAX) counter = 0;
    //    if (counter %3 == 0)
    
    //    pulseIndex++;
    //    if (pulseIndex > pixels.size()) pulseIndex = -pixels.size();
    
}


void Tributary::draw(){
    for( int j = 0; j < pixels.size(); j++ ) {
        pixels[j].draw(ofColor::fromHsb(255*(id*1.0/42), 255*(j*1.0/pixels.size()), 255));
    }
}

void Tributary::draw(ofColor c){
    for( int j = 0; j < pixels.size(); j++ ) {
        pixels[j].draw(c);
    }
}

void Tributary::drawGradient(ofColor start, ofColor end) {
    for (int j = 0; j < pixels.size(); j++) {
        pixels[j].draw(start.lerp(end, j*1.0/pixels.size()));
    }
}

void Tributary::setGradientTransition() {
    transitioning = true;
    lastOffset = 0;
    iterateNum = 0;
    startOffset = 0;
}

void Tributary::updateGradientPulse() {
    offset+=direction;
    if (offset < 0) offset = pixels.size() -1;
}

void Tributary::pulseGradient(int num, ofColor prevGradients[], ofColor gradients[]) {
    updateGradientPulse();
    for (int j = 0; j < pixels.size(); j++) {
        pixels[j].draw(getWrapGradient(j - (offset%pixels.size()), pixels.size(), num, gradients));
    }
}



ofColor Tributary::getWrapGradient(int ind, int totalInd, int num, ofColor gradients[]) {
    if (ind > totalInd) ind -= totalInd;
    else if (ind < 0) ind += totalInd;
    for (int i = 0; i < num; i++) {
        if (ind < totalInd/ (num*1.0) * (i+1)) {
            float pos = ofMap(ind, i * totalInd / (num*1.0), (i+1) * totalInd / (num*1.0), 0, 1.0);
            if (i == num - 1) {
                ofColor c = gradients[i];
                return c.lerp(gradients[0], pos);
            }
            else {
                ofColor c = gradients[i];
                return c.lerp(gradients[i+1], pos);
            }
        }
    }
    return ofColor(0);
    
}

ofColor Tributary::getTransitionGradient(int ind, int num,  ofColor prevGradients[], ofColor gradients[]) {
    if (ind > startOffset) {
        return getWrapGradient(ind - (offset%pixels.size()), pixels.size(), num, prevGradients);
        //return ofColor(0, 0, 255);
    }
    else {
        //return ofColor(255, 0, 0);
        return getWrapGradient(ind - (offset%pixels.size()), pixels.size(), num, gradients);
    }
}


void Tributary::drawPacket(ofColor gradients[]){
    ofColor c;
    for( int j = 0; j < pixels.size(); j++ ) {
        if (inPulse(j)) c = getPacketGradientColor(j, gradients);
        else c = ofColor(255, 0, 0); //ofColor(ofColor::fromHsb(255*(id*1.0/42), 255*(j*1.0/pixels.size()), 255));
        pixels[j].draw(c);
    }
}

ofColor Tributary::getPacketGradientColor(int j, ofColor gradients[]) {
    int n = (j-pulseIndex)%repeat;
    return getWrapGradient(n, packetSize, 4, gradients) ;
}

bool Tributary::inPulse(int j) {
//    int n = (j-pulseIndex)%repeat;
//    return (n >= 0 && n < packetSize);
    return true;
}



void Tributary::drawGroup() {
    for (int j = 0; j < pixels.size(); j++) {
        pixels[j].draw(ofColor::fromHsb((group*10) % 255, 255, 255));
    }
}


void Tributary::drawLabel() {
    drawLabel(pixels[pixels.size()-1].diodes[0]);
}

void Tributary::drawLabel(ofPoint vert) {
    ofSetColor(0);
    // ofDrawBitmapString(originalOrder, vert);
    ofDrawBitmapString(id + 1, vert);
    
    //ofDrawBitmapString(pixels.size(), vert);
    //ofDrawBitmapString(len*.39, vert);
}
