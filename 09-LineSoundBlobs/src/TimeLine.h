//
//  VisualPlayer.h
//  drawingTool
//
//  Created by Patricio Gonzalez Vivo on 4/27/13.
//
//

#ifndef VISUAL_PLAYER
#define VISUAL_PLAYER

#define MARK_SIZE 6.0
#define TIMELINE_HEIGHT 100.0

#include "ofMain.h"
#include "fftOctaveAnalyzer.h"

class TimeLine {
public:
    
    TimeLine();
    
    void    play();
    void    pause();
    
    float   getTime();
    float   getLength();
    float   getPosition();
    float   getLastTime();
    
    bool    getIsPlaying();
    void    setPosition(float _pct);
    void    setLength(float _lengt);
                       
    void    addBookmark(float _pct = -1.0); //  -1.0 is NOW
    
    bool    inside(int _x, int _y);
    
    void    update();
    void    draw();
    
    vector<float>   bookmarks;
    int     height;
    
    FFTOctaveAnalyzer   *fft;
    bool    bEdit;
    
private:
    void    drawMark(float _pct, bool _showSec, bool _showArrow);
    float   lastTime;
    float   header,lenght;
    
    bool    bPlay;
};

#endif
