#pragma once

#include "ofMain.h"

#include "Line.h"
#include "VisualPlayer.h"

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
    
    VisualPlayer    player;
    
    Line            actualLine;
    vector<Line>    storedLines;

    string          soundFile;
    string          xmlFile;
    ofPoint         selectOffSet;
    float           lineStroke;
    int             nSelected;
    int             nBookmark;
    bool            bCursor;
    bool            bLoaded;
    bool            bPDF;
    
};
