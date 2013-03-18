//
//  ofxPulseSensor.cpp
//  05_Pulse_Wave
//
//  Created by Patricio Gonzalez Vivo on 3/11/13.
//
//

#include "ofxPulseSensor.h"

//void ofxPulseSensor::setup(int _dev, int _baud){
//	serial.setup(0, _baud);
//}

void ofxPulseSensor::update(){
    string serialIn;
    
    unsigned char buffer[1];
    while( readBytes( buffer, 1) > 0){
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
        
        float val = ofMap(Sensor, 212, 1023, -1.0, 1.0, true);
        newPack.val = val;
        newPack.bGoodData = true;
        newPack.sec = ofGetElapsedTimef();
        
        //  Ineficient way of storing data
        //
        if ( (Sensor > 212) && (Sensor < 1023)){
//            raw.push_back( ofMap(Sensor, 212, 1023, -1.0, 1.0) );
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
        float lastGoodDataTime = 0.0;
        for (int i = data.size()-1; i >= 0; i--){
            
            if (data[i].bGoodData){
                lastGoodData = data[i].sensor;
                lastGoodDataIndex = i;
                lastGoodDataTime = data[i].sec;
            } else if ( lastGoodData != 0 ){
                //  Search for the next good Data
                //
                int nextGoodData = 0;
                int nextGoodDataIndex = 0;
                float nextGoodDataTime = 0.0;
                for (int j = i-1; j >= 0 ; j-- ){
                    if (data[j].bGoodData){
                        nextGoodData = data[j].sensor;
                        nextGoodDataIndex = j;
                        nextGoodDataTime = data[j].sec;
                        break;
                    }
                }
                
                if (nextGoodData != 0){
                    
                    //  Try to found a value that have sense given the others
                    //
//                    float pct = ofMap(i, nextGoodDataIndex, lastGoodDataIndex, 0.0, 1.0,true);
                    float pct = ofMap(i, nextGoodDataTime, lastGoodDataTime, 0.0, 1.0,true);
                    
                    data[i].sensor = nextGoodData*(1.0-pct)+lastGoodData*pct;
                    data[i].val = ofMap(data[i].sensor, 212, 1023, -1.0, 1.0,true);
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

PulseData ofxPulseSensor::getData( float _sec ){
    
    PulseData pData;
    
	for (int i = 0; i < data.size()-1; i++){
		if (_sec >= data[i].sec && _sec <= data[i+1].sec){
			
			// calculate pct:
			float part = _sec - data[i].sec;
			float whole = data[i+1].sec - data[i].sec;
			float pct = part / whole;
			
            pData.sensor = (1.0-pct) * (float)data[i].sensor + (pct) * (float)data[i+1].sensor;
            pData.BPM = (1.0-pct) * (float)data[i].BPM + (pct) * (float)data[i+1].BPM;
            pData.IBI = (1.0-pct) * (float)data[i].IBI + (pct) * (float)data[i+1].IBI;
            
            pData.val = (1.0-pct) * (float)data[i].val + (pct) * (float)data[i+1].val;
            pData.sec = (1.0-pct) * (float)data[i].sec + (pct) * (float)data[i+1].sec;
    
            break;
		}
	}
    
    return pData;
}

float ofxPulseSensor::getVal( float _sec ){
    
	for (int i = 0; i < data.size()-1; i++){
		if (_sec >= data[i].sec && _sec <= data[i+1].sec){
			
			// calculate pct:
			float part = _sec - data[i].sec;
			float whole = data[i+1].sec - data[i].sec;
			float pct = part / whole;
			
            return (1.0-pct) * (float)data[i].val + (pct) * (float)data[i+1].val;
		}
	}
    
}

int ofxPulseSensor::getBPM( float _sec ){
    for (int i = 0; i < data.size()-1; i++){
		if (_sec >= data[i].sec && _sec <= data[i+1].sec){
			
			// calculate pct:
			float part = _sec - data[i].sec;
			float whole = data[i+1].sec - data[i].sec;
			float pct = part / whole;
			
            return (1.0-pct) * (float)data[i].BPM + (pct) * (float)data[i+1].BPM;
		}
	}
}

int ofxPulseSensor::getIBI( float _sec ){
    for (int i = 0; i < data.size()-1; i++){
		if (_sec >= data[i].sec && _sec <= data[i+1].sec){
			
			// calculate pct:
			float part = _sec - data[i].sec;
			float whole = data[i+1].sec - data[i].sec;
			float pct = part / whole;
			
            return (1.0-pct) * (float)data[i].IBI + (pct) * (float)data[i+1].IBI;
		}
	}
}

int ofxPulseSensor::getSensor( float _sec ){
    for (int i = 0; i < data.size()-1; i++){
		if (_sec >= data[i].sec && _sec <= data[i+1].sec){
			
			// calculate pct:
			float part = _sec - data[i].sec;
			float whole = data[i+1].sec - data[i].sec;
			float pct = part / whole;
			
            return (1.0-pct) * (float)data[i].sensor + (pct) * (float)data[i+1].sensor;
		}
	}
}

void ofxPulseSensor::draw(){
    
    ofNoFill();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    ofSetColor(255);
    
    if ( data.size() > 0){
        if (data[data.size()-1].bGoodData){
            ofPushStyle();
            ofPushMatrix();
            ofTranslate(0,data[data.size()-1].val*ofGetHeight()*0.5);
            
            ofFill();
            ofCircle(0,0, 2);
            
            ofDrawBitmapString("BPM: " + ofToString(BPM), 5,15);
            ofDrawBitmapString("IBI: " + ofToString(IBI), 5,30);
//            ofDrawBitmapString("Sensor: " + ofToString(Sensor), 0,45);
            
            ofPopMatrix();
            ofPopStyle();
        }
    }
    
    float x = 0;
    for(int i = data.size()-1; i >= 0 && i >= data.size()-ofGetWidth()*0.5 ; i-- ){
        
        if ( data[i].bGoodData ){
            float alpha = ofMap(x, -ofGetWidth()*0.5, 0.0, 0.0, 1.0);
            mesh.addColor( ofFloatColor(1.0, alpha) );
            mesh.addVertex(ofPoint(x, data[i].val*ofGetHeight()*0.5));
        }
        
        x--;
    }
    
    mesh.draw();
    ofPopMatrix();
}