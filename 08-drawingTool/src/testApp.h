#pragma once

#include "ofMain.h"

#include "Line.h"
#include "VisualPlayer.h"

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
	
    Line            actualLine;
    vector<Line>    storedLines;
    
    VisualPlayer    player;
    
    float           offSetX;
    float           offSetY;
    float           stroke;
    int             selected;
    
};
