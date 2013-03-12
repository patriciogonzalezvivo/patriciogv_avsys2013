//
//  ofxPulseSensor.cpp
//  05_Pulse_Wave
//
//  Created by Patricio Gonzalez Vivo on 3/11/13.
//
//

#include "ofxPulseSensor.h"

ofxPulseSensor::ofxPulseSensor(){    
}

void ofxPulseSensor::setup(int _dev, int _baud){
	serial.setup(0, _baud);
}

void ofxPulseSensor::update(){
    string serialIn;
    
    unsigned char buffer[1];
    while( serial.readBytes( buffer, 1) > 0){
        if (buffer[0] == '\n'){
            break;
        }
        serialIn.push_back(buffer[0]);
    };
    
    if ( serialIn[0] == 'B' ){
        serialIn.erase(0,1);
        BPM = ofToInt(serialIn);
        
    }
    
    if ( serialIn[0] == 'Q' ){
        serialIn.erase(0,1);
        IBI = ofToInt(serialIn);
    }
    
    if ( serialIn[0] == 'S' ){
        
        serialIn.erase(0,1);
        Sensor = ofToInt(serialIn);
        
        PulseData newPack;
        
        newPack.sensor = Sensor;
        newPack.BPM = BPM;
        newPack.IBI = IBI;
        
        float tmpNorm = ofMap(Sensor, 212, 1023, -1.0, 1.0, true);
        newPack.normVal = tmpNorm;
        newPack.bGoodData = true;
        newPack.sec = ofGetElapsedTimef();
        
        //  Ineficient way of storing data
        //
        if ( (Sensor > 212) && (Sensor < 1023)){
            raw.push_back( ofMap(Sensor, 212, 1023, -1.0, 1.0) );
        } else {
            newPack.bGoodData = false;
        }
        
        data.push_back(newPack);
    }
    
    //  Check last values trying to clean wrong numbers
    //
    if (data.size() > 6){
    
        //  Search for the last good data
        //
        int lastGoodData = 0;
        int lastGoodDataIndex = 0;
        for (int i = data.size()-1; i >= 0; i--){
            
            if (data[i].bGoodData){
                lastGoodData = data[i].sensor;
                lastGoodDataIndex = i;
            } else if ( lastGoodData != 0 ){
                //  Search for the next good Data
                //
                int nextGoodData = 0;
                int nextGoodDataIndex = 0;
                for (int j = i-1; j >= 0 ; j-- ){
                    if (data[j].bGoodData){
                        nextGoodData = data[j].sensor;
                        nextGoodDataIndex = j;
                        break;
                    }
                }
                
                if (nextGoodData != 0){
                    float indexDiff = lastGoodDataIndex-nextGoodDataIndex;
                    float dataDiff = lastGoodData-nextGoodData;
                    float diff = (float)dataDiff/(float)indexDiff;
                    
                    data[i].sensor = lastGoodData+dataDiff;
                    cout << data[i].sensor << endl;
                    data[i].normVal = ofMap(data[i].sensor, 212, 1023, -1.0, 1.0,true);
                    data[i].bGoodData = true;
                } 
                
                break;
            }
            
        }
    }

    while (data.size() > ofGetWidth()*0.5) {
        data.erase(data.begin());
    }
}

void ofxPulseSensor::draw(){
    ofDrawBitmapString("Sensor: " + ofToString(Sensor), 15,15);
    ofDrawBitmapString("BPM: " + ofToString(BPM), 15,30);
    ofDrawBitmapString("IBI: " + ofToString(IBI), 15,45);
    
    ofNoFill();
    
    ofPushMatrix();
    ofTranslate(0, ofGetHeight()*0.5);
    
    ofSetColor(255);
    ofBeginShape();
    for(int i = 0; i < data.size();i++ ){
        
//        if ( data[i].bGoodData )
            ofVertex(i, data[i].normVal*ofGetHeight()*0.5);
        
        
        if (i == data.size()-1){
            ofPushStyle();
            ofFill();
            ofCircle(i, data[i].normVal*ofGetHeight()*0.5, 2);
            ofPopStyle();
        }
    }
    ofEndShape();
    
    ofPopMatrix();
}