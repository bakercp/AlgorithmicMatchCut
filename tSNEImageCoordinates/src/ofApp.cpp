#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    // uncomment to see the command queue logging
    video1.setVerbose(true);
    
    // load all the mp4s
    files.allowExt("mp4");
    files.listDir("/Users/nicholasdavis/Documents/SAMPLE_MEDIA/scaledMp4s"); // put a video path here with several video files in a folder --

    video1.loadMovie(files.getPath(ofRandom(files.numFiles())));
    video1.play();
    
    ofBackground(0, 0, 0);
    
    ofAddListener(video1.threadedVideoEvent, this, &ofApp::threadedVideoEvent);
    
    // load all the images
    ofLog() << "Gathering images...";
    ofDirectory dir;
    int nFiles = dir.listDir(ofToDataPath("imageSequences/"));
    if(nFiles) {
        for(int i=0; i<dir.size(); i++) {
            if (i % 20 == 0)    ofLog() << " - loading image "<<i<<" / "<<dir.size();
            string filePath = dir.getPath(i);
            ofImage im;
            im.setUseTexture(false);
            im.load(filePath);
            images.push_back(im);
            //images.push_back(ofImage()); // having allocation issues with 32-bit
            images.back().load(filePath);
            filenames.push_back(dir.getFile(i).getFileName());
        }
    }
    
    for (int i = 0; i < images.size(); i++){
        images[i].setUseTexture(true);
    }
    
    

    
/* ----------------------- Not important for debugging
    // setup ofxCcv
    ccv.setup("image-net-2012.sqlite3");
    
    // encode all of the images with ofxCcv
    ofLog() << "Encoding images...";
    for (int i=0; i<images.size(); i++) {
        if (i % 20 == 0)    ofLog() << " - encoding image "<<i<<" / "<<images.size();
        vector<float> encoding = ccv.encode(images[i], ccv.numLayers()-6); //adjust numLayers
        encodings.push_back(encoding);
    }
    
    // run t-SNE and load image points to imagePoints
    ofLog() << "Run t-SNE on images";
    imagePoints = tsne.run(encodings, 2, 25, 0.1, true);


    // fill JSON with imagePoints coordinates of each image
    ofJson output;
    for (int i=0; i<filenames.size(); i++) {
        
        auto tokens = ofSplitString(ofFilePath::getBaseName(filenames[i]), "-");
        
        output[tokens[0]].push_back({
            { "time", ofToDouble(tokens[1]) * 10 },
            { "x", imagePoints[i][0] },
            { "y", imagePoints[i][1] }
        }
        );
    }
 
    ofFile file("tSNEcoords.json", ofFile::WriteOnly);
    //    file << output;
    //    cout << output.dump(4) << endl;
-------------------------- Not important for debugging */
    
    
    ofFile inputFile("tSNEcoords04_43perplex.json", ofFile::ReadOnly);
    ofJson inputJson = ofJson::parse(ofToString(inputFile.readToBuffer()));
    for (ofJson::iterator it = inputJson.begin(); it != inputJson.end(); ++it) {
        cout << inputJson. << endl;
        
        for (int i = 0; i < inputJson[it.key()].size(); i++) {
            std::vector<double> v;
            imagePoints.push_back(v);
            //cout << i << endl;
            
            for (int j = 0; j < inputJson[it.key()][i].size(); j++) {
                //timeStamps[i].push_back(inputJson[it.key()][i]["time"]);
                imagePoints[i].push_back(inputJson[it.key()][i]["x"]);
                imagePoints[i].push_back(inputJson[it.key()][i]["y"]);
                
                //cout << j << endl;
                //cout << inputJson[it.key()][i]["x"] << endl; //successfully returns "x" coordinate
            }
        } // bakercp :
    } // something about this whole parse iterator/loop is not working. it should be iterating through every set of image sequences (there are 43), then looping through each image in each sequence, then loading the x,y coordinates for each into `vector<vector<double> > imagePoints`, but for some reason it is only loading coordinates into the first three sets of image sequences ("bd1" "bd2" "car" because alphabetical)
    
    
    // make the images the same size
    for (int i=0; i<images.size(); i++) {
        images[i].resize(100 * images[i].getWidth() / images[i].getHeight(), 100);
    }
    
    // setup gui
    gui.setup();
    gui.add(scale.set("scale", 1.0, 0.0, 10.0));
    gui.add(imageSize.set("imageSize", 1.0, 0.0, 2.0));
}

//--------------------------------------------------------------
void ofApp::update(){
    
    currentImageIndex = (currentImageIndex + 1) % images.size();
    
    //video1.update();
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    ofSetColor(255);
    ofPushMatrix();
//    ofTranslate(-ofGetMouseX() * (scale - 0.5), -ofGetMouseY() * (scale - 0.5));
    for (int i=0; i<imagePoints.size(); i++) {
        
        images[i].setUseTexture(true);
        if (imagePoints[i].size() >= 2) {
            float x = ofMap(imagePoints[i][0], 0, 1, 0, scale * ofGetWidth());
            float y = ofMap(imagePoints[i][1], 0, 1, 0, scale * ofGetHeight());
            images[i].draw(x, y, imageSize * images[i].getWidth(), imageSize * images[i].getHeight());
            ofDrawBitmapString(filenames[i], x + 14, y + 28);
        }
    }
    ofPopMatrix();
    
    
    video1.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    ostringstream os;
    os << "FPS: " << ofGetFrameRate() << endl;
    os << "use keys '<' '>' and  ' ' to load movies, 'p' to toggle pause," << endl;
    os << "L/R arrow keys to jump frames and U/D arrow keys to change speed" << endl;
    os << video1.getCurrentFrame() << "/" << video1.getTotalNumFrames() << " " << video1.getSpeed() << endl;
    os << video2.getCurrentFrame() << "/" << video2.getTotalNumFrames() << " " << video2.getSpeed() << endl;
    ofDrawBitmapString(os.str(), 20, ofGetHeight() - 100);
    
    
    
    
    /* // bakercp draw yellow frames on images in order of file name at ofSetFrameRate($)
    ofPushMatrix();
    ofTranslate(-ofGetMouseX() * (scale - 0.5), -ofGetMouseY() * (scale - 0.5));
    int i = currentImageIndex;
    float x = ofMap(imagePoints[i][0], 0, 1, 0, scale * ofGetWidth());
    float y = ofMap(imagePoints[i][1], 0, 1, 0, scale * ofGetHeight());
    images[i].draw(x, y, imageSize * images[i].getWidth(), imageSize * images[i].getHeight());
    
    ofDrawBitmapString(filenames[i], x + 14, y + 28);
    
    ofNoFill();
    ofSetColor(ofColor::yellow);
    ofDrawRectangle(x, y,imageSize * images[i].getWidth(), imageSize * images[i].getHeight());
    
    ofPopMatrix();
    */
    

    gui.draw();
}

//--------------------------------------------------------------
void ofApp::threadedVideoEvent(ofxThreadedVideoEvent & event){
    ofLogVerbose() << "VideoEvent: " << event.eventTypeAsString << " for " << event.path;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case '<':
        case ',':
            video1.loadMovie(files.getPath(ofRandom(files.numFiles())));
            cout << video1.getMovieName() << endl;
            video1.play();
            break;
        case '>':
        case '.':
            //            video2.loadMovie(files.getPath(ofRandom(files.numFiles())));
            //            video2.play();
            //            break;
        case ' ':
            video1.loadMovie(files.getPath(ofRandom(files.numFiles())));
            video1.play();
            //            video2.loadMovie(files.getPath(ofRandom(files.numFiles())));
            //            video2.play();
            break;
        case 'p':
            video1.setPaused(!video1.isPaused());
            //            video2.setPaused(!video2.isPaused());
            break;
        case OF_KEY_LEFT:
            video1.firstFrame();
            //            video2.firstFrame();
            break;
        case OF_KEY_RIGHT:
            video1.setFrame(ofRandom(video1.getTotalNumFrames()));
            //            video2.setFrame(ofRandom(video2.getTotalNumFrames()));
            break;
        case OF_KEY_UP:
            video1.setSpeed(video1.getSpeed() + 0.2);
            //            video2.setSpeed(video2.getSpeed() + 0.2);
            break;
        case OF_KEY_DOWN:
            video1.setSpeed(video1.getSpeed() - 0.2);
            //            video2.setSpeed(video2.getSpeed() - 0.2);
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
