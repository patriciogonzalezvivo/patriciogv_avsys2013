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
    playbackPos = 0;
    
    sampleCount			= 0;
	grainsPerSecond		= 150;
	grainCount			= 0;
	
	avgFrequency			= 400;
	avgFrequencySpread		= 10;   // will go from 390-410
	avgGrainLength			= 80;
	avgGrainLengthSpread	= 10;
	avgVolume				= 0.17f;
	avgVolumeSpread			= 0.005f;
}

//--------------------------------------------------------------
void testApp::update(){
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    ofPushMatrix();
    
    ofTranslate(0, ofGetHeight()*0.5 );
    
    ofSetColor(255,200);
    sample.soundWave.draw();
    ofPopMatrix();
    
    if (!bRecord){
        ofSetColor(255,0,0,50);
        ofRectangle rect;
        rect.setFromCenter(mouseX, ofGetHeight()*0.5, ((float)mouseY/(float)ofGetHeight())*ofGetWidth(), ofGetHeight());
        ofRect(rect);
        
        ofSetColor(255, 100);
        ofLine(mouseX,0,mouseX,ofGetHeight());
    }

    ofSetColor(255);
    ofDrawBitmapString("avg freq (a/z) : " + ofToString(avgFrequency, 3), 30, 30);
	ofDrawBitmapString("avg freqSpread (s/x) : " + ofToString(avgFrequencySpread,3), 30, 45);
	ofDrawBitmapString("freqencies between : " + ofToString(avgFrequency - avgFrequencySpread, 3) + " - " + ofToString(avgFrequency + avgFrequencySpread, 3), 30, 60);
    
	ofDrawBitmapString("avg length (d/c) : " + ofToString(avgGrainLength, 3), 30, 90);
	ofDrawBitmapString("avg length spread (f/v) : " + ofToString(avgGrainLengthSpread,3), 30, 105);
	ofDrawBitmapString("length between : " + ofToString(avgGrainLength - avgGrainLengthSpread, 3) + " - " + ofToString(avgGrainLength + avgGrainLengthSpread, 3), 30, 120);
	
	ofDrawBitmapString("avg volume (g/b) : " + ofToString(avgVolume, 3), 30, 150);
	ofDrawBitmapString("avg volume spread (h/n) : " + ofToString(avgVolumeSpread,3), 30, 165);
	ofDrawBitmapString("volumes between : " + ofToString(avgVolume - avgVolumeSpread, 3) + " - " + ofToString(avgVolume + avgVolumeSpread, 3), 30, 180);
	
	ofDrawBitmapString("grains per second  (j/m) : " + ofToString((int)(grainsPerSecond)), 30, 210);
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

        //  Clean buffer
        //
        for (int i = 0; i < bufferSize; i++){
			output[i*nChannels    ] = 0;
			output[i*nChannels + 1] = 0;
		}
        
        //  Play using the mouse as a header
        //
//        playbackPos = ((float)mouseX/(float)ofGetWidth())*(float)sample.size();
//        sample.ply(playbackPos, output, bufferSize, nChannels);
        
        //  Use the mouse as a header but using granular synthesis
        //
        makeGrains(output, bufferSize, nChannels);
        
    }
}

void testApp::makeGrains(float * _output, int _bufferSize, int _nChannels){
    
    float oldGrainCount  = grainCount;
	grainCount			+= grainsPerSecond * ( _bufferSize/44100.0f );
	
	int diff = (int)grainCount - (int)oldGrainCount;
	
	for (int i = 0; i < diff; i++){
		Grain grain;
		grain.sample = &sample;
        
		grains.push_back(grain);
		
		// calc random freq, volume and length
		float freq		= ofRandom(avgFrequency - avgFrequencySpread, avgFrequency + avgFrequencySpread);
		float volume	= ofRandom(avgVolume - avgVolumeSpread, avgVolume + avgVolumeSpread);
		float length	= ofRandom(avgGrainLength - avgGrainLengthSpread, avgGrainLength + avgGrainLengthSpread);
		
		// check for extreme conditions:
		if (length < 1) length = 1;
		if (volume < 0) volume = 0;
		if (freq < 1) freq = 1;
		
		grains[grains.size() - 1].calcuteGrainSamples((int)length, sampleCount + (int)ofRandom(0,_bufferSize), freq, volume);
	}
	
    
	grains.erase(std::remove_if(grains.begin(),grains.end(), canBeRemoved), grains.end());
	
    
	for (int i = 0; i < _bufferSize; i++){
		
		for (int j = 0; j < grains.size();  j++){
			float sample = grains[j].getSampleForPosition (sampleCount);
			//if (sample > 0) printf("sample = %f \n", sample);
			_output[i*2    ] += sample;
			_output[i*2 + 1] += sample;
		}
		sampleCount++;
		//printf("sampleCount %i \n", (int) sampleCount);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == ' '){
        if (!bRecord){
            sample.clear();
            bRecord = true;
        }
    } else
        switch (key){
                
            case 'a':
                avgFrequency += 10;
                break;
            case 'z':
                avgFrequency -= 10;
                if (avgFrequency < 30) avgFrequency = 30;
                break;
            case 's':
                avgFrequencySpread += 10;
                break;
            case 'x':
                avgFrequencySpread -= 10;
                if (avgFrequencySpread < 0) avgFrequencySpread = 0;
                break;
                
            case 'd':
                avgGrainLength += 1;
                break;
            case 'c':
                avgGrainLength -= 1;
                if (avgGrainLength < 0) avgFrequency = 0;
                break;
            case 'f':
                avgGrainLengthSpread += 1;
                break;
            case 'v':
                avgGrainLengthSpread -= 1;
                if (avgGrainLengthSpread < 0) avgGrainLengthSpread = 0;
                break;
                
            case 'g':
                avgVolume += 0.01f;
                break;
            case 'b':
                avgVolume -= 0.01f;
                if (avgVolume < 0) avgVolume = 0;
                break;
            case 'h':
                avgVolumeSpread += 0.001f;
                break;
            case 'n':
                avgVolumeSpread -= 0.001f;
                if (avgVolumeSpread < 0) avgVolumeSpread = 0;
                break;
                
            case 'j':
                grainsPerSecond += 1;
                break;
            case 'm':
                grainsPerSecond -= 1;
                if (grainsPerSecond < 0) grainsPerSecond = 0;
                break;
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