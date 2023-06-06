#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Player.h"
#include "Shape.h"
#include "AgentSpawner.h"
#include "ExplosionEffectEmitter.h"


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
    void startGame();
    void endGame();
    void newGame();
    void applyLevelPreset(int l);
    string timeToString(int sec);

    AgentSpawner  *spawner = NULL;
    Player *player = NULL;
    ExplosionEffectEmitter *explosionEffect = NULL;
    ImpulseRadialForce *radialForce;

    ofImage backgroundImage;
    ofImage playerImage;
    ofImage agentImage1;
    ofImage agentImage2;
    ofImage rayImage;
    ofImage satelliteImage;
    vector<ofImage> agentImages;
    
    float minX;
    float maxX;
    float minY;
    float maxY;

    // Some basic UI
    //
    bool bHide;
    ofxIntSlider level;
    ofxFloatSlider rate;
    ofxFloatSlider life;
    ofxFloatSlider agentSpeed;
    ofxIntSlider nAgents;
    ofxFloatSlider playerSpeed;
    ofxFloatSlider rotationAcceleration;
    float playerRadius;
    ofxFloatSlider playerScale;
    ofxFloatSlider agentScale;
    ofxFloatSlider rayRate;
    ofxFloatSlider raySpeed;
    ofxFloatSlider rayScale;
    ofxToggle playerDrawnInImage;
    ofxToggle agentDrawnInImage;
    ofxToggle showOutline;
    ofxToggle showHeading;
    int energyInit = 10;
    int energy;
    int explosionMagnitude = 2500;
    float explosionParticleRadius = 3;
    float explosionParticleLifespan = 1;
    int explosionParticleCount = 20;
    ofxLabel nEnergy;
    ofxLabel framerate1;
    int lastTime;
    int elapsedTime;
    int framerate;
    ofxLabel elapsedTime1;
    ofxLabel screenSize;
    float agentRadius;
    float rayRadius;
    ofxPanel gui;
    
    float windowWidth;
    ofRectangle tmpRect;
    string tmpStr;
    string gameTitle = "Earth Defender";
    string gameOverText = "Game Over";
    string restartText = "Press 'R' to restart";
    bool helpMenu;
    
    ofTrueTypeFont verdana12;
    ofTrueTypeFont verdana36;
    
    ofSoundPlayer backgroundSound;
    ofSoundPlayer thrusterSound;
    ofSoundPlayer raySound;
    ofSoundPlayer collisionSound;
    bool bThrust;
};
