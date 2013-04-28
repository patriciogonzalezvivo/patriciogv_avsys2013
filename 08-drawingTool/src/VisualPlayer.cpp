//
//  VisualPlayer.cpp
//  drawingTool
//
//  Created by Patricio Gonzalez Vivo on 4/27/13.
//
//

#include "VisualPlayer.h"

VisualPlayer::VisualPlayer(){
    height = 60;
    bPlay = false;
    lastTime = -1;
}

bool VisualPlayer::loadSound(string _file){
    bool _loaded = ofSoundPlayer::loadSound(_file, false);

    ofSoundPlayer::play();
    setPosition(0.999999);
    lenght = getPositionMS()*0.001;
    setPosition(0.0);
    ofSoundPlayer::stop();
    pct = 0.0;
    bPlay = false;
    
    return _loaded;
}

void VisualPlayer::play(){
    ofSoundPlayer::play();
    ofSoundPlayer::setPosition(pct);
    bPlay = true;
    lastTime = -1.0;
}

void VisualPlayer::pause(){
    lastTime = ofGetElapsedTimef();
    pct = getPosition();
    ofSoundPlayer::stop();
    setPosition(pct);
    bPlay = false;
}

float VisualPlayer::getPosition(){
    if (bPlay)
        pct = ofSoundPlayer::getPosition();
    
    return pct;
}

float VisualPlayer::getLenght(){
    return lenght;
}

float VisualPlayer::getLastTime(){
    return lastTime;
}

bool VisualPlayer::inside(int _x, int _y){
    if ( _y > ofGetHeight()-height ){
        return true;
    } else {
        return false;
    }
}

void VisualPlayer::draw(){
    ofPushStyle();
    ofPushMatrix();
    
    //  Box
    //
    ofNoFill();
    ofSetColor(255,100);
    ofTranslate(0, ofGetHeight()-height);
    
    ofRect(0,0,ofGetWidth(),height);
    
    //  Header
    //
    ofSetColor(255, 0,0);
    float headPos = getPosition() * (float)ofGetWidth();
    ofLine(headPos, 0, headPos, height);
    
    ofDrawBitmapString( ofToString(lenght * getPosition(),3,7,'0'), headPos-27,-3) ;
    
    ofPopMatrix();
    ofPopStyle();
}