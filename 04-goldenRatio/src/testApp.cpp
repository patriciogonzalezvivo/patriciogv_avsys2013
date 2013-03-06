#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetCircleResolution(60);
    ofSetDrawBitmapMode(OF_BITMAPMODE_SIMPLE );
    
    phi = (1.0+sqrt(5.0))/2.0;
    cout << "This is fye the Greater "<< phi << endl;
    cout << "This is fee the Lesser "<< 1.0/phi << endl;
    
    center.x = ofGetWidth()*0.5;
    center.y = ofGetHeight()*0.5;
}

//--------------------------------------------------------------
void testApp::update(){
    mouse += ( ofPoint(mouseX,mouseY) - mouse)*0.1;

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    //  Line
    //
    ofSetColor(255);
    ofCircle(center, 2);
    ofLine(center,mouse);
    
    //  Mouse to Polar
    //
    ofPoint dist = mouse - center ;
    float radian = atan2(dist.y, dist.x);
    float radius = dist.length();
    
    ofPushMatrix();
    ofTranslate(center);
    
    //  1D:  A & B Points
    //
    float a = dist.length()/phi;
    ofPoint A = ofPoint(a*cos(radian),a*sin(radian));
    ofCircle(A, 2);
    
    float b = dist.length()/pow(phi,2);
    ofPoint B = ofPoint(b*cos(radian),b*sin(radian));
    ofCircle(A+B, 2);
    
    //  2D: Make it a PLANE
    //
    ofPushMatrix();
    ofPushStyle();
    
    ofNoFill();
    
    ofRotate( ofRadToDeg(radian), 0.0, 0.0, 1.0);
    ofSetColor(255);
    ofDrawBitmapString("A", a*0.5, -5);
    ofDrawBitmapString("A", -15, a*0.5);
    ofSetColor(100);
    ofRect(0,0,a,a);
    ofSetColor(255);
    ofDrawBitmapString("B", a+b*0.5, -5);
    ofSetColor(100);
    ofRect(a,0,b,a);
    
    //
    //  Found ARC
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(a*0.5, a);
    ofSetColor(50);
    ofCircle(0,0, a*0.5+b);
    ofLine(0,0,a*0.5,-a);
    ofPopStyle();
    ofPopMatrix();
    
    
    ofPopStyle();
    ofPopMatrix();
    
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    center.x = w*0.5;
    center.y = h*0.5;
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}