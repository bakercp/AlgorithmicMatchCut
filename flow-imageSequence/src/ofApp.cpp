#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    ofBackground(0);
    
    ofSetFrameRate(10);
    
    std::string path = "ztc.mp4";
    
    ofFile file(path);
    
    baseName = file.getBaseName();
    
    ofDirectory dir(baseName);
    dir.create();
    
    player.load(file.getAbsolutePath());
    player.play();
    player.setLoopState(OF_LOOP_NONE);
    player.setVolume(0);
    
    cout << file.getExtension();
    
    gui.setup();
    
	gui.add(fbPyrScale.set("fbPyrScale", .5, 0, .99));
	gui.add(fbLevels.set("fbLevels", 4, 1, 8));
    gui.add(fbIterations.set("fbIterations", 2, 1, 8));
    gui.add(fbPolyN.set("fbPolyN", 7, 5, 10));
    gui.add(fbPolySigma.set("fbPolySigma", 1.5, 1.1, 2));
    gui.add(fbUseGaussian.set("fbUseGaussian", false));
    gui.add(fbWinSize.set("winSize", 32, 4, 64));

}

void ofApp::update(){
	player.update();
	
    if (player.getIsMovieDone() == true) {
        std::exit(1);
    }
    
	if(player.isFrameNew()) {
        fb.setPyramidScale(fbPyrScale);
        fb.setNumLevels(fbLevels);
        fb.setWindowSize(fbWinSize);
        fb.setNumIterations(fbIterations);
        fb.setPolyN(fbPolyN);
        fb.setPolySigma(fbPolySigma);
        fb.setUseGaussian(fbUseGaussian);
        
        fb.calcOpticalFlow(player.getPixels());
        
        ofVec2f v = fb.getAverageFlow();
        
    
//        cout << v.length() << endl;
    
        if (v.length() > 4)
        {
            float positionInSeconds = (player.getPosition() * player.getDuration()) * 0.01;
            
            ofSaveImage(player.getPixels(), baseName + "/" + baseName + "-" + ofToString(positionInSeconds, 5) + ".jpg", OF_IMAGE_QUALITY_LOW);
            
        }
        
	}
}

void ofApp::draw(){
    ofPushMatrix();
    ofTranslate(250, 100);
	player.draw(0,0,640,480);
    fb.draw(0,0,640,480);
    ofDrawBitmapStringHighlight(ofToString((int) ofGetFrameRate()) + "fps", 10, 20);
    ofPopMatrix();
    gui.draw();
}