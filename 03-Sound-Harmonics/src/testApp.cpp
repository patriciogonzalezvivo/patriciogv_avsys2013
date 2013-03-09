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
    
    targetFrequency     = FUNDAMENTAL_TONE;
    
    audio.assign(bufferSize, 0.0);
    
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
    
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, ofGetHeight()*0.5 , 0);
    
    ofSetColor(255);
    
    float threshold = 0.0012 + volume*0.02;
    int count = 0;
    float lastX = 0;
    vector <float> harmonics;
    
    ofBeginShape();
    for (int i = 0; i < audio.size(); i++){
        float x =  ofMap(i, 0, audio.size(), 0, ofGetWidth()-32*2, true);
        ofVertex(x, -audio[i]*180.0f);
        
        //  Search for evithing stock on 0
        //
        if ( (audio[i] < threshold) && (audio[i] > -threshold)   ){
            
            //  Filter multiple point
            //
            if ( x-lastX > 20 ){
                ofPushStyle();
                ofFill();
                ofSetColor(255, 0, 0);
                ofCircle(x, - audio[i]*180.0f, 2);
                ofPopStyle();
                
                lastX = x;
                count++;
            
                //  record it as a posible armonic
                //
                if ( posibleHarmonics.size() < count ){
                    posibleHarmonics.push_back(x);
                } else {
                    if ( abs(posibleHarmonics[count]-x) < 5 ){
                        harmonics.push_back(x);
                    }
                    
                    posibleHarmonics[count] = x;
                }
            }
        }
    }
    ofEndShape(false);
    
    ofPopMatrix();
	ofPopStyle();
    
    //  Take out the excedent
    //
    while (posibleHarmonics.size() > count ){
        posibleHarmonics.pop_back();
    }
    
    //  Draw the armonics
    //
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(32, 0, 0);
    ofSetColor(255, 20);
    for (int i = 0; i < harmonics.size(); i++){
        ofLine(harmonics[i], 0, harmonics[i], ofGetHeight());
    }
    ofPopMatrix();
    ofPopStyle();
    
    
    ofDrawBitmapString("Freq: " + ofToString(targetFrequency, 2) + "Hz (LEFT/RIGHT & by pressing the numbers you can jump between harmonics)", 15,15);
    ofDrawBitmapString("Vol.: " + ofToString(volume, 2) + " (UP/DOWN)", 15,30);
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
        audio[i] = output[i*nChannels    ] = sample * volume;
        output[i*nChannels + 1] = sample * volume;
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if ( key == OF_KEY_RIGHT){
        targetFrequency += 0.1;
    } else if ( key == OF_KEY_LEFT ){
        targetFrequency -= 0.1;
    } if ( key == ','){
        targetFrequency += 0.01;
    } else if ( key == '.'){
        targetFrequency -= 0.01;
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