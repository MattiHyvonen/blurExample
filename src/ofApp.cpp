#include "ofApp.h"
#ifndef TWO_PI
    #define TWO_PI 6.283185307
#endif


ofColor randomColor() {
    return ofColor(ofRandom(80, 256), ofRandom(80, 256), ofRandom(80, 256));
}


float radToDeg(float rads) {
    return 360.0f * rads / TWO_PI;
}


void lineSegment::set(ofVec2f P0, ofVec2f P1, float w, float blurAmount, ofColor c) {
    
    //vector d from start to end point:
    ofVec2f d = (P1-P0);
    
    position = P0;
    length = d.length();
    direction = atan2(d.y, d.x);
    
    width = w;
    if(width < 10) width = 10;
    this->blurAmount = blurAmount;
    color = c;
}


void lineSegment::draw(float x, float y) {
    //draw segment in the middle of a 100x100 area:
    ofVec2f middle(50, 50);
    
    //P: upper left corner of line segment rectangle
    ofVec2f P( (100 - width)/2, (100 - width)/2 );
    
    //corner roundness
    float corner_r = width / 4;

    ofPushMatrix();
    ofPushStyle(); //set drawing style temporarily

        ofSetColor(color);

        //rotate segment around its middle point:
        ofTranslate(middle.x, middle.y, 0);
        ofRotateZ(radToDeg(direction));
        ofTranslate(-middle.x, -middle.y, 0);
        
        ofDrawRectRounded(P.x + x, P.y + y, 0, width, width, corner_r, corner_r, corner_r, corner_r);
    
    ofPopStyle(); //return the previous style
    ofPopMatrix();
}


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(230,230,220);
    
    //ofEnableAlphaBlending(); //do we need this?
    
    //load blur shaders
    blur_x_Shader.load("shaders/blur_x.vertexShader", "shaders/blur_x.fragmentShader");
    blur_y_Shader.load("shaders/blur_y.vertexShader", "shaders/blur_y.fragmentShader");

    //allocate 100x100 area for drawing & blurring the line segment
    segFbo.allocate(100,100, GL_RGBA);
    blurFbo1.allocate(100,100, GL_RGBA);
    blurFbo2.allocate(100,100, GL_RGBA);
}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofVec2f P(mouseX, mouseY);
    ofVec2f img_middle(50, 50); //center of the line segment texture
    
    for(unsigned int i=0; i<line.size(); i++) {
        
        //clearing color shows a little when blurring. Use the segment's color for clearing:
        ofColor clearColor(line[i].color);
        clearColor.a = 0;
        
        //draw the segment to its own frame buffer object.
        //Segment is rotated to correct orientation, drawn in the middle of fbo
        segFbo.begin();
            ofClear(clearColor);
            line[i].draw(0,0);
        segFbo.end();
        
        
        //draw the segment fbo to another fbo using the blur x shader:
        blurFbo1.begin();
            ofClear(clearColor);
            blur_x_Shader.begin();
                blur_x_Shader.setUniform1f("blurAmount", line[i].blurAmount);
                segFbo.draw(0,0);
            blur_x_Shader.end();
        blurFbo1.end();
        
        
        //draw the 1st pass fbo to the 2nd pass fbo using the blur y shader:
        blurFbo2.begin();
            ofClear(clearColor);
            blur_y_Shader.begin();
                blur_y_Shader.setUniform1f("blurAmount", line[i].blurAmount);
                blurFbo1.draw(0,0);
            blur_y_Shader.end();        
        blurFbo2.end();
        
        
        //add cute color modulation
        ofSetColor(ofColor::lightPink);
        
        //draw the blurred 100x100 fbo on screen in the correct position:
        blurFbo2.draw(line[i].position - img_middle);        
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    //remember the previous mouse position:
    static ofVec2f prevPoint;
    
    //current mouse position:
    ofVec2f newPoint(x,y);
    
    //vector from previous to current: velocity
    ofVec2f d = newPoint - prevPoint;
    float v = d.length();

    //if not too close, add line segment:
    if(v > 5) {
        lineSegment new_l;
        
        //velocity affects line width and blur:
        float blurAmount = ofMap(v, 6, 30, 0, 5);
        float lineWidth = ofMap(v, 6, 45, 20, 75, true);
        ofColor c = randomColor();
        
        //set the line segment properties and add it to the line:
        new_l.set(prevPoint, newPoint, lineWidth, blurAmount, c);        
        line.push_back(new_l);

        //if too many segments, remove from the tail, which is the beginning of vector
        if(line.size() > max_points) {
            int remove_n = line.size() - max_points;
            line.erase(line.begin(), line.begin() + remove_n);
        }
        
        prevPoint = newPoint;
    }
}



//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){}
//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}
