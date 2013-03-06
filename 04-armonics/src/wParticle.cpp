//
//  Particle.cpp
//  week2_whitney
//
//  Created by Patricio Gonzalez Vivo on 9/20/12.
//
//

#include "wParticle.h"

wParticle::wParticle(){
    
}

void wParticle::moveTo(float _x, float _y){
    ofPoint temp;
    
	x = temp.x = _x;
	y = temp.y = _y;
    
	points.push_back(temp);
    
	while (points.size() > 50){
		points.erase(points.begin());
	}
}

void wParticle::drawDot(){
	
	ofFill();
    
    float alpha = ofMap(abs(ofGetHeight()*0.5-y), 0, ofGetHeight()*0.25, 0.7, 0.0,true);
    ofSetColor(ofFloatColor(1.0,0.0,0.0,alpha));
    ofCircle(x, y, 5);
    
    ofSetColor(255);
    ofCircle(x, y, 1);
}

void wParticle::drawTrail(float _alpha){
	
    ofSetColor(255);
    ofMesh lineMesh;
    lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    for (int i = 0 ; i < points.size() ; i++){
        float alpha = ofMap(i+1, 1,points.size(), 0.0,1.0);
        
        lineMesh.addColor(ofFloatColor(1.0, alpha*_alpha));
        lineMesh.addVertex(points[i]);
    }
    
    ofSetColor(255);
    lineMesh.draw();
}