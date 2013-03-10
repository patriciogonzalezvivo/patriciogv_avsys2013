#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    //  Sound I/O 
    //
    ofSoundStreamSetup(2,2,this, 44100, 512, 4);
    
    bRecord = false;
    soundWave.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    margin = 32;
    playerHead = 0;
}

//--------------------------------------------------------------
void testApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    ofPushMatrix();
    
    ofTranslate(margin, ofGetHeight()*0.5);
    
    ofSetColor(255,200);
    soundWave.draw();
    
    if (!bRecord){
        float x = ofMap( playerHead, 0, inLeft.size(), 0, ofGetWidth()-margin*2.0);
        ofSetColor(255, 100);
        ofLine(x,-ofGetHeight()*0.5,x,ofGetHeight()*0.5);
    }
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){
    
    if (bRecord){
        for (int i = 0; i < bufferSize; i++){
            inLeft.push_back(   input[i*nChannels]*0.5  );
            inRight.push_back(  input[i*nChannels+1]*0.5);
        }
    }
}

//--------------------------------------------------------------
void testApp::audioOut(float * output, int bufferSize, int nChannels){
	
    if (!bRecord){
        if ((inLeft.size() > 0)){
            for (int i = 0; i < bufferSize; i++){
                output[i*nChannels] = inLeft[playerHead];
                output[i*nChannels + 1] = inRight[playerHead];
                
                playerHead = (playerHead+1)%inLeft.size();
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (!bRecord){
        inLeft.clear();
        inRight.clear();
        bRecord = true;
        cout << "Start Recording" << endl;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    bRecord = false;
    playerHead = 0;
    cout << "End Recording" << endl;
    
    if (inLeft.size() > 0){
        updateSoundWave();
    }
}

void testApp::updateSoundWave(){
    soundWave.clear();
    for (int i = 0; i < inLeft.size() ; i++){
        float x = ofMap(i, 0, inLeft.size(), 0, ofGetWidth()-margin*2.0);
        soundWave.addVertex(ofPoint(x, inLeft[i]*180));
    }
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
    updateSoundWave();
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}