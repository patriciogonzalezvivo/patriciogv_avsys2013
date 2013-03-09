#pragma once

#include "ofMain.h"

//  Based on:
//              http://en.wikipedia.org/wiki/Phase_cancellation
//              http://en.wikipedia.org/wiki/Active_noise_control
//

#define FUNDAMENTAL_TONE 43.066666666666666666666666666666666

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	
	void audioOut(float * input, int bufferSize, int nChannels);
    
    ofSoundStream soundStream;
    
    vector <float> lAudio;
    vector <float> rAudio;
    
    float   volume;
   
    float 	targetFrequency;
    
    float   phase, phaseOffSet;
    float 	phaseAdder;
    float 	phaseAdderTarget;
    
    int		sampleRate;
};
