#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
//    ofEnableSmoothing();
    
    ofToggleFullscreen();
    ofHideCursor();
    
    player.loadSound("Light_And_Day.mp3");
    player.play();
    
    actualLine.setVisualPlayer( &player );
    selected = -1;
    stroke = 1;
    actualLine.stroke = stroke;
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    //  Cursor
    //
    ofPushMatrix();
    ofPushStyle();
    ofSetColor(255);
    ofSetLineWidth(0.1);
    ofTranslate(mouseX, mouseY);
    ofCircle(0, 0, 1);
    int size = 5;
    int margin = 3;
    ofLine(-margin, 0, -margin-size, 0);
    ofLine(margin, 0, margin+size, 0);
    ofLine(0, -margin, 0, -margin-size);
    ofLine(0, margin, 0, margin+size);
    ofPopStyle();
    ofPopMatrix();
    
    //  TIME LINE
    //
    player.draw();
    
    //  LINES
    //
    ofSetColor(255);
    actualLine.draw();
    
    ofSetColor(180);
    for (int i = 0; i < storedLines.size(); i++){
        if ( i == selected) ofSetColor(255,0,0);
        else ofSetColor(255);
        
        storedLines[i].draw( player.getPositionMS()*0.001 );
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    float strokeJump = 0.1;
    
    if (key == ' '){
        if (player.getIsPlaying())
            player.pause();
        else
            player.play();
        
    } else if (key == 'f'){
        ofToggleFullscreen();
    } else if (key == OF_KEY_BACKSPACE){
        if (selected != -1){
            storedLines.erase(storedLines.begin()+selected);
            selected = -1;
        }
    } 
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if (player.inside(x, y)){
        
        //  If it was draing, finish the line
        //
        if (actualLine.size() > 1){
            cout << actualLine.size() << endl;
            actualLine.setFinish();
            storedLines.push_back(actualLine);
            actualLine.clear();
            cout << "clear and push" << endl;
        }
        
        //  If there is a selected refBox, move it
        //
        if (selected != -1){
            storedLines[selected].moveRefTo(x-offSetX,y-offSetY);
        } else
        
        //  If it's nothing selected, just move the header of the player
        //
        {
            player.setPosition((float)x/ofGetWidth());
            selected = -1;
        }
        
    } else {
        
        //  If it's on canvas, draw!
        //
        actualLine.addPoint(ofPoint(x,y));
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    //  Any new click could be de start of new gesture
    //
    selected = -1;
    
    if (player.inside(x, y)){
        
        //  If is inside the canvas, check if it's clicking a referenceBox
        //
        for (int i = 0; i < storedLines.size(); i++){
            if ( storedLines[i].checkOver(x, y) ){
                selected = i;
                offSetX = x - storedLines[i].getReferenceBox().x;
                offSetY = y - storedLines[i].getReferenceBox().y;
                break;
            }
        }
        
    } else {
        
        //  On canvas, draw something
        //
        actualLine.addPoint(ofPoint(x,y));
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if (player.inside(x, y)){
        
    } else {
        actualLine.setFinish();
        storedLines.push_back(actualLine);
        actualLine.clear();
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    for (int i = 0; i < storedLines.size(); i++){
        storedLines[i].updateBox();
    }
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}