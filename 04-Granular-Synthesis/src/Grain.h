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
    
    void calcuteGrainSamples(int millis, unsigned long long startSample, float freq, float amplitude);
    float getSampleForPosition(unsigned long long samplePosition);
    
    unsigned long long		firstSample;        // the time of the first sample, should be not an int
    int						lengthInSamples;
    float					lengthInMillis;
    bool					bAmReadyToBeErased;
    
    Sample                  *sample;
    
    vector <float>          grainSample;            // this is fairly inneficient.  better is preallocated arrays.
};

#endif
