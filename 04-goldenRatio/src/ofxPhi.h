//
//  ofxPhi.h
//  goldenRatio
//
//  Created by Patricio Gonzalez Vivo on 3/6/13.
//
//

#ifndef OFXPHI
#define OFXPHI

#include "ofMain.h"

class ofxPhi {
public:
    
    ofxPhi();
    
    double   getFye();
    double   getFee();
    
    double   getA();
    double   getB();
    
    
private:
    double phi;
    
};

#endif 
