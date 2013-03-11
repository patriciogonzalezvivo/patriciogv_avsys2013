#pragma once

#include "ofMain.h"

#include "Sample.h"
#include "Grain.h"
#include "TimeLine.h"

//  Using http://www.intmath.com/trigonometric-graphs/music.php as a reference for the notes
//

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void audioIn(float * input, int bufferSize, int nChannels);
    void audioOut(float * input, int bufferSize, int nChannels);
    
    void makeGrains(float * _output, int _bufferSize, int _nChannels);
    
    TimeLine            line;
    
    ofSoundStream       soundStream;
    Sample              sample;
    
    unsigned long long	sampleCount;
	float				grainCount;
	
	vector <Grain>      grains;
	
    float               head;
    float               headSize;
    
	float				grainsPerSecond;
	float				avgFrequency;
	float				avgFrequencySpread;
	float				avgGrainLength;
	float				avgGrainLengthSpread;
	float				avgVolume;
	float				avgVolumeSpread;
    
	int                 playbackPos;
    
    bool                bRecord;
    bool                bShowGrain;
    bool                bPress;
};
