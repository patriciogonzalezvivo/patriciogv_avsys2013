#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    // 0 output channels,
	// 2 input channels
	// 44100 samples per second
	// bufferSize(512) samples per buffer
	// 4 num buffers (latency)
    //
    bufferSize = 512;
	ofSoundStreamSetup(0,2,this, 44100, bufferSize, 4);
	
	left        = new float[bufferSize];
	right       = new float[bufferSize];
    magnitude   = new float[bufferSize];
    phase       = new float[bufferSize];
    power       = new float[bufferSize];
    freq        = new float[bufferSize/2];
    
	FFTanalyzer.setup(44100, bufferSize/2, 2);
	FFTanalyzer.peakHoldTime        = 15;       // hold longer
	FFTanalyzer.peakDecayRate       = 0.95f;    // decay slower
	FFTanalyzer.linearEQIntercept   = 0.9f;     // reduced gain at lowest frequency
	FFTanalyzer.linearEQSlope       = 0.01f;    // increasing gain at higher frequencies
    
    //  TimeLine
    //
    nBookmark = -1;
    nSelected = -1;
    lineStroke = 1.0;
    loadSecuence("test.xml");
    
    player.fft = &FFTanalyzer;
    
    bCursor = true;
    bFlipped = false;
    ofHideCursor();
}

//--------------------------------------------------------------
void testApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    //  Do the FFT
    //
    float avg_power = 0.0f;
	myFft.powerSpectrum(0,(int)bufferSize/2, left, bufferSize, magnitude, phase, power, &avg_power);
	for (int i = 0; i < (int)(bufferSize/2); i++){
		freq[i] = magnitude[i];
	}
    FFTanalyzer.calculate(freq);
    
    for (int i = 0; i < storedLines.size(); i++){
        storedLines[i].updateBox();
    }
    
    player.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    float time = player.getPosition()*player.getLength();
    
    //  TIME LINE
    //
    player.draw();
    
    //  Current lineStroke
    //
    ofPushMatrix();
    ofSetColor(255,0,0,100);
    ofTranslate(0, ofGetHeight() - player.height);
    float strokeY = ofMap(lineStroke, 0.0,MAX_LINE_WIDTH, player.height*0.9-REFERENCEBOX_HEIGHT*0.5,player.height*0.1+REFERENCEBOX_HEIGHT*0.5);
    ofBeginShape();
    ofVertex(0, strokeY-MARK_SIZE);ofVertex(0, strokeY+MARK_SIZE);ofVertex(MARK_SIZE, strokeY);
    ofEndShape(false);
    ofLine(MARK_SIZE, strokeY, ofGetWidth(), strokeY);
    ofPopMatrix();
    
    //  LINES
    //
    ofEnableSmoothing();
    
    ofSetColor(255);
    actualLine.draw(time);
    
    ofSetColor(180);
    for (int i = 0; i < storedLines.size(); i++){
        
        storedLines[i].bSelected = false;
        
        if ( i == nSelected){
            ofSetColor(255,0,0);
            storedLines[i].bSelected = true;
        } else {
            ofSetColor(255);
        }
        
        storedLines[i].draw( time );
    }

    ofDisableSmoothing();
    
    //  Cursor
    //
    if (bCursor){
        ofPushMatrix();
        ofPushStyle();
        ofSetColor(255);
        ofSetLineWidth(0.1);
        
        if (bFlipped)
            ofTranslate(ofGetScreenWidth()-mouseX, mouseY);
        else
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
    
    ofSetColor(255,0,0,0);
	for (int i = 0; i < FFTanalyzer.nAverages; i++){
		ofRect(200+(i*20),600-FFTanalyzer.peaks[i] * 6,20,-4);
	}
}

//--------------------------------------------------------------
void testApp::audioReceived (float * input, int bufferSize, int nChannels){
	
    //  Samples are "interleaved"
    //
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
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
    } else if (key == 'e'){
        player.bEdit = !player.bEdit;
    } else if (key == OF_KEY_BACKSPACE){
        if (nSelected != -1){
            storedLines.erase(storedLines.begin()+nSelected);
            nSelected = -1;
        } else if (nBookmark != -1){
            player.bookmarks.erase(player.bookmarks.begin()+nBookmark);
            nBookmark = -1;
        }
    } else if (key == 's'){
//        saveSecuence("secuence-"+ofGetTimestampString()+".xml");
        saveSecuence("test.xml");
    } else if (key == 'c'){
        if ((storedLines.size() == 0) && (actualLine.size() <= 2))
            player.bookmarks.clear();
            
        storedLines.clear();
        actualLine.clear();
    } else if (key == 'v'){
        player.bookmarks.clear();
        
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
    if (player.inside(x, y) && player.bEdit){
        
        //  If it was draing, finish the line
        //
        if (actualLine.size() > 1){
            actualLine.setFinish();
            storedLines.push_back(actualLine);
            actualLine.clear();
        }
        
        //  If there is a selected refBox, move it
        //
        if (nSelected != -1){
            storedLines[nSelected].moveRefTo(x-selectOffSet.x,y-selectOffSet.y);
        }
        //  If a bookmark is selected
        //
        else if (nBookmark != -1){
            player.bookmarks[nBookmark] = (float)x/(float)ofGetWidth();
        }
        //  What about LINE STROKE
        //
        else if ( x < MARK_SIZE*2.0 ){
            float strokeY = ofMap(lineStroke,
                                  0.0,MAX_LINE_WIDTH,
                                  player.height*0.9-REFERENCEBOX_HEIGHT*0.5,
                                  player.height*0.1+REFERENCEBOX_HEIGHT*0.5);
            float mY = y - (ofGetHeight()-player.height);
            if ( abs(strokeY - mY) < MARK_SIZE*2.0 ){
                lineStroke = ofMap(mY,
                                   player.height*0.9-REFERENCEBOX_HEIGHT*0.5,
                                   player.height*0.1+REFERENCEBOX_HEIGHT*0.5,
                                   0.0,
                                   MAX_LINE_WIDTH,
                                   true);
                actualLine.stroke = lineStroke;
            }
        }
        //  If it's nothing selected, just move the header of the player
        //
        else {
            player.setPosition((float)x/ofGetWidth());
            nSelected = -1;
            nBookmark = -1;
        }
        
    } else {
        
        //  If it's on canvas, draw!
        //
        
        if (bFlipped)
            actualLine.addPoint(ofPoint(ofGetScreenWidth()-x,y));
        else
            actualLine.addPoint(ofPoint(x,y));
        
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    //  Any new click could be de start of new gesture
    //
    nSelected = -1;
    nBookmark = -1;
    
    if (player.inside(x, y) && player.bEdit){
        
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
        
        //  If no reference box is selected try with a bookmark
        //
        if (nSelected == -1){
            for (int i = 0; i < player.bookmarks.size(); i++){
                int mark= player.bookmarks[i] * ofGetWidth();
                if ( x > mark-5 && x < mark+5 ){
                    nBookmark = i;
                    break;
                }
            }
        } 
        
    } else {
        
        //  On canvas, draw something
        //
        if (bFlipped)
            actualLine.addPoint(ofPoint(ofGetScreenWidth()-x,y));
        else
            actualLine.addPoint(ofPoint(x,y));
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if (player.inside(x, y) && player.bEdit ){
        
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
    
    if ( file[1] == "xml" ){
        ofxXmlSettings XML;
        
        if (XML.loadFile(_file)){
            player.setLength(XML.getValue("length", 1.0));
            
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
                    newPoint.angle = XML.getValue("pnt:a", 0.0,j);
                    lPnts.push_back(newPoint);
                }
                
                Line newLine = Line(lPnts, lSTime, lStroke);
                newLine.setTimeLine(&player);
                storedLines.push_back( newLine );
                
                XML.popTag();
            }
            
            int totalBookmark = XML.getNumTags("bookmark");
            for (int i = 0; i < totalBookmark; i++) {
                player.bookmarks.push_back(XML.getValue("bookmark", 0.0, i));
            }
        }
        
        player.play();
        actualLine.setTimeLine( &player );
        actualLine.stroke = lineStroke;
//        ofSetFullscreen(true);
    }
}

bool testApp::saveSecuence(string _file){
    ofxXmlSettings XML;
    
    XML.addValue("length", player.getLength());
    
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
            XML.setValue("pnt:a", storedLines[i][j].angle, j);
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