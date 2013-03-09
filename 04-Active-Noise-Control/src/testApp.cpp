#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    int bufferSize      = 512;
	
    sampleRate 			= 44100;
	phase 				= 0;
	phaseAdder 			= 0.0f;
	phaseAdderTarget 	= 0.0f;
	volume				= 0.1f;
    
    targetFrequency     = FUNDAMENTAL_TONE*4.0;
    
    lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);
    
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    
}

//--------------------------------------------------------------
void testApp::update(){
    phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    
    ofNoFill();
    
    // draw the left channel:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, ofGetHeight()*0.5-200, 0);
    
    ofDrawBitmapString("Left Channel", 4, 18);
    
    ofSetColor(225,100);
    ofRect(0, 0, ofGetWidth()-32*2.0, 200);
    
    ofSetColor(255);
    
    ofBeginShape();
    for (int i = 0; i < lAudio.size(); i++){
        float x =  ofMap(i, 0, lAudio.size(), 0, ofGetWidth()-32*2.0, true);
        ofVertex(x, 100 -lAudio[i]*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();
    
	// draw the right channel:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, ofGetHeight()*0.5, 0);
    
    
    ofDrawBitmapString("Right Channel ( + "+ ofToString(phaseOffSet,3)+  " offset )", 4, 18);
    
    ofSetColor(225,100);
    ofRect(0, 0, ofGetWidth()-32*2.0, 200);
    
    ofSetColor(255);
    
    ofBeginShape();
    for (int i = 0; i < rAudio.size(); i++){
        float x =  ofMap(i, 0, rAudio.size(), 0, ofGetWidth()-32*2.0, true);
        ofVertex(x, 100 -rAudio[i]*180.0f);
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();
    
    ofDrawBitmapString("Freq: " + ofToString(targetFrequency, 2) + "Hz", 15,15);
    ofDrawBitmapString("Vol.: " + ofToString(volume, 2), 15,30);
    ofDrawBitmapString("Use your LEFT/RIGHT keys to move the offset and pay atention for changes", 15,45);
}


//--------------------------------------------------------------
void testApp::audioOut(float * output, int bufferSize, int nChannels){
    
	while (phase > TWO_PI){
		phase -= TWO_PI;
	}
    
    
    phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
    for (int i = 0; i < bufferSize; i++){
        phase += phaseAdder;
        
        float sample = sin(phase);
        lAudio[i] = output[i*nChannels    ] = sin(phase) * volume;
        rAudio[i] = output[i*nChannels + 1] = sin(phase+phaseOffSet) * volume;
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if ( key == OF_KEY_RIGHT){
        phaseOffSet += PI*0.1;
    } else if ( key == OF_KEY_LEFT ){
        phaseOffSet -= PI*0.1;
    } else if ( key == OF_KEY_UP ){
        volume += 0.01;
    } else if ( key == OF_KEY_DOWN ){
        volume -= 0.01;
    } else if ( key == '1'){
        targetFrequency     = FUNDAMENTAL_TONE;
    } else if ( key == '2'){
        targetFrequency     = FUNDAMENTAL_TONE*2.0;
    } else if ( key == '3'){
        targetFrequency     = FUNDAMENTAL_TONE*3.0;
    } else if ( key == '4'){
        targetFrequency     = FUNDAMENTAL_TONE*4.0;
    } else if ( key == '5'){
        targetFrequency     = FUNDAMENTAL_TONE*5.0;
    } else if ( key == '6'){
        targetFrequency     = FUNDAMENTAL_TONE*6.0;
    } else if ( key == '7'){
        targetFrequency     = FUNDAMENTAL_TONE*7.0;
    } else if ( key == '8'){
        targetFrequency     = FUNDAMENTAL_TONE*8.0;
    } else if ( key == '9'){
        targetFrequency     = FUNDAMENTAL_TONE*9.0;
    } else if ( key == 'f'){
        ofToggleFullscreen();
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	float height = (float)ofGetHeight();
	float heightPct = ((height-y) / height);
	
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