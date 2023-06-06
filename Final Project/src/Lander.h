#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ParticleEmitter.h"

class Lander : public ofxAssimpModelLoader{
public:
    typedef enum {IdleZ, Forward, Backward} ZMotion; // go forward or backward
    typedef enum {IdleX, Left, Right} XMotion; // go left or right
    typedef enum {IdleR, Clockwise, Counterclockwise} RMotion; // rotate clockwise or counterclockwise
    
    Lander();
    void update();
    void integrate();
    void updateForces();
    void updateRotation();
    
    void setHasFuel(bool b) { hasFuel = b; }
    void setZMotion(ZMotion m) { zMotion = m; }
    void setXMotion(XMotion m) { xMotion = m; }
    void setRMotion(RMotion m) { rMotion = m; }
    void setThruster(bool b) { bThruster = b; }
    void setKeyPressed(bool b) { bKeyPressed = b; }
    
    void setCollision(bool b) { bCollision = b; }
    void setNormal(ofVec3f n) { normal = n; }
    
    // motion
    ZMotion zMotion;
    XMotion xMotion;
    RMotion rMotion;
    bool bThruster;
    bool bKeyPressed;
    
    // physics
    ofVec3f velocity;
    ofVec3f acceleration;
    ofVec3f forces;
    float damping;
    float mass;
    bool hasFuel;
    
    // rotation
    ofVec3f yAxis = ofVec3f(0, 1, 0);
    float rotation; // use a separate rotation variable to keep track of rotation because ofxAssimpModelLoader::getRotationAngle is more complex
    float rotationSpeed;
    float rotationAcceleration;
    
    // forces
    ofVec3f gravityForce = ofVec3f(0, -1.62, 0);
    //ofVec3f gravityForce = ofVec3f(0, 0, 0); // for testing
    float thrusterForce = 3;
    
    // collision
    bool bCollision;
    bool bLanded;
    bool bCrashed;
    ofVec3f normal;
    float restitution = 0.01;
    int modifier = 100;
    
    // particle emitter
    ParticleEmitter emitter;
};
