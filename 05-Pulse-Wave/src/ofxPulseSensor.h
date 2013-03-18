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
    
    float   val;
    float   sec;
    
    bool    bGoodData;
};

class ofxPulseSensor : public ofSerial {
public:
    
    void        update();
    
    PulseData   getData( float _sec);
    float       getVal( float _sec);
    int         getBPM( float _sec);
    int         getIBI( float _sec);
    int         getSensor( float _sec);
    
    void        draw();
    
    int         Sensor;     // HOLDS PULSE SENSOR DATA FROM ARDUINO
    int         IBI;        // http://en.wikipedia.org/wiki/Heart_rate
    int         BPM;        // http://en.wikipedia.org/wiki/Interbeat_interval

private:
//    ofSerial            serial;
    vector<PulseData>   data;
};
#endif
