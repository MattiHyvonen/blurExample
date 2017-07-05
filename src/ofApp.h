#pragma once

#include "ofMain.h"
#include <vector>


class lineSegment{
public:
    ofVec2f position; //position of the starting point of the line segment
    float direction;
    float length;
    float width; //line width
    
    float blurAmount;
    ofColor color;
    
    void set(ofVec2f P0, ofVec2f P1, float w = 3, float blurAmount = 0.1, ofColor c = ofColor::blue); //set line geometry & properties, using starting point P0, end point P1
    
    void draw(float x=0, float y=0); //draw line segment at given position, leave some empty space around
};


class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
                
                ofShader blur_x_Shader;
                ofShader blur_y_Shader;
                
                std::vector<lineSegment> line; //list of line segments
                unsigned int max_points = 300; //maximum number of segments in line
                
                ofFbo segFbo; //frame buffer object for rendering line segments
                ofFbo blurFbo1; //frame buffer object for blur 1st pass (x direction)
                ofFbo blurFbo2; //frame buffer object for blur 2nd pass (y))

};
