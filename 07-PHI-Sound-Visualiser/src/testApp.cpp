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
    
    large = 500;
    cam.setDistance(large*2);
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    light.setPosition(ofVec3f(0,200,large*2.5));
    light.rotate(90, 1.0, 0.0, 0.0);
//    light.setDiffuseColor(ofFloatColor(0.9,0.8,0.0));
//    light.setAmbientColor(ofFloatColor(0.9,0.8,0.0));
    
    
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
    
    createTerrain(5);
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
	ofSetColor(255);
    //	for (int i = 0; i < (int)(bufferSize/2 - 1); i++){
    //		ofRect(100+(i*4),300,4,-freq[i]*10.0f);
    //	}
    //
    //	for (int i = 0; i < FFTanalyzer.nAverages; i++){
    //        ofSetColor(255);
    //		ofRect(100+(i*20),600,20,-FFTanalyzer.averages[i] * 6);
    //        ofSetHexColor(0xff0000);
    //        ofRect(100+(i*20),600-FFTanalyzer.peaks[i] * 6,20,-2);
    //	}
    
    cam.begin();
    ofEnableLighting();
    light.enable();
    glEnable(GL_DEPTH_TEST);
    mesh.drawFaces();
    glDisable(GL_DEPTH_TEST);
    light.disable();
    ofDisableLighting();
    
    ofPushMatrix();
    ofTranslate(0, 0, large*2.5 );
    arc(FFTanalyzer.averages, FFTanalyzer.nAverages, ofPoint(0,0), PI-0.65, 0.0f+0.65).draw();
    ofPopMatrix();
    
    cam.end();
}

//--------------------------------------------------------------
ofPolyline testApp::arc(float *values, int bufferSize, const ofPoint &center, float angleBegin, float angleEnd ){
    ofPolyline line;
    
    float angle = ofWrapRadians(angleBegin);
    float step = (ofWrapRadians(angleEnd) - angle)/((float)bufferSize);
    float scale = 1.0f;
    float minRadio = 100;
    
    ofPoint start = ofPoint(center.x + minRadio * cos(angle),
                            center.y + minRadio * sin(angle));
    ofPoint end = ofPoint(center.x + minRadio * cos(angleEnd),
                          center.y + minRadio * sin(angleEnd));
    
    line.addVertex( start );
    points.push_back(start);
    
    for (int i = 0; i < bufferSize; i++){
        
        float value = ofMap(values[i]*scale, 0.0, 60.0f, minRadio, minRadio+100.0f);
        ofPoint p = center;
        p.x += value * cos(angle);
        p.y += value * sin(angle);
        angle += step;
        
        line.curveTo( p );
        points.push_back( p );
    }
    line.addVertex( end );
    points.push_back( end );
    
    while(points.size()>bufferSize*large){
        points.erase(points.begin());
    }

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
void testApp::createTerrain( int _scale ){

    int width = FFTanalyzer.nAverages+2;
    int height = points.size() / width;
    
    mesh.clear();
    for(int y = 0; y < height - 1; y ++) {
		for(int x = 0; x < width - 1; x ++) {
            
			ofVec3f nw = points[ x + y * width] + ofPoint(0.0,0.0, (-height*0.5 + y) * _scale);
			ofVec3f ne = points[ (x+1) + y * width] + ofPoint(0.0,0.0, (-height*0.5 + y) * _scale);
			ofVec3f sw = points[ x + (y+1) * width] + ofPoint(0.0,0.0, (-height*0.5 + y+1) * _scale);
			ofVec3f se = points[ (x+1) + (y+1) * width] + ofPoint(0.0,0.0, (-height*0.5 + y+1) * _scale);
			
			addFace(mesh, nw, ne, se, sw);
		}
	}
    
    /*
    // Generate Vertex Field
    //
    int nVertexCount = (int) ( width * height * 6 );
    
    ofVec3f *pVertices      = new ofVec3f[nVertexCount];
    ofVec3f *pNormals       = new ofVec3f[nVertexCount];
    ofFloatColor *pColors   = new ofFloatColor[nVertexCount];
    
    int nIndex = 0;

    for( int nY = 0; nY < height-1 ; nY ++ ){
        for( int nX = 0; nX < width-1 ; nX++ ){
            float flX, flY;
            for( int nTri = 0; nTri < 6; nTri++ ){
                
                flX = (float) nX + ( ( nTri == 1 || nTri == 2 || nTri == 5 ) ? 1.0f : 0.0f );
                flY = (float) nY + ( ( nTri == 2 || nTri == 4 || nTri == 5 ) ? 1.0f : 0.0f );
                
                int i = flY*(width) + flX;
                
                pVertices[nIndex].x = points[ i ].x;
                pVertices[nIndex].y = points[ i ].y;
                pVertices[nIndex].z = (-height*0.5 + flY) * _scale ;
                
                // 3	0 --- 1		nTri reference
                // | \	  \	  |
                // |   \	\ |
                // 4 --- 5	  2
                
                //  Normals by vert
                ofVec3f a,b,c;
                ofVec3f normal;
                
                if ( nTri < 3 ){
                    a = points[ (nX+1) + nY* width];    //  1
                    b = points[ nX + nY* width];        //  0
                    c = points[ (nX+1) + (nY+1)* width];//  2
                } else {
                    a = points[ nX + nY* width];         //  3
                    b = points[ nX + (nY+1)* width];     //  4
                    c = points[ (nX+1) + (nY+1)* width]; //  5
                }
                pNormals[nIndex] = ((b - a).cross(c - a)).normalize();
                
                //  Colors by vert
                pColors[nIndex].set( ofMap(nY,0,height-1.0,0.1,1.0) );
                
                // Increment Our Index
                nIndex++;
            }
        }
    }
        
    mesh.clear();
    mesh.addVertices(pVertices, nVertexCount);
    mesh.addNormals(pNormals, nVertexCount);
    mesh.addColors(pColors, nVertexCount);
    
    delete [] pVertices; pVertices = NULL;
    delete [] pNormals; pNormals = NULL;
    delete [] pColors; pColors = NULL;
    */
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