#include "testApp.h"
#define STRINGIFY(A) #A

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    img.loadImage("test01.jpg");
    
    string fragShader = STRINGIFY(uniform sampler2DRect srcTexture;
                                  uniform sampler2DRect prevTexture;
                                  uniform vec2  mouse;
                                  uniform float offset;
                                  uniform float dispersion;
                                  uniform float grainSize;
                                  uniform float time;
                                  
                                  float rand(vec2 co){
                                      return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
                                  }
                                  
                                  float noise2f( in vec2 p ){
                                      vec2 ip = vec2(floor(p));
                                      vec2 u = fract(p);
                                      u = u*u*(3.0-2.0*u);
                                      
                                      float res = mix(mix(rand(ip),
                                                          rand(ip+vec2(1.0,0.0)),u.x),
                                                      mix(rand(ip+vec2(0.0,1.0)),
                                                          rand(ip+vec2(1.0,1.0)),u.x),
                                                      u.y);
                                      return res*res;
                                  }
                                  
                                  void main(){
                                      vec2 st = gl_TexCoord[0].st;
                                      vec4 color = texture2DRect(prevTexture, st);
                                      
                                      if ( abs(st.x - offset) < dispersion ){
                                          vec2 srcSt = vec2(mouse.x,st.y);
                                          float noise = noise2f( vec2(st.x*time*0.3,st.y) * grainSize );
                                          srcSt.x += dispersion * noise*2.0;
                                          vec4 srcColor = texture2DRect(srcTexture, srcSt);
                                          color = mix(color,srcColor, noise );
                                      }
                                      
                                      gl_FragColor = color;
                                  });
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.linkProgram();
    
    for (int i = 0; i < 2; i++){
        fbo[i].allocate(ofGetScreenWidth(), img.height);
        fbo[i].begin();
        ofClear(0,0);
        fbo[i].end();
    }
    
    offset = 0;
    dispersion = 3.0;
    count = 0;
    grainSize = 0.995;
}

//--------------------------------------------------------------
void testApp::update(){
    mouse.x = ofLerp(mouse.x, ofClamp(mouseX, 0, img.width), 0.1);
    mouse.y = ofLerp(mouse.y, ofClamp(mouseY, 0, ofGetHeight()), 0.01);
    dispersion = ofMap(mouse.y,0,img.height,0.0,img.width*0.25);
    
    offset += 0.5;
    while ( offset > fbo[0].getWidth() ){
        offset -= fbo[0].getWidth();
    }
    
    fbo[(count+1)%2].begin();
    shader.begin();
    
    shader.setUniform1f("offset", offset);
    shader.setUniform1f("dispersion", dispersion);
    shader.setUniform1f("grainSize", 1.0-grainSize);
    shader.setUniform1f("time", ofGetElapsedTimef());
    shader.setUniform2f("mouse", mouse.x,mouse.y);
    shader.setUniformTexture("srcTexture", img.getTextureReference(), 1);
    shader.setUniformTexture("prevTexture", fbo[count%2].getTextureReference(), 0);
    
    fbo[count%2].draw(0, 0);
    
    shader.end();
    fbo[(count+1)%2].end();
    
    count++;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofPushMatrix();
    ofTranslate(0, ofGetHeight()*0.5 - img.getHeight()*0.5 );
    ofSetColor(255);
    img.draw(0, 0);
    fbo[count%2].draw(img.width, 0);
    
    ofSetColor(255,50);
    ofRect(mouse.x-dispersion, 0, dispersion*2.0, img.height);
    ofSetColor(255, 0, 0);
    ofLine(mouse.x, 0, mouse.x, img.height);
    
    ofSetColor(255,50);
    ofRect(img.getWidth()+offset-dispersion, 0, dispersion*2.0, img.height);
    
    ofPopMatrix();
    
    ofSetColor(255);
    ofDrawBitmapString("Grain Size: " + ofToString(grainSize,3), 15,15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'f'){
        ofToggleFullscreen();
    } else if ( key == OF_KEY_RIGHT){
        grainSize += 0.01;
    } else if ( key == OF_KEY_LEFT){
        grainSize -= 0.01;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    offset += 0.5;
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
    for (int i = 0; i < 2; i++){
        fbo[i].allocate(w-img.width, img.height);
        fbo[i].begin();
        ofClear(0,0);
        fbo[i].end();
    }
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}