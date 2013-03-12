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
    mouse += ( ofPoint(mouseX,mouseY) - mouse)*0.1;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    //  Line
    //
    ofSetColor(200,0,0);
    ofCircle(center, 2);
    ofSetColor(255);
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
    ofSetColor(200,0,0);
    ofCircle(A, 2);
    
    float b = dist.length()/pow(phi,2);
    ofPoint B = ofPoint(b*cos(radian),b*sin(radian));
    ofSetColor(200,0,0);
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
    ofSetColor(255,150);
    ofRect(0, 0, a, a);
    ofSetColor(255);
    ofDrawBitmapString("B", a+b*0.5, -5);
    ofSetColor(255,150);
    ofRect(a,0,b,a);

    //  Draw occult center
    //
    ofSetColor(255,100);
    ofLine(0,0,a+b,a);
    ofLine(a+b,0,a,a);
    
    //
    //  Found Circle 
//    ofPushMatrix();
//    ofPushStyle();
//    ofTranslate(a*0.5, a);
//    ofSetColor(255,25);
//    ofCircle(0,0, a*0.5+ (a/phi) );
//    ofLine(0,0,a*0.5,-a);
//    ofPopStyle();
//    ofPopMatrix();
    
    //
    //  Draw Spiral
    float segment = a;
    for (int i = 0; i < 10; i++) {
        drawSpiralArc(segment);
        segment /= phi;
    }
    
    ofPopStyle();
    ofPopMatrix();
    
    ofPopMatrix();
    
}

void testApp::drawSpiralArc( double _size ){
    ofRect(0,0,_size,_size);
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(_size, _size);
    ofRotate(-90, 0, 0, 1.0);
    
    ofPolyline arc;
    arc.arc(0,0, _size, _size, -90, 0, 60);
    
    ofSetColor(50);
    arc.draw();
    
    ofPopStyle();
    ofPopMatrix();
    
    ofTranslate(_size+_size/phi, 0);
    ofRotate(90, 0,0, 1.0);
    
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
    center.x = w*0.2;
    center.y = h*0.2;
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}