#pragma once

#include "ofMain.h"
#include "ofxCcv.h"
#include "ofxTSNE.h"
#include "ofxGui.h"
#include "json.hpp"
#include "ofxThreadedVideo.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    ofxCcv ccv;
    ofxTSNE tsne;
        
    std::size_t currentImageIndex = 0;
    
    vector<ofImage> images;
    vector<std::string> filenames;
    vector<vector<double> > imagePoints;
    vector<vector<float> > encodings;
    vector<vector<double> > timeStamps;
    
    ofxPanel gui;
    ofParameter<float> scale;
    ofParameter<float> imageSize;
    
    int t;
    
    ofDirectory files;
    
    ofxThreadedVideo video1;
    ofxThreadedVideo video2;
    
    void threadedVideoEvent(ofxThreadedVideoEvent & event);
};
