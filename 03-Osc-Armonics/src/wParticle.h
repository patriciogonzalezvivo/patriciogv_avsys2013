//
//  Particle.h
//
//  Created by Patricio Gonzalez Vivo on 9/20/12.
//
//

#ifndef WPARTICLE
#define WPARTICLE

#include "ofMain.h"

class wParticle : public ofPoint {
public:
    
    wParticle();
    
    void moveTo(float _x, float _y);
    
    void drawDot();
    void drawTrail(float _alpha);
    
    ofSoundPlayer   sound;
    
private:
    vector <ofPoint> points;
    
    float   redValue;
};


#endif
