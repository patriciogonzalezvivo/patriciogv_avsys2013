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
    
    float   getLength();
    float   getPosition();
    float   getLastTime();
    
    void    addBookmark(float _pct = -1.0); //  -1.0 is NOW
    
    bool    inside(int _x, int _y);
    
    void    draw();
    
    vector<float>   bookmarks;
    int     height;
    
private:
    void    drawMark(float _pct, bool _showSec);
    float   lastTime;
    float   lenght;
    float   pct;
    
    bool    bPlay;
};

#endif
