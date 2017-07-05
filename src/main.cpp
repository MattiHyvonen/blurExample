#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2); //we define the OpenGL version we want to use
    settings.width = 960;
    settings.height = 768;

    ofCreateWindow(settings);
    ofRunApp( new ofApp());

}
