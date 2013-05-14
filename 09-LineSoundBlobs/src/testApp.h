#pragma once

#include "ofMain.h"

#include "Line.h"
#include "TimeLine.h"

#include "fft.h"
#include "FFTOctaveAnalyzer.h"

#include "ofxXmlSettings.h"

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
	
    bool loadSecuence(string _file );
    bool saveSecuence(string _file = "none");
    
    //  Sound
    //
    void audioReceived(float * input, int bufferSize, int nChannels);
    fft                 myFft;
    FFTOctaveAnalyzer   FFTanalyzer;
    int                 bufferSize;
    int                 large;
    float               *left;
    float               *right;
    float               *magnitude;
    float               *phase;
    float               *power;
    float               *freq;
    
    //  Drawing
    //
    TimeLine        player;
    Line            actualLine;
    vector<Line>    storedLines;
    ofPoint         selectOffSet;
    float           lineStroke;
    int             nSelected;
    int             nBookmark;
    
    bool            bCursor;
    bool            bFlipped;
};
