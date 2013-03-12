#pragma once

#include "ofMain.h"
#include "wParticle.h"

//  Watch:
//          - http://www.youtube.com/watch?v=1M8ciWSgc_k&feature=youtu.be
//          - https://vimeo.com/27336153
//          - https://vimeo.com/28239261

#define TOTAL_PARTICLES 20

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
		
    void drawParticlesLine(float _alpha);
    
    vector<wParticle> particles;
    
    float speed;
};
