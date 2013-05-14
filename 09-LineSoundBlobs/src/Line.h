//
//  TimeLine.h
//  week3-multiLine
//
//  Created by Patricio Gonzalez Vivo on 9/28/12.
//
//

#ifndef TIMELINE
#define TIMELINE

#define MAX_LINE_WIDTH 5.0
#define REFERENCEBOX_HEIGHT 15.0

#include "ofMain.h"
#include "TimeLine.h"

#include "fftOctaveAnalyzer.h"

class TimePoint : public ofPoint {
public:
    float time;
    float angle;
    float fft[18];
};

class Line {
public:
    
    Line();
    Line(vector<TimePoint> _line, float _startTime, float _stroke );
    
    void        setTimeLine( TimeLine *_player );
    void        setFinish();
    
    void        addPoint(ofPoint _pos);
    int         size();
    void        clear();
    TimePoint&  operator[](int _index);
    bool        isFinish();
    
    void        moveRefTo( float _x, float _y );
    
    float       getDuration();
    float       getStartTime();
    float       getIndexForTime( float _time );
    ofPoint     getPositionForTime( float _time );
    ofPoint     getVelocityForTime( float _time );
    ofRectangle getReferenceBox();
    
    bool        checkOver(int _x, int _y);
    
    void        updateBox();
    
    void        draw(float _time);
    
    float       stroke;
    
    bool        bSelected;
    
private:
    ofMesh              referenceFFT;
    ofRectangle         referenceBox;
    
    vector<TimePoint>   points;
    
    TimeLine            *player;
    
    float               length;
    float               startTime;

    int                 octave;
    
    bool                bFinish;
};


#endif
