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
    
    large = 2000;
//    cam.setDistance(large*2);
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    light.setPosition(ofVec3f(0,200,large*2.5));
    light.rotate(90, 1.0, 0.0, 0.0);
    
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
    
    //  Make an Arc with the FFT
    ofPolyline line = freqArc(FFTanalyzer.averages, FFTanalyzer.nAverages, ofPoint(0,0), PI-0.65, 0.0f+0.65,150-ofGetElapsedTimef()*5);
    int lineWidth = line.size();
    
    //  Copy the points from the arc to the mesh
    //
    matrix.rotate(1.0, 1.0, 0.0, 0.0);
    float s = 1.000;
    matrix.scale(s,s,s);

    for (int i = 0; i < lineWidth; i++){
        points.push_back( matrix*(line[i]+ofPoint( (TWO_PI/ofGetElapsedTimef())*lineWidth*20,0.0,0.0)));
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
    ofBackground(0);
    
	ofSetColor(255);
    
    cam.begin();
    ofEnableLighting();
    light.enable();
    glEnable(GL_DEPTH_TEST);
    
    ofPushMatrix();
    //ofTranslate(0.0, 0.0, -ofGetElapsedTimef()*500);
    mesh.drawFaces();
    ofPopMatrix();
    
    glDisable(GL_DEPTH_TEST);
    light.disable();
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