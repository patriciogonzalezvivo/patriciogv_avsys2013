#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofSetFullscreen(true);
    
    //  Set audio
    //
    soundStream.setup(this, 0, 1, 44100, 256, 4);
    sampleSmooth = 0;
    bSignOflastValue = false;
    volume = 0;
    pitch = 0;
    volumeSmoothSlow = 0;
    pitchSmoothSlow = 0;
    
    //  Set Graphics
    //
    width = ofGetScreenWidth();
    height = ofGetScreenHeight();
    glow.allocate(width, height);
    chromaAb.allocate(width, height);
    nFaces = 2;
    mLeft.setMode(OF_PRIMITIVE_LINE_STRIP);
//    mRight.setMode(OF_PRIMITIVE_LINE_STRIP);
    audioMeshAlpha = 0.0;
    
    mLeft.clear();
//    mRight.clear();
}

//--------------------------------------------------------------
void testApp::update(){
    glow.begin();
    ofPushMatrix();
    ofTranslate(width*0.5, height*0.5);
    
    ofSetColor(255);
    mLeft.draw();
//    mRight.draw();
    
    ofSetColor( 255, 50 + ofMap(volume,0.1,0,50,255) );
    ofRect(-50, -50, 100, 100);
    
    ofPopMatrix();
    glow.end();
    
    glow.setRadius(ofMap( pitchSmoothSlow, 2000,6000, 1,5, true ));
    glow.update();
    chromaAb << glow;
    chromaAb.offset = ofMap( pitchSmoothSlow, 2000,6000, 0,100, true ) * volumeSmoothSlow * 10;
    chromaAb.update();
    
    if (audioMeshAlpha < 1.0){
        audioMeshAlpha = ofLerp(audioMeshAlpha, 1.0, 0.0005);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofSetColor(255);
    
    if ( nFaces == 1){
        
        chromaAb.draw(0,0);
        
    } else if (nFaces == 2){
        //  Mirror
        //
        
        chromaAb.getTextureReference().bind();
        glBegin(GL_QUADS);
        //  First half
        //
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(width*0.5, 0); glVertex3f(width*0.5, 0, 0);
        glTexCoord2f(width*0.5, height); glVertex3f(width*0.5, height, 0);
        glTexCoord2f(0,height);  glVertex3f(0,height, 0);
        
        //  Second half
        //
        glTexCoord2f(width*0.5, 0); glVertex3f(width*0.5, 0, 0);
        glTexCoord2f(0, 0); glVertex3f(width, 0, 0);
        glTexCoord2f(0, height); glVertex3f(width, height, 0);
        glTexCoord2f(width*0.5,height);  glVertex3f(width*0.5,height, 0);
        glEnd();
        chromaAb.getTextureReference().unbind();
        
    } else if (nFaces >= 3){
        
        //  Kaleidoscope
        //
        ofVec2f center = ofVec2f(width*0.5, height*0.5);
        
        float   angleStep = TWO_PI/nFaces;
        float   angle = -angleStep*0.5;
        ofVec2f radio = ofVec2f(-center.x,0);
        
        ofVec2f p[3];
        p[0] = radio.getRotated(ofRadToDeg(angle)) + center;
        p[1] = radio.getRotated(ofRadToDeg(angle+angleStep)) + center;
        
        ofMesh mesh;
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        mesh.addTexCoord( center );
        mesh.addVertex( center );
        for(int i = 0; i < nFaces+1; i++){
            mesh.addTexCoord( p[i%2] );
            ofVec2f e = radio.getRotated(ofRadToDeg(angle)+ofRadToDeg(angleStep)*i) + center;
            mesh.addVertex(e);
        }
        
        chromaAb.getTextureReference().bind();
        mesh.draw();
        chromaAb.getTextureReference().unbind();
    }
    
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){
    
    float rms = 0;
    int zeroCrossingCount = 0;
    
    for (int i = 0; i < bufferSize; i++){
        
        //  Root Mean Square
        //  http://en.wikipedia.org/wiki/Root_mean_square
        //
        rms += input[i] * input[i];
        
        //  Low pass filtering
        //  http://www.cplusplus.com/forum/articles/14631/
        //
        sampleSmooth = 0.9f * sampleSmooth + 0.1 * input[i];
        bool curSign = input[i] > 0 ? true : false;
        if (curSign != bSignOflastValue)
            zeroCrossingCount++;
        bSignOflastValue = curSign;
        
        //  Store Values
        //
        inLeft.push_back(   input[i*nChannels]*0.5  );
//        inRight.push_back(  input[i*nChannels+1]*0.5);
    }
    
    //  Compute RMS
    //
    rms /= (float)bufferSize;
    rms = sqrt(rms);
    volume = 0.9f * volume + 0.1 * rms;
    volumeSmoothSlow = 0.99f * volumeSmoothSlow + 0.01 * rms;
    
    //  Compute pitch
    //
    float pitchVal = zeroCrossingCount * (44100.0 / (float)bufferSize) * (0.5);
    pitch = 0.9f * pitch + 0.1 * pitchVal;
    pitchSmoothSlow = 0.99f * pitchSmoothSlow + 0.01f * pitchVal;
    
    while (inLeft.size() > width*0.5) {
        inLeft.erase(inLeft.begin());
//        inRight.erase(inRight.begin());
    }
    
    mLeft.clear();
//    mRight.clear();
    for (int i = 0; i < inLeft.size() ; i++){
        float x = ofMap(i, 0, inLeft.size(), -width*0.5, 0);
        float alpha = ofMap(i, 0, inLeft.size(), 0.0, 1.0) * audioMeshAlpha;
        float scale = ofMap(1.0-alpha,0.0,1.0,height,180.0);
        
        mLeft.addColor( ofFloatColor(1.0, alpha) );
        mLeft.addVertex(ofPoint(x, inLeft[i]*scale));
        
//        x = ofMap(i, inLeft.size(), 0, 0, width*0.5);
//        mRight.addColor( ofFloatColor(1.0, alpha) );
//        mRight.addVertex(ofPoint(x, inLeft[i]*scale));
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == OF_KEY_LEFT){
        nFaces--;
    } else if (key == OF_KEY_RIGHT){
        nFaces++;
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

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}