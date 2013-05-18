#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    ofSetWindowShape(640, 480);
    millisBetweenFileCheck = 2 * 1000;
    
    oscReceiver.setup(PORT);
    
    sServer.setName(ofGetTimestampString());
    pingpong.allocate(ofGetScreenWidth(), ofGetScreenHeight());
    video = NULL;
}

void jumpNextWord(int &_index, vector<string> &_words){
    _index++;
    
    while( ((_words[ _index].size() <= 1) || (_words[ _index] == "\t")) && ( _index < _words.size())){
        _index++;
    }
}

void eraseSemiColon(string &_word){
    int end = _word.find(';') ;
    for(int i = _word.size()-1; i >= end ; i--){
        _word.erase(_word.begin()+i);
    }
}

bool testApp::reloadShader(string _filePath){
    shader.unload();
	
	// hackety hack, clear errors or shader will fail to compile
	GLuint err = glGetError();
	
    if (_filePath != "none"){
        shaderFilename = _filePath;
	}
    
	shaderFile.clear();
	shaderFile = ofFile( ofToDataPath( shaderFilename ) );
	shaderChangedTimes = getLastModified( shaderFile );
    lastTimeCheckMillis = ofGetElapsedTimeMillis();
    
	ofBuffer shaderBuffer = ofBufferFromFile( ofToDataPath( shaderFilename ) );
	
	if( shaderBuffer.size() > 0 ){
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderBuffer.getText());
	}
	
    if (shader.linkProgram()){
        
        //  Prepear the uniform to be checked
        //
        for (int i = 0; i < uniforms.size(); i++) {
            uniforms[i]->bUpdated = false;
        }
        
        //  Read the shder and check what needs to be added;
        //
        string fragmentShaderText = shaderBuffer.getText();
        vector<string> lines = ofSplitString(shaderBuffer.getText(), "\n");

        for(int i = 0; i<lines.size(); i++){
            vector<string> words = ofSplitString( lines[i], " ");
            
            for(int j = 0; j < words.size(); j++){
                if (words[j] == "uniform"){
                    jumpNextWord(j,words);
                    
                    if (words[j] == "sampler2DRect"){
                        jumpNextWord(j,words);
                        eraseSemiColon(words[j]);
                        addUniform(UNIFORM_SAMPLE2DRECT,words[j]);
                    } else if (words[j] == "vec2"){
                        jumpNextWord(j,words);
                        eraseSemiColon(words[j]);
                        addUniform(UNIFORM_VEC2,words[j]);
                    } else if (words[j] == "float"){
                        jumpNextWord(j,words);
                        eraseSemiColon(words[j]);
                        addUniform(UNIFORM_FLOAT,words[j]);
                    } 
                }
                else {
                    break;
                }
            }
        }
        
        //  Get rid of extra uniforms
        //
        for (int i = uniforms.size()-1; i >= 0; i--) {
            if (!uniforms[i]->bUpdated){
                ofRemoveListener(uniforms[i]->textChanged, this, &testApp::reloadUniforms);
                delete uniforms[i];
                uniforms.erase(uniforms.begin()+i);
            }
        }
        
        //  How many SyphonClients we need?
        //
        int needSyphonClients = 0;
        for(int i = 0; i < uniforms.size(); i++){
            if (uniforms[i]->type == UNIFORM_SAMPLE2DRECT) {
                needSyphonClients++;
            }
            uniforms[i]->y = 5+(BOX_HEIGHT+5)*i;
        }
        
        //  Add SyphonClients
        //
        while ( sTextures.size() < needSyphonClients) {
            SyphonTexture *newSyphonTexture = new SyphonTexture();
            newSyphonTexture->setup();
            sTextures.push_back(newSyphonTexture);
        }
    
        string nothing = "nothing";
        reloadUniforms(nothing);
        
        return true;
    } else {
        return false;
    }
}

void testApp::addUniform(UniformType _type, string _name){
    
    //  Check for an uniform with the same name in order not to lose the syphone address
    //
    bool bExist = false;
    for(int i = 0; i < uniforms.size(); i++){
        if ( uniforms[i]->name == _name ){
            uniforms[i]->bUpdated = true;
            bExist = true;
            break;
        }
    }
    
    if (!bExist){
        TextBox *newTextBox = new TextBox(_type, _name);
        newTextBox->x = -3;
        newTextBox->height = BOX_HEIGHT;
        newTextBox->bUpdated = true;
        
        ofAddListener(newTextBox->textChanged, this, &testApp::reloadUniforms);
        uniforms.push_back(newTextBox);
    }
}

void testApp::reloadUniforms(string &_text){
    bool bVideo = false;
    for(int i = 0; i < uniforms.size(); i++){
        
        //  TEXTURES
        //
        if (uniforms[i]->type == UNIFORM_SAMPLE2DRECT) {
            if (uniforms[i]->text == "video"){
                bVideo = true;
            } else {
                if (uniforms[i]->text.size() > 0){
                    sTextures[i]->setServer( uniforms[i]->text );
                } else {
                    sTextures[i]->sServer = uniforms[i]->text;
                }
            }
        }
    }
    
    //  Turn video on/off
    //
    if (bVideo && video == NULL){
        video = new ofVideoGrabber();
        video->initGrabber(640, 480);
    } else if (!bVideo && video != NULL){
        video->close();
        delete video;
        video = NULL;
    }
}

void testApp::checkShaderFile(){
	int currTime = ofGetElapsedTimeMillis();
	if( ((currTime - lastTimeCheckMillis) > millisBetweenFileCheck) ){
        if( shaderFile.exists() ){
            std::time_t fragmentShaderFileLastChangeTime = getLastModified( shaderFile );
            if( fragmentShaderFileLastChangeTime != shaderChangedTimes ){
                shaderChangedTimes = fragmentShaderFileLastChangeTime;
                reloadShader();
            }
        }
    
		lastTimeCheckMillis = currTime;
	}
}

std::time_t testApp::getLastModified( ofFile& _file ){
	if( _file.exists() ){
		Poco::File& pocoFile		= _file.getPocoFile();
		Poco::Timestamp timestamp	= pocoFile.getLastModified();
		std::time_t fileChangedTime = timestamp.epochTime();
		
		return fileChangedTime;
	} else {
		return 0;
	}
}

//--------------------------------------------------------------
void testApp::update(){
    checkShaderFile();
    
    //  Video 
    //
    if (video != NULL){
        video->update();
    }
    
    // OSC
    //
	while(oscReceiver.hasWaitingMessages()){
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
        for(int i = 0; i < uniforms.size(); i++){
            if(uniforms[i]->type == UNIFORM_FLOAT){
                if (uniforms[i]->text.find('/') == 0){
                    
                    if (uniforms[i]->text == m.getAddress() ){
                        uniforms[i]->fValue = m.getArgAsFloat(0);
                    }
                }
            } else if(uniforms[i]->type == UNIFORM_VEC2){
                if (uniforms[i]->text.find('/') == 0){
                    
//                    cout << uniforms[i]->text << "---" << m.getAddress() << "---" << endl;
                    if (uniforms[i]->text == m.getAddress() ){
                        uniforms[i]->vec2Value.set(m.getArgAsFloat(0), m.getArgAsFloat(1));
                    }
                }
            }
        }
        
	}
    
    ofSetWindowTitle(sServer.getName() + " ( " +ofToString(ofGetFrameRate())+ " fps)");
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);

    pingpong.swap();
    pingpong.dst->begin();
    shader.begin();
    int texLoc = 0;
    for (int i = 0; i < uniforms.size(); i++){
        if (uniforms[i]->type == UNIFORM_SAMPLE2DRECT) {
            
            if ( uniforms[i]->text == "video" && video != NULL){
                shader.setUniformTexture(uniforms[i]->name.c_str(),
                                         video->getTextureReference(),
                                         texLoc + 1);
            } else if ( uniforms[i]->text == "backbuffer" ){
                shader.setUniformTexture(uniforms[i]->name.c_str(),
                                         pingpong.src->getTextureReference(),
                                         texLoc + 1);
            } else if ( sTextures[i]->sServer != "" ){
                shader.setUniformTexture(uniforms[i]->name.c_str(),
                                         sTextures[i]->getTextureReference(),
                                         texLoc + 1);
            }
            
            texLoc++;
        } else if (uniforms[i]->type == UNIFORM_FLOAT){
            
            if (uniforms[i]->text.find('/') == 0){
                shader.setUniform1f(uniforms[i]->name.c_str(), uniforms[i]->fValue);
            } else if (uniforms[i]->text == "time"){
                shader.setUniform1f(uniforms[i]->name.c_str(), ofGetElapsedTimef());
            } else {
                shader.setUniform1f(uniforms[i]->name.c_str(), ofToFloat(uniforms[i]->text) );
            }
        } else if (uniforms[i]->type == UNIFORM_VEC2){
            
            if (uniforms[i]->text.find('/') == 0){
                shader.setUniform2f(uniforms[i]->name.c_str(), uniforms[i]->vec2Value.x, uniforms[i]->vec2Value.y);
            } else if (uniforms[i]->text == "mouse"){
                shader.setUniform2f(uniforms[i]->name.c_str(), mouseX, mouseY );
            } else if (uniforms[i]->text == "screen"){
                shader.setUniform2f(uniforms[i]->name.c_str(), ofGetScreenWidth(), ofGetScreenHeight() );
            } else if (uniforms[i]->text == "windows"){
                shader.setUniform2f(uniforms[i]->name.c_str(), ofGetWindowWidth(), ofGetWindowHeight() );
            }else {
                
                //  If names a texture on a vec2 pass the resolution of it
                //
                for (int j = 0; j < uniforms.size(); j++) {
                    if (uniforms[j]->type == UNIFORM_SAMPLE2DRECT){
                        if (uniforms[i]->text == uniforms[j]->name){
                            
                            if ( uniforms[j]->text == "video" && video != NULL){
                                shader.setUniform2f(uniforms[i]->name.c_str(), video->getWidth(), video->getHeight() );
                            } else if ( sTextures[j]->sServer != "" ){
                                shader.setUniform2f(uniforms[i]->name.c_str(), sTextures[j]->getWidth(), sTextures[j]->getHeight() );
                            }
                        }
                    }
                }
            }
        }
    }
    
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(width, 0); glVertex3f(width, 0, 0);
    glTexCoord2f(width, height); glVertex3f(width, height, 0);
    glTexCoord2f(0,height);  glVertex3f(0,height, 0);
    glEnd();
    shader.end();
    pingpong.dst->end();

    sServer.publishTexture( &(pingpong.dst->getTextureReference()) );
    pingpong.dst->draw(0, 0);
    
    for (int i = 0; i < uniforms.size(); i++){
        uniforms[i]->draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    bool bEdit = false;
    for(int i = 0; i < uniforms.size(); i++){
        if (uniforms[i]->isBeingEdited()){
            bEdit = true;
            break;
        }
    }
    
    if (!bEdit){
        if (key == 'f'){
            ofToggleFullscreen();
        } else if (key == 'o'){
            string comand = "open " + shaderFilename ;
            system(comand.c_str());
        } else if (key == 'x'){
            string comand = "xterm -e emacs " + shaderFilename + " &";
            system(comand.c_str());
        } else if (key == 'x'){
            string comand = "xterm -e emacs " + shaderFilename + " &";
            system(comand.c_str());
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

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    width = w;
    height = h;
    pingpong.allocate(w, h);
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    vector<string> file = ofSplitString(dragInfo.files[0], ".");
    
    if ( file[1] == "fs" || file[1] == "frg" || file[1] == "frag" ) {
        
        cout << dragInfo.files[0] << " loaded"<< endl;
        reloadShader(dragInfo.files[0]);
//        shader.load( "", dragInfo.files[0], "" );
    }
}