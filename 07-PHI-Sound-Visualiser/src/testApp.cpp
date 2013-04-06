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
    
    large = 1000;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    coldLight.setPosition(ofVec3f(-20,-200,-large*2.5));
    coldLight.rotate(90+45, 1.0, 0.0, 0.0);
    coldLight.setDiffuseColor(ofFloatColor(0.1,0.1,0.2));
    coldLight.setSpotConcentration(0.001);
    
    warmLight.setPosition(ofVec3f(20,200, large*2.5));
    warmLight.rotate(45, 1.0, 0.0, 0.0);
    warmLight.setDiffuseColor(ofFloatColor(0.7,0.7,0.35));
    warmLight.setSpecularColor(ofFloatColor(0.2,0.2,0.1));
    
    angle = 0.0;
    radio = 1.0;
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
    
    float phi   = (1.0+sqrtf(5.0))/2.0;
    float grow  = (1.0+phi);
    
    float stepAngle = (TWO_PI/ofGetFrameRate());
    float stepGrow = grow/ofGetFrameRate();
    
    angle += stepAngle*0.001;
    radio += (radio*stepGrow)*0.1;
    
    float size = radio*grow;
    cam.setDistance(10+radio*3);
    
    offSet = ofPoint(radio*(1.0+phi),-(size*0.5));
    
    //  Make an Arc with the FFT
    //
    ofPolyline line = freqArc(FFTanalyzer.averages, FFTanalyzer.nAverages, offSet, PI-0.65, 0.0f+0.65, size);
    int lineWidth = line.size();
    
    //  Copy the points from the arc to the mesh
    //
    matrix.rotate(1.0, ofRadToDeg(angle), 0.0, 0.0);

    for (int i = 0; i < lineWidth; i++){
        points.push_back( matrix*line[i] );
    }
    
    //  Delete the excess of information
    //
    while(points.size()>lineWidth*large){
        points.erase(points.begin());
    }
    
    createSkin(lineWidth);
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
	ofSetColor(255);
    
    cam.begin();
    ofEnableLighting();
    coldLight.enable();
    warmLight.enable();
    
    glEnable(GL_DEPTH_TEST);
    
    ofPushMatrix();
    
    ofTranslate(0,offSet.x*0.5);
    ofRotate(-90, 0, 0, 1);
    
    ofSetColor(255,250,240);
    mesh.drawFaces();
    ofPopMatrix();
    
    glDisable(GL_DEPTH_TEST);
    
    coldLight.disable();
    warmLight.disable();
    ofDisableLighting();
    cam.end();
}

//--------------------------------------------------------------
ofPolyline testApp::freqArc(float *_values, int _size, const ofPoint &_center, float _angleBegin, float _angleEnd, float _minRad,  bool _bSmooth){
    
    ofPolyline line;
    
    float angle = ofWrapRadians(_angleBegin);
    float step = (ofWrapRadians(_angleEnd) - angle)/((float)_size);
    float scale = 1.0f;
    
    ofPoint start = ofPoint(_center.x + _minRad * cos(angle),
                            _center.y + _minRad * sin(angle));
    ofPoint end = ofPoint(_center.x + _minRad * cos(_angleEnd),
                          _center.y + _minRad * sin(_angleEnd));
    
    line.addVertex( start );
    for (int i = 0; i < _size; i++){
        
        float value = ofMap(_values[i]*scale, 0.0, 60.0f, _minRad, _minRad*2);
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
void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	ofVec3f normal = ((b - a).cross(c - a)).normalize() * -1.0;
	mesh.addNormal(normal);
	mesh.addVertex(a);
	mesh.addNormal(normal);
	mesh.addVertex(b);
	mesh.addNormal(normal);
	mesh.addVertex(c);
}

//--------------------------------------------------------------
void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

//--------------------------------------------------------------
ofVec3f getVertexFromImg(ofFloatImage& img, int x, int y) {
	float scaleFactor = 100;
	return ofVec3f(x, y, 100 * img.getColor(x, y).getBrightness());
}

//--------------------------------------------------------------
void testApp::createSkin(int _width){

    int width = _width;
    int height = points.size() / width;
    float scale = 5.0f;
    
    mesh.clear();
    for(int y = 0; y < height - 1; y ++) {
		for(int x = 0; x < width - 1; x ++) {
            
			ofVec3f nw = points[ x + y * width];// + ofPoint(0.0,0.0, (-height*0.5 + y) * scale);
			ofVec3f ne = points[ (x+1) + y * width];// + ofPoint(0.0,0.0, (-height*0.5 + y) * scale);
			ofVec3f sw = points[ x + (y+1) * width];// + ofPoint(0.0,0.0, (-height*0.5 + y+1) * scale);
			ofVec3f se = points[ (x+1) + (y+1) * width];// + ofPoint(0.0,0.0, (-height*0.5 + y+1) * scale);
			
			addFace(mesh, nw, ne, se, sw);
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
void testApp::mouseReleased(){

}