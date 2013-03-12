//
//  ofxPulseSensor.h
//  05_Pulse_Wave
//
//  Created by Patricio Gonzalez Vivo on 3/11/13.
//
//

#ifndef PULSSENSOR
#define PULSSENSOR

#include "ofMain.h"

struct PulseData{
    int     sensor;
    int     IBI;
    int     BPM;
    
    float   normVal;
    float   sec;
    
    bool    bGoodData;
};

class ofxPulseSensor {
public:
    
    ofxPulseSensor();
    
    void        setup(int _dev, int _baud);
    void        update();
    
    void        draw();
    
    int         Sensor;     // HOLDS PULSE SENSOR DATA FROM ARDUINO
    int         IBI;        // HOLDS TIME BETWEN HEARTBEATS FROM ARDUINO
    int         BPM;        // HOLDS HEART RATE VALUE FROM ARDUINO
    
    vector<float>   raw;
    
private:
    vector<PulseData>   data;
    ofSerial            serial;
    
    
};
#endif
