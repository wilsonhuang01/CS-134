#pragma once
#include "ParticleEmitter.h"

//  "Custom" Particle Emitter
//  Emits Particle sprites which will have our own custom behavior
//
class RaySpawner : public ParticleEmitter {
public:
    void spawnParticle();
    void moveParticle(Particle* ray);
    void update();
    
    void setPos(glm::vec3 p) { pos = p; }
    void setSpeed(float sp) { speed = sp; }
    void setNEnergy(int n) { nEnergy = n; }
    void setContactDistance(float dist) { contactDistance = dist; }
    void setRayRadius(float rad) { rayRadius = rad; }
    void setPlayerRadius(float r) { playerRadius = r; }
    void setShowImage(bool b) { showImage = b; }
    void setShowOutline(bool b) { showOutline = b;}
    void setShowHeading(bool b) { showHeading = b; }
    void setRayImage(ofImage img) { rayImage = img; }
    void setRayRot(float r) { rayRot = r; }
    void setSound(ofSoundPlayer s) { sound = s; }
    
    ofImage rayImage;
    float speed;
    int nEnergy;
    float contactDistance;
    float rayRadius;
    float rayRot;
    float playerRadius;
    int windowWidth = ofGetWindowWidth();
    int windowHeight = ofGetWindowHeight();
    bool showImage;
    bool showOutline;
    bool showHeading;
    
    ofSoundPlayer sound;
};


