#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetCircleResolution(60);
    ofSetDrawBitmapMode(OF_BITMAPMODE_SIMPLE );
    
    phi = (1.0+sqrtf(5.0))/2.0;
    cout << "This is fye the Greater "<< phi << endl;
    cout << "This is fee the Lesser "<< 1.0/phi << endl;

}

//--------------------------------------------------------------
void testApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    ofNoFill();
    ofSetColor(255);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    
    ofPoint pos = ofPoint(0,0);
    
    for(int i = 0; i < ofGetFrameNum() && i < 1000; i++ ){
        
        float grow = i;
        
        pos.x += i * cos(powf(phi,2)*TWO_PI*i);
        pos.y += i * sin(powf(phi,2)*TWO_PI*i);
        
        ofCircle(pos, grow*0.1);
    }
    
    ofPopMatrix();
    
    ofDrawBitmapString("Circles: " + ofToString(ofGetFrameNum()), 15,15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'f'){
        ofToggleFullscreen();
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