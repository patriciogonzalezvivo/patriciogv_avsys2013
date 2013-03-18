#pragma once

#include "ofMain.h"
#include "ofxFX.h"

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
	
    //  Sound Stuff
    //
    void audioIn(float * input, int bufferSize, int nChannels);
    ofSoundStream soundStream;
    vector <float> inLeft;
//    vector <float> inRight;
    float   volume;             // via RMS
    float   pitch;              // via zero crossings
    float   volumeSmoothSlow;   // via RMS
    float   pitchSmoothSlow;    // via zero crossings
    float   sampleSmooth;
    bool    bSignOflastValue;   // was the last sample + or -
    
    //  Visual Stuff
    //
    ofxGlow glow;
    ofxChromaAb chromaAb;
    ofMesh  mLeft;
//    ofMesh  mRight;
    float   audioMeshAlpha;
    int     width, height;
    int     nFaces;
};
