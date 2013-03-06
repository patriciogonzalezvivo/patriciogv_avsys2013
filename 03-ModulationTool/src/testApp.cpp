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
    
    phase   = 0.0f;
    amplitud  = 140.0f;
    frequency = 1.0f;
    
    modAngle = 0.0f;
    modRadio = 140.0f;
    
    bFM = false;
}

//--------------------------------------------------------------
void testApp::update(){
    //  Modulator step
    //
    modAngle += ofMap(mouseX,0,ofGetWidth(),0,10)*TWO_PI/ofGetFrameRate();
    while (modAngle > PI*2)
        modAngle -= PI*2;
    
    if (!bFM){
        amplitud = 1 + cos(modAngle)*modRadio;
        frequency = 1.0f;
        ofSetWindowTitle("AM");
    } else {
        frequency = 1 + cos(modAngle)*5;
        amplitud = modRadio;
        ofSetWindowTitle("FM");
    }
        
    phaseAdder = (float)(frequency * TWO_PI) / (float)ofGetFrameRate();
    phase += phaseAdder;
        
    //  Just for making it clear angle could be more than the 2PI
    //
    while (phase > PI*2)
        phase -= PI*2;
    
    //  Get the cartesian
    //
    dotPos.x = cos(phase) * amplitud;
    dotPos.y = sin(phase) * amplitud;
    
    //  Store the Y (sin) position
    //
    sinHistory.push_back(dotPos.y);

    while (sinHistory.size() > ofGetWidth()-center.x-200-30){
        sinHistory.erase(sinHistory.begin());
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    ofDrawBitmapString("Switch between AM/FM", 15,15);
    ofDrawBitmapString("Freq = " + ofToString(frequency) + "Hz ( App. at " + ofToString((float)ofGetFrameRate()) + "Hz )", 15,30);
    
    
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
    angleLine.arc(0, 0, amplitud*0.3, amplitud*0.3, 0, ofRadToDeg(phase));
    ofNoFill();
    ofSetColor(255,180);
    angleLine.draw();
    
    //  Draw sin proyected dots
    //
    ofSetColor(255,100);
    ofLine(lenghtOfAxis + 30,-amplitud, lenghtOfAxis + 30,amplitud);
    ofDrawBitmapString(ofToString(-amplitud), lenghtOfAxis + 30 - 12,-amplitud -5);
    ofDrawBitmapString(ofToString(amplitud), lenghtOfAxis + 30 - 3, amplitud + 15 );
    ofPoint sinPos = ofPoint(lenghtOfAxis + 30, dotPos.y);
    drawDot(sinPos);
    
    //  Draw cos of modulator
    //
    ofSetColor(255,100);
    ofLine(-modRadio, lenghtOfAxis + 30, modRadio, lenghtOfAxis + 30);
    ofDrawBitmapString("-"+ofToString(modRadio), -modRadio - 15, lenghtOfAxis + 30 + 5);
    ofDrawBitmapString(ofToString(modRadio) + " Amp // Freq = " + ofToString(ofMap(mouseX,0,ofGetWidth(),0,10)) + "Hz", modRadio + 5, lenghtOfAxis + 30 + 5);
    ofPoint cosPos = ofPoint( cos(modAngle)*modRadio ,  lenghtOfAxis + 30);
    drawDot(cosPos);
    cosPos = ofPoint( -cos(modAngle)*modRadio ,  lenghtOfAxis + 30);
    drawDot(cosPos);
    
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
    if (key == 'f'){
        ofToggleFullscreen();
    } else {
        bFM = !bFM;
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
    center.y = h*0.5;
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}