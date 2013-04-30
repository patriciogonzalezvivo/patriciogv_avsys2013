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

Line::Line(vector<TimePoint> _line, float _startTime, float _stroke){
    player      = NULL;
    
    startTime   = _startTime;
    stroke      = _stroke;
    points      = _line;
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    for (int i = 0 ; i < _line.size(); i++){

        points.push_back(_line[i]);
        mesh.addVertex(_line[i]);
    }
    
    bSelected   = false;
    bFinish     = true;
    
    updateBox();
}

void Line::addPoint(ofPoint _pos){
    
    float time;
    if ( player != NULL){
        if (player->getIsPlaying()){
            time = player->getPositionMS()*0.001;
        } else {
            time = (player->getPosition()*player->getLength()) + (ofGetElapsedTimef()- player->getLastTime());
        }
    }
    
    if ( (startTime == -1) || (points.size() == 0) ){
        startTime = time;
    } 
    
    TimePoint newPoint;
    newPoint.set(_pos);
    newPoint.time = time - startTime;
    
    mesh.addVertex( _pos );
    points.push_back( newPoint );
    
    updateBox();
}

int Line::size(){
    return points.size();
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
        float linePos = getStartTime()/player->getLength();
        float lineLenght = getDuration()/player->getLength();
        
        referenceBox.x = linePos*(float)ofGetWidth();
        referenceBox.width = lineLenght*(float)ofGetWidth();
        
        referenceBox.height = REFERENCEBOX_HEIGHT;
        referenceBox.y = (ofGetHeight()-player->height) +
                          ofMap(stroke,
                                0,MAX_LINE_WIDTH,
                                player->height*0.9-referenceBox.height,player->height*0.1, true);
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
                           MAX_LINE_WIDTH,0, true);
            
            updateBox();
        }
    }
}


void Line::draw(float _time,bool _NOGL ){
    if ( points.size() > 0 ){
        ofPushStyle();
        ofSetLineWidth(stroke);
    
        if (_NOGL){
            float relativeTime = _time - startTime;
            
            ofBeginShape();
            for (int i = 0 ; i < points.size() - 1 ; i++){
                
                if ( relativeTime > points[i].time ){
                    ofVertex(points[i]);
                } else if (relativeTime >= points[i].time && relativeTime <= points[i+1].time){
                    float part = relativeTime - points[i].time;
                    float whole = points[i+1].time - points[i].time;
                    float pct = part / whole;
                    ofPoint pos;
                    // figure out where we are between a and b
                    pos.x = (1-pct) * points[i].x + (pct) * points[i+1].x;
                    pos.y = (1-pct) * points[i].y + (pct) * points[i+1].y;
                    ofVertex( pos );
                } else {
                    break;
                }
            }
            ofEndShape(false);

        } else if (!isFinish() || _time == 0.0 || _time > startTime+getDuration() || bSelected ){
            mesh.draw();
        } else if ( _time <= startTime+getDuration() ){
            float relativeTime = _time - startTime;
            ofMesh lineMesh;
            lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            for (int i = 0 ; i < points.size() - 1 ; i++){
                if ( relativeTime > points[i].time ){
                    lineMesh.addVertex(points[i]);
                } else if (relativeTime >= points[i].time && relativeTime <= points[i+1].time){
                    float part = relativeTime - points[i].time;
                    float whole = points[i+1].time - points[i].time;
                    float pct = part / whole;
                    ofPoint pos;
                    // figure out where we are between a and b
                    pos.x = (1-pct) * points[i].x + (pct) * points[i+1].x;
                    pos.y = (1-pct) * points[i].y + (pct) * points[i+1].y;
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
            if (!isFinish()) ofSetColor(255,0,0,200);
            else ofSetColor(ofGetStyle().color,50);
            ofRect(referenceBox);
        }
        
        ofPopStyle();
    }
}