//
//  Sample.cpp
//  04-Granular-Synthesis
//
//  Created by Patricio Gonzalez Vivo on 3/9/13.
//
//

#include "Sample.h"

Sample::Sample(){
    soundWave.setMode(OF_PRIMITIVE_LINE_STRIP);
}

int Sample::size(){
    return left.size();
}

void Sample::clear(){
    left.clear();
    right.clear();
}

void Sample::rec(float *_input, int _bufferSize, int _nChannels){
    for (int i = 0; i < _bufferSize; i++){
        left.push_back(   _input[i * _nChannels   ]*0.5   );
        right.push_back(  _input[i * _nChannels+1 ]*0.5   );
    }
}

void Sample::ply(int &_head, float *_output, int _bufferSize, int _nChannels){
//    if ( _head < left.size() ){
    if (left.size() > 0 ){
        
        _head = _head%left.size();
        
        for (int i = 0; i < _bufferSize; i++){
            _output[i*_nChannels] = left[_head];
            _output[i*_nChannels + 1] = right[_head];
            
            _head = ( _head+1 )%left.size();
        }
    }
}

void Sample::update(){
    if (left.size() > 0){
        soundWave.clear();
        for (int i = 0; i < left.size() ; i++){
            float x = ofMap(i, 0, left.size(), 0, ofGetWidth());
            soundWave.addVertex(ofPoint(x, left[i]*180));
        }
    }
}
