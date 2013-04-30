//
//  VisualPlayer.cpp
//  drawingTool
//
//  Created by Patricio Gonzalez Vivo on 4/27/13.
//
//

#include "VisualPlayer.h"

VisualPlayer::VisualPlayer(){
    height = TIMELINE_HEIGHT;
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
    bookmarks.clear();
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

float VisualPlayer::getLength(){
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

bool ascendSort(float i,float j) { return (i<j); }

void VisualPlayer::addBookmark(float _pct){
    
    if ( _pct == -1.0)
        _pct = getPosition();
    
    bookmarks.push_back(_pct);
    
    sort(bookmarks.begin(), bookmarks.end(), ascendSort);
}

void VisualPlayer::draw(){
    
    ofPushStyle();
    ofPushMatrix();
    
    //  Box
    //
    ofSetColor(255,100);
    ofTranslate(0, ofGetHeight()-height);
    ofLine(0,0,ofGetWidth(),0);
    
    //  Header
    //
    ofSetColor(255, 0,0);
    drawMark(getPosition(),true,false);
    
    //  Bookmarks
    //
    ofSetColor(255,0,0,100);
    for(int i = 0; i < bookmarks.size(); i++){
        drawMark(bookmarks[i],false,true);
    }
    
    ofPopMatrix();
    ofPopStyle();
}

void VisualPlayer::drawMark(float _pct, bool _showSec, bool _showArrow){
    float pos = _pct * (float)ofGetWidth();
    float size = 0;
    if (_showArrow){
        size = MARK_SIZE;
        ofBeginShape();
        ofVertex(pos-size,0);
        ofVertex(pos+size,0);
        ofVertex(pos,size);
        ofEndShape();
    }
    ofLine(pos, size, pos, height);
    
    if (_showSec)
        ofDrawBitmapString( ofToString( _pct * lenght ,3,7,'0'), pos-27,-3) ;
}