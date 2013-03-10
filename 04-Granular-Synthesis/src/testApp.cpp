#include "testApp.h"

bool canBeRemoved ( Grain  &g ) {
	return g.bAmReadyToBeErased;
}

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    //  Sound I/O 
    //
    ofSoundStreamSetup(2,2,this, 44100, 512, 4);
    
    bRecord = false;
    
}

//--------------------------------------------------------------
void testApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    ofPushMatrix();
    
    ofTranslate(0, ofGetHeight()*0.1 );
    
    ofSetColor(255,200);
    sample.soundWave.draw();
    ofPopMatrix();
    
    if (!bRecord){
        ofSetColor(255, 100);
        ofLine(mouseX,0,mouseX,ofGetHeight());
    }

}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){
    if (bRecord){
        sample.rec(input,bufferSize,nChannels);
    }
}

//--------------------------------------------------------------
void testApp::audioOut(float * output, int bufferSize, int nChannels){
    if (!bRecord){
        int i = ofMap( mouseX, 0.0, (float)ofGetWidth(), 0, (float)sample.size());
        sample.ply(i, output, bufferSize, nChannels);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == ' '){
        if (!bRecord){
            sample.clear();
            bRecord = true;
        }
    } else if (key == 'f'){
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if (key == ' '){
        bRecord = false;
        sample.update();
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
//    grainHead = 
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
    sample.update();
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}