#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
    ofEnableSmoothing();
    
    //  This is the center position of the diagram
    //
    center.set(250,250);
    ofSetWindowShape(1024, 540);
    
    angle   = 0.0f;
    radius  = 140.0f;
    frequency = 1.0f;
    
}

//--------------------------------------------------------------
void testApp::update(){

    float phaseAdder = (float)(frequency * TWO_PI) / (float)ofGetFrameRate();
    angle += phaseAdder;
        
    //  Just for making it clear angle could be more than the 2PI
    //
    while (angle > PI*2)
        angle -= PI*2;
    
    //  Get the cartesian 
    //
    dotPos.x = cos(angle) * radius;
    dotPos.y = sin(angle) * radius;
    
    //  Store the Y (sin) position
    //
    sinHistory.push_back(dotPos.y);

    if (sinHistory.size() > 400){
        sinHistory.erase(sinHistory.begin());
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    ofDrawBitmapString("Freq = " + ofToString(frequency) + "Hz", 15,15);
    
    ofPushMatrix();
    ofTranslate(center);
    
    //  Draw Cartesian
    //
    float lenghtOfAxis = 200;
    ofSetColor(100);
    
    ofLine(ofPoint(-lenghtOfAxis,0),
           ofPoint(lenghtOfAxis,0));
    ofDrawBitmapString("-X", -lenghtOfAxis - 25, 5);
    ofDrawBitmapString("X", lenghtOfAxis + 5, 5);
    ofLine(ofPoint(0,-lenghtOfAxis),
           ofPoint(0,lenghtOfAxis));
    ofDrawBitmapString("-Y", -11, -lenghtOfAxis - 5);
    ofDrawBitmapString("Y", -3, lenghtOfAxis + 15);

    //  Draw Vector
    //
    ofSetColor(255);
    ofLine(0,0, dotPos.x, dotPos.y);
    
    //  Draw Vector projections on axis
    //
    ofSetColor(255);
    ofLine(0,0,dotPos.x,0); // X
    ofLine(0,0,0,dotPos.y); // Y
    ofSetColor(255,50);
    ofLine(dotPos.x,dotPos.y,dotPos.x,0); // X
    ofLine(dotPos.x,dotPos.y,0,dotPos.y); // Y
    
    //  Draw Dot
    //
    drawDot( dotPos );
    
    //  Draw angle References
    //
    ofPolyline  angleLine;
    angleLine.arc(0, 0, radius*0.3, radius*0.3, 0, ofRadToDeg(angle));
    ofNoFill();
    ofSetColor(255,180);
    angleLine.draw();
    
    //  Draw sin and cos proyected dots
    //
    ofSetColor(255,100);
    ofLine(lenghtOfAxis + 30,-radius, lenghtOfAxis + 30,radius);
    ofDrawBitmapString(ofToString(-radius), lenghtOfAxis + 30 - 12,-radius -5);
    ofDrawBitmapString(ofToString(radius), lenghtOfAxis + 30 - 3, radius + 15 );
    ofPoint sinPos = ofPoint(lenghtOfAxis + 30, dotPos.y);
    drawDot(sinPos);
    
    //  Draw the history of sin throught time
    //
    ofMesh lineMesh;
    lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    for (int i = sinHistory.size() -1 ; i > 0 ; i--){
        float alpha = ofMap(i+1, 1,sinHistory.size(), 0.0,1.0);
        lineMesh.addColor( ofFloatColor(1.0, alpha) );
        lineMesh.addVertex( ofPoint(lenghtOfAxis + 30 + (sinHistory.size()-i), sinHistory[i]) );
    }
    
    ofSetColor(255);
    lineMesh.draw();
    
    
    ofPopMatrix();
}

void testApp::drawDot(ofPoint _pos){
    ofPushStyle();
    
    ofFill();
    ofSetColor(255, 0,0);
    ofCircle(_pos, 5);
    
    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == OF_KEY_LEFT){
        frequency -= 0.1;
    } else if (key == OF_KEY_RIGHT){
        frequency += 0.1;
    } 
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    ofPoint mouse = ofPoint(x,y) - center;
    radius = mouse.length();
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}