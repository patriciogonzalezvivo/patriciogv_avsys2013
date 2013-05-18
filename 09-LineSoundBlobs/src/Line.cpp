//
//  Line.cpp
//  week3-multiLine
//
//  Created by Patricio Gonzalez Vivo on 9/28/12.
//
//

#include "Line.h"

float phi = (1.0+sqrtf(5.0))/2.0;

Line::Line(){
    
    bSelected   = false;
    bFinish     = false;
    player      = NULL;
    
    stroke      = 1.0;
    startTime   = -1;
    
    referenceFFT.setMode(OF_PRIMITIVE_LINE_STRIP);
}

Line::Line(vector<TimePoint> _line, float _startTime, float _stroke){
    player      = NULL;
    
    startTime   = _startTime;
    stroke      = _stroke;
    points      = _line;
    bSelected   = false;
    
    setFinish();
    
    referenceFFT.setMode(OF_PRIMITIVE_LINE_STRIP);
    updateBox();
}

void Line::addPoint(ofPoint _pos){
    
    TimePoint newPoint;
    newPoint.set(_pos);
    
    float time;
    if ( player != NULL){
        if (player->getIsPlaying()){
            time = player->getTime();
        } else {
            time = (player->getPosition()*player->getLength()) + (ofGetElapsedTimef()- player->getLastTime());
        }
        if (player->fft != NULL){
//            memcpy ( newPoint.fft, player->fft->averages, sizeof(float)*player->fft->nAverages );
            memcpy ( newPoint.fft, player->fft->peaks, sizeof(float)*player->fft->nAverages );
        }
    } else {
        time = ofGetElapsedTimef();
    }
    
    if ( (startTime == -1) || (points.size() == 0) ){
        startTime = time;
    } 
    newPoint.time = time - startTime;
    
    if (points.size() == 0){
        newPoint.angle = 0;
    } else {
        ofPoint diff = _pos - points[points.size()-1];
        newPoint.angle = atan2(diff.y, diff.x);
    }
    
    points.push_back( newPoint );
    
    updateBox();
}

int Line::size(){
    return points.size();
}

void Line::clear(){
    points.clear();
    bFinish = false;
    
    startTime = -1;
}

TimePoint& Line::operator[](int _index){
    return points[_index] ;
}

float Line::getDuration(){
	float duration = 0;
	if (points.size() > 0){
		duration =  points[points.size()-1].time;
	}
	return duration;
}

float Line::getStartTime(){
    return startTime;
}

ofRectangle Line::getReferenceBox(){
    return referenceBox;
}

bool Line::isFinish(){
    if ( (points.size() > 0) && (startTime != -1) && bFinish ){
        return true;
    } else {
        return false;
    }
}

void Line::setFinish(){
    bFinish = true;
    
    //  Calculate how large it is
    //
    length = 0.0;
    for(int i = 0; i < points.size()-1; i++){
        length += points[i].distance(points[i+1]);
    }
    
    updateBox();
}

void Line::setTimeLine( TimeLine *_player ){
    player = _player;
}

float Line::getIndexForTime( float _time ){
    for (int i = 0; i < points.size()-1; i++){
		if (_time >= points[i].time && _time <= points[i+1].time){
			float part = _time - points[i].time;
			float whole = points[i+1].time - points[i].time;
            
            return i + part / whole;
		}
	}
}

ofPoint	Line::getPositionForTime( float _time ){
	
	// are we recording?
    //
	if ( !isFinish() ){
		return ofPoint(0,0,0);
	}
	
	// now, let's figure out where we are in the drawing...
    //
	ofPoint pos;
	for (int i = 0; i < points.size()-1; i++){
		if (_time >= points[i].time && _time <= points[i+1].time){
			
			// calculate pct:
            //
			float part = _time - points[i].time;
			float whole = points[i+1].time - points[i].time;
			float pct = part / whole;
			
			// figure out where we are between a and b
            //
			pos.x = (1-pct) * points[i].x + (pct) * points[i+1].x;
			pos.y = (1-pct) * points[i].y + (pct) * points[i+1].y;
		}
	}
	
	return pos;
}

ofPoint	 Line::getVelocityForTime( float _time){
	
	// to get the velcoity, look back a bit of time, and at the current time
	// and get the difference
	// veclocity = pos at time 1 - pos at time 0...
	//
	ofPoint prevPt = getPositionForTime( MAX(_time - 0.09f, 0));		// check for where we were 0.05 seconds ago
	ofPoint currPt = getPositionForTime(_time);							// check for where we are now.
	
	ofPoint diff;
	diff.x = currPt.x - prevPt.x;
	diff.y = currPt.y - prevPt.y;
	
	return diff;
}

void Line::updateBox(){
    if ( player != NULL){
        float linePos = getStartTime()/player->getLength();
        float lineLenght = getDuration()/player->getLength();
        
        referenceBox.x = linePos*(float)ofGetWidth();
        referenceBox.width = lineLenght*(float)ofGetWidth();
        
        referenceBox.height = REFERENCEBOX_HEIGHT;
        referenceBox.y = (ofGetHeight()-player->height) +
                          ofMap(stroke,
                                0.5,MAX_LINE_WIDTH,
                                player->height*0.9-referenceBox.height,player->height*0.1, true);
        
        
        octave = (int)ofMap(stroke, 0.5, MAX_LINE_WIDTH,17, 0,true);
        
        referenceFFT.clear();
        for (int i = 0 ; i < points.size(); i++){
            ofPoint pos = ofPoint(referenceBox.x,referenceBox.y+referenceBox.height);
            pos.x += ofMap(i, 0, points.size(), 0, referenceBox.width);
            pos.y += ofMap( points[i].fft[ octave ], 0, 10, -2, -referenceBox.height+4);
            
            referenceFFT.addColor(ofFloatColor(0.0));
            referenceFFT.addVertex(pos);
        }
    }
}

bool Line::checkOver(int _x, int _y){
    return referenceBox.inside(_x, _y);
}

void Line::moveRefTo( float _x, float _y ){
    
    if ( player != NULL){
        if ( player->inside(_x, _y)){
            float time = _x/(float)ofGetWidth();
            time *= player->getLength();
            startTime = time;
            
            float st = _y - (ofGetHeight()-player->height);
            stroke = ofMap(st,
                           player->height*0.1, player->height-referenceBox.height-referenceBox.height*0.5,
                           MAX_LINE_WIDTH,0.5, true);
            
            updateBox();
        }
    }
}


void Line::draw(float _time){
    
    if ( points.size() > 1 ){
        float relativeTime = _time - startTime;
        
        ofPushStyle();
        ofSetLineWidth(stroke);
        ofNoFill();
        ofBeginShape();
        for (int i = 0 ; i < points.size()-1; i++){
            ofPoint pos;
            
            if ( relativeTime > points[i].time ){
                pos = points[i];
            } else if (relativeTime >= points[i].time && relativeTime <= points[i+1].time ){
                float part = relativeTime - points[i].time;
                float whole = points[i+1].time - points[i].time;
                float pct = part / whole;
                
                // figure out where we are between a and b
                //
                pos.x = (1-pct) * points[i].x + (pct) * points[i+1].x;
                pos.y = (1-pct) * points[i].y + (pct) * points[i+1].y;
            } else {
                break;
            }
            
            //  calculate displacement
            //
            float freq = (float)i/(float)points.size();
            float disp = (points[i].fft[octave] - player->fft->averages[octave]) * 5;//sin( freq*TWO_PI );
            pos.x += disp * cos(points[i].angle-PI*0.5) * sin(freq*PI + ofGetElapsedTimef());
            pos.y += disp * sin(points[i].angle-PI*0.5) * sin(freq*PI + ofGetElapsedTimef());
;
            
            ofVertex(pos);
        }
        ofEndShape(false);
        ofPopStyle();

        //  Draw reference box
        //
        if ( player != NULL){
            ofPushStyle();
            
            ofFill();
            if (!isFinish()) ofSetColor(255,0,0,100);
            else ofSetColor(ofGetStyle().color,100);
            ofRect(referenceBox);
            
            referenceFFT.draw();
            
            ofPopStyle();
        }
    }
} 