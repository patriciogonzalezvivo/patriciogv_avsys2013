//
//  VisualPlayer.h
//  drawingTool
//
//  Created by Patricio Gonzalez Vivo on 4/27/13.
//
//

#ifndef VISUAL_PLAYER
#define VISUAL_PLAYER

#include "ofMain.h"

class VisualPlayer : public ofSoundPlayer {
public:
    
    VisualPlayer();
    
    bool    loadSound(string _file);

    void    play();
    void    pause();
    
    float   getLenght();
    float   getPosition();
    float   getLastTime();  // if playing -1 else the elaptime of when stops
    
    bool    inside(int _x, int _y);
    
    void    draw();
    
    int     height;
private:
    float   lastTime;
    float   lenght;
    float   pct;
    
    bool    bPlay;
};

#endif
