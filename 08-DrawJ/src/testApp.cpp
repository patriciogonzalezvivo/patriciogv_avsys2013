#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    bPDF = false;
    bCursor = true;
    ofHideCursor();
    
    nSelected = -1;
    lineStroke = 1;
    
    loadSecuence("test.xml");
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    //  Cursor
    //
    if (bCursor){
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
    }
    
    if (!bLoaded){
        ofPushStyle();
        ofSetColor(255,0,0);
        ofDrawBitmapString("DRAG me MUSIC", ofGetWidth()*0.5-50, ofGetHeight()*0.5-4);
//        ofNoFill();
//        float w = 160;
//        float h = w*((1.0+sqrtf(5.0))/2.0-1.0);
//        ofRectRounded(ofGetWidth()*0.5-w*0.5, ofGetHeight()*0.5-h*0.5, w, h, 1);
//        ofPopStyle();
    }
    
    //  TIME LINE
    //
    player.draw();
    
    if (bPDF){
        ofBackground(255);
        ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
    }
    
    //  LINES
    //
    ofEnableSmoothing();
    ofSetColor(255);
    actualLine.draw();
    
    ofSetColor(180);
    float time = player.getPosition()*player.getLength();
    for (int i = 0; i < storedLines.size(); i++){
        if (bPDF) ofSetColor(0);
        else if ( i == nSelected) ofSetColor(255,0,0);
        else ofSetColor(255);
        storedLines[i].draw( time , bPDF );
    }

    ofDisableSmoothing();
    
    if (bPDF){
        ofEndSaveScreenAsPDF();
        bPDF = false;
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
        if (nSelected != -1){
            storedLines.erase(storedLines.begin()+nSelected);
            nSelected = -1;
        }
    } else if (key == 's'){
        saveSecuence("secuence-"+ofGetTimestampString()+".xml");
    } else if( key == 'p'){
        if (!bPDF)
            bPDF = true;
    } else if (key == 'c'){
        storedLines.clear();
        actualLine.clear();
    } else if (key == 'b'){
        if (player.getIsPlaying())
            player.addBookmark();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    if (player.inside(x, y)){
        
        if (bCursor)
            ofShowCursor();
        
        bCursor = false;
    } else {
        if (!bCursor)
            ofHideCursor();
        
        bCursor = true;
    }
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
        if (nSelected != -1){
            storedLines[nSelected].moveRefTo(x-selectOffSet.x,y-selectOffSet.y);
        } else
        
        //  If it's nothing selected, just move the header of the player
        //
        {
            player.setPosition((float)x/ofGetWidth());
            nSelected = -1;
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
    nSelected = -1;
    
    if (player.inside(x, y)){
        
        //  If is inside the canvas, check if it's clicking a referenceBox
        //
        for (int i = 0; i < storedLines.size(); i++){
            if ( storedLines[i].checkOver(x, y) ){
                nSelected = i;
                selectOffSet.x = x - storedLines[i].getReferenceBox().x;
                selectOffSet.y = y - storedLines[i].getReferenceBox().y;
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
    loadSecuence(dragInfo.files[0]);
}

bool testApp::loadSecuence(string _file){
    vector<string> file = ofSplitString(_file, ".");
    
    if ( file[1] == "mp3" ) {
        bLoaded = player.loadSound( _file );
        if (bLoaded){
            soundFile = _file;
        }
    } else if ( file[1] == "xml" ){
        ofxXmlSettings XML;
        
        if (XML.loadFile(_file)){
            soundFile = XML.getValue("soundFile", "test.mp3");
            bLoaded = player.loadSound( soundFile );
            
            if (bLoaded){
                actualLine.clear();
                storedLines.clear();
                int totalLines = XML.getNumTags("line");
                for (int i = 0; i < totalLines; i++) {
                    XML.pushTag("line",i);
                    
                    float lStroke = XML.getValue("stroke", 0.0f);
                    float lSTime = XML.getValue("startTime", 0.0f);
                    
                    int totalPnts = XML.getNumTags("pnt");
                    vector<TimePoint> lPnts;
                    for (int j = 0; j < totalPnts; j++){
                        TimePoint newPoint;
                        newPoint.x = XML.getValue("pnt:x", 0.0, j);
                        newPoint.y = XML.getValue("pnt:y", 0.0, j);
                        newPoint.z = XML.getValue("pnt:z", 0.0, j);
                        newPoint.time = XML.getValue("pnt:t", 0.0,j);
                        lPnts.push_back(newPoint);
                    }
                    
                    Line newLine = Line(lPnts, lSTime, lStroke);
                    newLine.setVisualPlayer(&player);
                    storedLines.push_back( newLine );
                    
                    XML.popTag();
                }
                int totalBookmark = XML.getNumTags("bookmarks");
                for (int i = 0; i < totalBookmark; i++) {
                    player.bookmarks.push_back(XML.getValue("bookmarks", 0.0, i));
                }
            }
        }
    }
    
    if (bLoaded){
        player.play();
        actualLine.setVisualPlayer( &player );
        actualLine.stroke = lineStroke;
        ofSetFullscreen(true);
    }
}

bool testApp::saveSecuence(string _file){
    ofxXmlSettings XML;
    
    XML.addValue("soundFile", soundFile);
    
    int totalLines = storedLines.size();
    for(int i = 0; i < totalLines; i++){
        XML.addTag("line");
    }
    
    for(int i = 0; i < totalLines; i++){
        XML.pushTag("line",i);
        
        XML.addValue("stroke", storedLines[i].stroke );
        XML.addValue("startTime", storedLines[i].getStartTime() );
        
        int totalPnts = storedLines[i].size();
        for(int j = 0; j < totalPnts; j++){
            XML.addTag("pnt");
        }
        
        for(int j = 0; j < totalPnts; j++){
            XML.setValue("pnt:x", storedLines[i][j].x , j);
            XML.setValue("pnt:y", storedLines[i][j].y , j);
            XML.setValue("pnt:z", storedLines[i][j].z , j);
            XML.setValue("pnt:t", storedLines[i][j].time, j);
        }
        
        XML.popTag();
    }
    
    int totalBookmarks = player.bookmarks.size();
    for(int i = 0; i < totalBookmarks; i++){
        XML.addTag("bookmark");
    }
    
    for(int i = 0; i < totalBookmarks; i++){
        XML.setValue("bookmark", player.bookmarks[i] , i);
    }
    
    XML.saveFile(_file);
}