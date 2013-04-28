//
//  Line.cpp
//  week3-multiLine
//
//  Created by Patricio Gonzalez Vivo on 9/28/12.
//
//

#include "Line.h"

Line::Line(){
    
    bSelected   = false;
    bFinish     = false;
    player      = NULL;
    
    stroke      = 0.5;
    startTime   = -1;
    
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
}

void Line::addPoint(ofPoint _pos){
    
    float time;
    if ( player != NULL){
        if (player->getIsPlaying()){
            time = player->getPositionMS()*0.001;
        } else {
            time = (player->getPosition()*player->getLenght()) + (ofGetElapsedTimef()- player->getLastTime());
        }
    }
    
    if ( (startTime == -1) || (points.size() == 0) ){
        startTime = time;
    } 
    
    TimePoint newPoint;
    newPoint.set(_pos);
    newPoint.time = time - startTime;
    
//    mesh.addColor(1.0);
    mesh.addVertex( _pos );
    points.push_back( newPoint );
    
    updateBox();
}

int Line::size(){
    points.size();
}

void Line::clear(){
    mesh.clear();
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
    updateBox();
}

void Line::setVisualPlayer( VisualPlayer *_player ){
    player = _player;
}

ofPoint	 Line::getPositionForTime( float _time ){
	
	// are we recording?
	if ( !isFinish() ){
		return ofPoint(0,0,0);
	}
	
	// now, let's figure out where we are in the drawing...
	ofPoint pos;
	for (int i = 0; i < points.size()-1; i++){
		if (_time >= points[i].time && _time <= points[i+1].time){
			
			// calculate pct:
			float part = _time - points[i].time;
			float whole = points[i+1].time - points[i].time;
			float pct = part / whole;
			
			// figure out where we are between a and b
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
	
	ofPoint prevPt = getPositionForTime( MAX(_time - 0.09f, 0));		// check for where we were 0.05 seconds ago
	ofPoint currPt = getPositionForTime(_time);							// check for where we are now.
	
	ofPoint diff;
	diff.x = currPt.x - prevPt.x;
	diff.y = currPt.y - prevPt.y;
	
	return diff;
}

void Line::updateBox(){
    if ( player != NULL){
        float linePos = getStartTime()/player->getLenght();
        float lineLenght = getDuration()/player->getLenght();
        
        referenceBox.x = linePos*(float)ofGetWidth();
        referenceBox.width = lineLenght*(float)ofGetWidth();
        
        referenceBox.height = 10;
        referenceBox.y = (ofGetHeight()-player->height) +
                          ofMap(stroke,
                                0,MAX_LINE_WIDTH,
                                (player->height-referenceBox.height)-referenceBox.height*0.5,player->height*0.1, true);
    }
}

bool Line::checkOver(int _x, int _y){
    return referenceBox.inside(_x, _y);
}

void Line::moveRefTo( float _x, float _y ){
    
    if ( player != NULL){
        if ( player->inside(_x, _y)){
            float time = _x/(float)ofGetWidth();
            time *= player->getLenght();
            startTime = time;
            
            float st = _y - (ofGetHeight()-player->height);
            stroke = ofMap(st,
                           player->height*0.1, player->height-referenceBox.height-referenceBox.height*0.5,
                           MAX_LINE_WIDTH, 0, true);
            
            updateBox();
        }
    }
}


void Line::draw(float _time){
    if ( points.size() > 0 ){
        
        ofPushStyle();
        ofSetLineWidth(stroke);
    
        if (!isFinish() || _time == 0.0 || _time > startTime+getDuration() ){
            mesh.draw();
        } else {
            float relativeTime = _time - startTime;
            
            ofMesh lineMesh;
            lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
        
            for (int i = 0 ; i < points.size() - 1 ; i++){
                
                if ( relativeTime > points[i].time ){
//                    lineMesh.addColor(1.0);
                    lineMesh.addVertex(points[i]);
                } else if (relativeTime >= points[i].time && relativeTime <= points[i+1].time){
                    float part = relativeTime - points[i].time;
                    float whole = points[i+1].time - points[i].time;
                    float pct = part / whole;
                    ofPoint pos;
                    // figure out where we are between a and b
                    pos.x = (1-pct) * points[i].x + (pct) * points[i+1].x;
                    pos.y = (1-pct) * points[i].y + (pct) * points[i+1].y;
//                    lineMesh.addColor(1.0);
                    lineMesh.addVertex( pos );
                } else {
                    break;
                }
            }
        
            lineMesh.draw();
        }
        
        //  Draw reference box
        //
        if ( player != NULL){
            ofFill();
            ofSetColor(ofGetStyle().color,50);
            ofRect(referenceBox);
        }
        
        ofPopStyle();
    }
}