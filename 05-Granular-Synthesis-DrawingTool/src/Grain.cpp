//
//  Grain.cpp
//  04-Granular-Synthesis
//
//  Created by Patricio Gonzalez Vivo on 3/9/13.
//
//

#include "Grain.h"

Grain::Grain(){
    start = 0;
    sample = NULL;
}

void Grain::calcuteGrainSamples(int _millis, unsigned long long _startSample, float _freq, float _amplitude){
    
	firstSample				= _startSample;
	lengthInMillis			= _millis;
	float lengthInSeconds	= _millis / 1000.0f;
	lengthInSamples			= (int)(44100.0f * lengthInSeconds);
	float phase				= ofRandom(0,TWO_PI);			// give a random phase
	
	
	float sinProfileAngle		= 0;
	float sinProfileAdder		= PI / (float)lengthInSamples;
	float carrierAngle			= phase;
	float carrierAngleAdder		= (_freq / (float) 44100.0f) * TWO_PI;
	
	bAmReadyToBeErased = false;
	
	if (sample->size() > 0){
        
        float pctWithOffset = *head/(float)ofGetWidth() + ofRandom( *headSize, - *headSize )*0.5;
        
        freq    = carrierAngleAdder;
        amp     = _amplitude;
        start   = (int)(pctWithOffset * sample->size());
		int startPosition =  start - lengthInSamples/2;
		
		for (int i = 0; i < lengthInSamples; i++){
			
			int samplePosition = startPosition + i;// * ofMap(freq, 220.0, 880.0, 0.5, 1.5);
			while (samplePosition < 0) samplePosition += sample->size();
			while (samplePosition >= sample->size() ) samplePosition -= sample->size();
			
			float s = sample->left[samplePosition] * _amplitude * sin(sinProfileAngle);
			grainSample.push_back(s);
			carrierAngle	+= carrierAngleAdder;
			sinProfileAngle += sinProfileAdder;
		}
        
	} else {
		
		for (int i = 0; i < lengthInSamples; i++){
			float s = sin(carrierAngle) * _amplitude * sin(sinProfileAngle);
			grainSample.push_back(s);
			carrierAngle	+= carrierAngleAdder;
			sinProfileAngle += sinProfileAdder;
		}
        
	}
	
	//printf("made a grain with %i samples starting at %i\n", lengthInSamples, (int)startSample);
}

float Grain::getSampleForPosition(unsigned long long samplePosition){
    
	if (samplePosition < firstSample) return 0;
	
	float returnVal = 0;
	int position = (int)(samplePosition - firstSample);
	
	if (position < lengthInSamples && position >= 0){
		returnVal = grainSample[position];
	} else {
		if (samplePosition > (firstSample + lengthInSamples)){
			bAmReadyToBeErased = true;
		}
	}
	
	return returnVal;
}