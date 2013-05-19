#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
	// 0 output channels, 
	// 2 input channels
	// 44100 samples per second
	// bufferSize(512) samples per buffer
	// 4 num buffers (latency)
    //
    bufferSize = 512;
	
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
    
    large = 1200;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    cam.setOrientation(ofVec3f(52.5306, 58.7509, -16.9383));
    
    coldLight.setPosition(ofVec3f(0,-500,-large*100));
    coldLight.rotate(-45, 1.0, 0.0, 0.0);
    coldLight.setDiffuseColor(ofFloatColor(0.07,0.01,0.01));
    coldLight.setDirectional();
    
    warmLight.setPosition(ofVec3f(-100,500,-large));
    warmLight.rotate(45, 1.0, 0.0, 0.0);
    warmLight.setDirectional();
    warmLight.setDiffuseColor(ofFloatColor(0.7,0.7,0.35));
    
    ofToggleFullscreen();
    dof = 11;
    
    sServer.setName("Shell");
    ofSoundStreamSetup(0,2,this, 44100, bufferSize, 4);
}


//--------------------------------------------------------------
void testApp::update(){
    
    //  Do the FFT
    //
    float avg_power = 0.0f;
	myFft.powerSpectrum(0,(int)bufferSize/2, left, bufferSize, magnitude, phase, power, &avg_power);
	for (int i = 0; i < (int)(bufferSize/2); i++){
		freq[i] = magnitude[i];
	}
    FFTanalyzer.calculate(freq);
    
    //  Add FFT averages
    //
    vector<float> newAverage;
    newAverage.assign(FFTanalyzer.averages,FFTanalyzer.averages+18);
    if(averages.size()>0){
        for(int i = 0; i < newAverage.size(); i++){
            newAverage[i] /= ofNoise(ofGetElapsedTimef()*0.1,(float)i/((float)newAverage.size()))*100;
        }
    }
    newAverage.push_back(ofNoise(ofGetElapsedTimef()*0.1));
    newAverage.push_back(ofNoise(ofGetElapsedTimef()*0.1));
    averages.push_back( newAverage );
    while(averages.size()>large){
        averages.erase(averages.begin());
    }
    
    vector<float> newPeaks;
    newPeaks.assign(FFTanalyzer.peaks,FFTanalyzer.peaks+18);
    peaks.push_back( newPeaks );
    while(peaks.size()>large){
        peaks.erase(peaks.begin());
    }
    
    
    //  Align and rotate
    //
    float phi   = (1.0+sqrtf(5.0))/2.0;
    float grow  = (1.0+phi);
    float stepGrow  = grow*0.001;
    float radio     = 1.0;
    offSet          = ofPoint(0,0);
    
    int lineWidth;
    points.clear();
    
    ofMatrix4x4 matrix;
    for (int i = 0; i < averages.size(); i++){
        radio += radio*stepGrow;
        float size = radio*grow;
        offSet = ofPoint(size,size*-0.5);
        
        ofPolyline line = freqArc( peaks[i], offSet, PI-0.65, 0.0f+0.65, size );
        line = line.getSmoothed(1);
        lineWidth = line.size();
        
        matrix.rotate(1.0, 1.0 , 0.0, 0.0);
        for (int j = 0; j < lineWidth; j++){
            points.push_back( matrix*line[j] );
        }
    }
    
    //  Make the mesh
    //
    createSkin(lineWidth);
    
    //  Set the camera
    //
    depthOfField.setFocalDistance(dof);
    
    if (averages.size() < 1190){
        cam.setDistance(offSet.x*2.0);
    }
//    else {
//        cam.setDistance(117.254);
//    }
//    
//    cam.setOrientation( ofVec3f(sin(ofGetElapsedTimef()*0.1)*180,
//                                0.0,
//                                cos(ofGetElapsedTimef()*0.05)*45 ));
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
//    ofBackgroundGradient(ofColor::gray, ofColor::black);
//    ofBackground(0);
    
	ofSetColor(255);
    
    depthOfField.begin();
    cam.begin(depthOfField.getDimensions());
    ofEnableLighting();
    coldLight.enable();
    warmLight.enable();
    
    glEnable(GL_DEPTH_TEST);
    ofPushMatrix();
    ofRotateY((ofGetElapsedTimef()*0.1)*180);
    ofRotateX(sin(ofGetElapsedTimef()*0.1)*90);
    
//    ofRotateZ((ofGetElapsedTimef()*0.1)*180);
    
    ofTranslate(0,offSet.x*0.7);
    ofRotate(-90, 0, 0, 1);
    ofSetColor(255,250,240);
    if(ofGetKeyPressed( 'm' )){
        mesh.drawWireframe();
    } else {
        mesh.drawFaces();
    }
    ofPopMatrix();
    glDisable(GL_DEPTH_TEST);
    
    coldLight.disable();
    warmLight.disable();
    ofDisableLighting();
    ofPopMatrix();
    cam.end();
    depthOfField.end();
    
    
    ofPushMatrix();
	if(ofGetKeyPressed( 'a' )){
		depthOfField.drawFocusAssist(0, 0);
        
    } else{
		depthOfField.getFbo().draw(0, 0);
	}
    ofPopMatrix();
    
    
    sServer.publishScreen();
}

//--------------------------------------------------------------
ofPolyline testApp::freqArc( vector<float> &_vector, const ofPoint &_center, float _angleBegin, float _angleEnd, float _minRad,  bool _bSmooth){
    
    ofPolyline line;
    
    float angle = ofWrapRadians(_angleBegin);
    float step = (ofWrapRadians(_angleEnd) - angle)/((float)_vector.size());
    float scale = 1.0f;
    
    ofPoint start = ofPoint(_center.x + _minRad * cos(angle),
                            _center.y + _minRad * sin(angle));
    ofPoint end = ofPoint(_center.x + _minRad * cos(_angleEnd),
                          _center.y + _minRad * sin(_angleEnd));
    
    line.addVertex( start );
    for (int i = 0; i < _vector.size(); i++){
        
        float value = ofMap(_vector[i]*scale, 0.0, 60.0f, _minRad, _minRad*2);
        ofPoint p = _center;
        p.x += value * cos(angle);
        p.y += value * sin(angle);
        angle += step;
        
        if (_bSmooth){
            line.curveTo( p );
        } else {
            line.addVertex(p);
        }
    }
    line.addVertex( end );
    
    return line;
}


//--------------------------------------------------------------
void addFace(ofMesh& mesh,
             ofVec3f a, ofFloatColor aC,
             ofVec3f b, ofFloatColor bC,
             ofVec3f c, ofFloatColor cC) {
    
	ofVec3f normal = ((b - a).cross(c - a)).normalize() * -1.0;
    
	mesh.addNormal(normal);
    mesh.addColor(aC);
	mesh.addVertex(a);
    
	mesh.addNormal(normal);
    mesh.addColor(bC);
	mesh.addVertex(b);
    
	mesh.addNormal(normal);
	mesh.addColor(cC);
    mesh.addVertex(c);
}

//--------------------------------------------------------------
void addFace(ofMesh& mesh,
             ofVec3f a, ofFloatColor aC,
             ofVec3f b, ofFloatColor bC,
             ofVec3f c, ofFloatColor cC,
             ofVec3f d, ofFloatColor dC) {
    
	addFace(mesh, a, aC, b, bC, c, cC);
	addFace(mesh, a, aC, c, cC, d, dC);
}

//--------------------------------------------------------------
void testApp::createSkin(int _width){

    int width = _width;
    int height = points.size() / width;
    float scale = 5.0f;
    
    mesh.clear();
    for(int y = 0; y < height - 1; y ++) {
		for(int x = 0; x < width - 1; x ++) {
            
			ofVec3f nw = points[ x + y * width];
            ofFloatColor nwC = ofFloatColor( 0.5 + averages[y][x] * 0.5);
            
			ofVec3f ne = points[ (x+1) + y * width];
            ofFloatColor neC = ofFloatColor( 0.5 + averages[y][x] * 0.5);
            
			ofVec3f sw = points[ x + (y+1) * width];
            ofFloatColor swC = ofFloatColor( 0.5 + averages[y][x] * 0.5);
            
			ofVec3f se = points[ (x+1) + (y+1) * width];
			ofFloatColor seC = ofFloatColor( 0.5 + averages[y][x] * 0.5);
            
			addFace(mesh,
                    nw, nwC,
                    ne, neC,
                    se, seC,
                    sw, swC);
		}
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
void testApp::keyPressed  (int key){ 
	if (key == 'f'){
        ofToggleFullscreen();
    } else if ( key == ' '){
        averages.clear();
    } else if ( key == OF_KEY_UP){
        dof++;
    } else if ( key == OF_KEY_DOWN){
        dof--;
    }
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
    depthOfField.setup(w, h);
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}