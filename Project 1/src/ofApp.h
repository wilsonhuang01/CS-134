#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include "Player.h"
#include "Shape.h"
#include "Sprite.h"
#include "AgentSpawner.h"


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
    void endGame();
    void newGame();
    void applyLevelPreset(int l);
    string timeToString(int sec);

    AgentSpawner  *spawner = NULL;
    Player *player = NULL;

    ofImage backgroundImage;
    ofImage playerImage;
    ofImage agentImage1;
    ofImage agentImage2;
    ofImage agentImage3;
    vector<ofImage> agentImages;
    
    float minX;
    float maxX;
    float minY;
    float maxY;
    int level = 1;

    // Some basic UI
    //
    bool bHide;
    ofxFloatSlider rate;
    ofxFloatSlider life;
    ofxFloatSlider agentSpeed;
    ofxIntSlider nAgents;
    ofxFloatSlider playerSpeed;
    float playerRadius;
    ofxFloatSlider playerScale;
    ofxFloatSlider agentScale;
    ofxToggle playerDrawnInImage;
    ofxToggle agentDrawnInImage;
    ofxToggle showOutline;
    ofxToggle showHeading;
    int energyInit = 10;
    int energy;
    ofxLabel nEnergy;
    ofxLabel framerate1;
    int lastTime;
    int elapsedTime;
    int framerate;
    ofxLabel elapsedTime1;
    ofxLabel screenSize;
    float spriteRadius;
    ofxPanel gui;
    
    ofTrueTypeFont verdana12;
    ofTrueTypeFont verdana36;
};
