//
//  Grain.cpp
//  04-Granular-Synthesis
//
//  Created by Patricio Gonzalez Vivo on 3/9/13.
//
//

#include "Grain.h"

void Grain::calcuteGrainSamples(int millis, unsigned long long startSample, float freq, float amplitude){
    
	firstSample				= startSample;
	lengthInMillis			= millis;
	float lengthInSeconds	= millis / 1000.0f;
	lengthInSamples			= (int)(44100.0f * lengthInSeconds);
	float phase				= ofRandom(0,TWO_PI);			// give a random phase
	
	
	float sinProfileAngle		= 0;
	float sinProfileAdder		= PI / (float)lengthInSamples;
	float carrierAngle			= phase;
	float carrierAngleAdder		= (freq / (float) 44100.0f) * TWO_PI;
	
	bAmReadyToBeErased = false;
	
	if (sample->size() > 0){
        
//		float pctWithOffset = gsamples->pct + ofRandom(-gsamples->pctOffset, gsamples->pctOffset);
        
        float mouseY = (float)ofGetMouseY()/(float)ofGetHeight();
        float pctWithOffset = (float)ofGetMouseX()/(float)ofGetWidth() + ofRandom( mouseY, -mouseY )*0.5;
        
		int startPosition = (int)(pctWithOffset * sample->size()) - lengthInSamples/2;
		
		for (int i = 0; i < lengthInSamples; i++){
			
			int samplePosition = startPosition + i;
			while (samplePosition < 0) samplePosition += sample->size();
			while (samplePosition >= sample->size() ) samplePosition -= sample->size();
			
			float s = sample->left[samplePosition] * amplitude * (sin(sinProfileAngle));
			grainSample.push_back(s);
			carrierAngle	+= carrierAngleAdder;
			sinProfileAngle += sinProfileAdder;
		}
        
	} else {
		
		for (int i = 0; i < lengthInSamples; i++){
			float s = sin(carrierAngle) * amplitude * (sin(sinProfileAngle));
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