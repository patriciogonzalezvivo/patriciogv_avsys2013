//
//  Sample.h
//  04-Granular-Synthesis
//
//  Created by Patricio Gonzalez Vivo on 3/9/13.
//
//

#ifndef SAMPLE
#define SAMPLE

#include "ofMain.h"

class Sample {
public:
    Sample();
    
    int     size();
    void    clear();
    
    void    rec(float * _input, int _bufferSize, int _nChannels);
    void    ply(int _head, float * _output, int _bufferSize, int _nChannels);
    
    void    update();
    
    ofMesh  soundWave;
    
    vector <float> left;
    vector <float> right;
    
private:
    
};

#endif 
