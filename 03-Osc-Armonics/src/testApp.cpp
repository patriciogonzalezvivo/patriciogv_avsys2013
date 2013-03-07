#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
    ofEnableSmoothing();
    
    for (int i = 0; i < TOTAL_PARTICLES; i++){
        wParticle newParticle;
        
        if ( i != 0){
            newParticle.sound.loadSound("synth.wav");
        }
        particles.push_back(newParticle);
    }
    
    speed = 0.5;
}

//--------------------------------------------------------------
void testApp::update(){
	ofSoundUpdate();
    
	for (int i = 0; i < particles.size(); i++){
		float x = (ofGetWidth()/(particles.size()+1))*(i+1);
		float y = (ofGetHeight()*0.25) * -sin( ofGetElapsedTimef() * (i / (float)(particles.size())) * speed );
		
		particles[i].moveTo(x,y);
	}
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray,ofColor::black);
    
    ofPushMatrix();
    ofTranslate(0, ofGetHeight()*0.5);
    
    float sinSwitcher = abs(sin(ofGetElapsedTimef()*0.1));
    
    drawParticlesLine(1.0-sinSwitcher);

    
    for (int i = 0; i < particles.size(); i++){
        
        particles[i].drawTrail(sinSwitcher*0.5);
        
        for ( int j = i+1; j < particles.size(); j++){
            
            if ( (j < i+20) ){
                int dist = particles[i].distance(particles[j]);
                
                if ( (dist < 100) ){
                    ofSetColor(255, 100 );
                    ofLine(particles[i], particles[j]);
                }
            }
        }
        
        particles[i].drawDot();
    }
    
    ofPopMatrix();
    
    ofSetColor(200);
    ofDrawBitmapString( "Particles: " + ofToString(particles.size()) + " ( UP / DOWN )", 15,15);
    ofDrawBitmapString( "Speed: " + ofToString(speed) + " ( LEFT / RIGHT )", 15,30);
}

void testApp::drawParticlesLine(float _alpha){
    ofPolyline macroLine;
    macroLine.addVertex(particles[0]);
    for (int i = 0 ; i < particles.size() ; i++){
        macroLine.curveTo(particles[i]);
    }
    macroLine.curveTo(particles[particles.size()-1]);
    macroLine = macroLine.getResampledByCount( 1000 );
    
    ofSetColor(255);
    ofMesh lineMesh;
    lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    for (int i = 0 ; i < macroLine.getVertices().size() ; i++){
        lineMesh.addColor(ofFloatColor(1.0, _alpha));
        lineMesh.addVertex(macroLine.getVertices()[i]);
    }
    ofSetColor(255);
    lineMesh.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'f'){
        ofToggleFullscreen();
    } else if (key == OF_KEY_RIGHT){
        speed += 0.01;
    } else if (key == OF_KEY_LEFT){
        speed -= 0.01;
        
        if (speed < 0)
            speed = 0;
    } else if ( key == OF_KEY_UP){
        wParticle newParticle;
        newParticle.sound.loadSound("synth.wav");
        particles.push_back(newParticle);
    } else if ( key == OF_KEY_DOWN){
        particles.erase(particles.end()-1);
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