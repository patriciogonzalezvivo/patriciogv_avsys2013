//
//  Grain.h
//  04-Granular-Synthesis
//
//  Created by Patricio Gonzalez Vivo on 3/9/13.
//
//

#ifndef GRAIN
#define GRAIN

#include "ofMain.h"
#include "Sample.h"

class Grain {
public:
    Grain();
    
    void calcuteGrainSamples(int _millis, unsigned long long _startSample, float _freq, float _amplitude);
    float getSampleForPosition(unsigned long long samplePosition);
    
    float                   *head;
    float                   *headSize;
    
    int                     start;
    float                   freq;
    float                   amp;
    
    unsigned long long		firstSample;        // the time of the first sample, should be not an int
    int						lengthInSamples;
    float					lengthInMillis;
    bool					bAmReadyToBeErased;
    
    Sample                  *sample;
    
    vector <float>          grainSample;            // this is fairly inneficient.  better is preallocated arrays.
};

#endif
