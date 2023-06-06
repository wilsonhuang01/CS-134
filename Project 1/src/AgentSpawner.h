#pragma once


//  "Custom" Particle Emitter
//  Emits Particle sprites which will have our own custom behavior
//
class AgentSpawner : public Emitter {
public:

    void spawnSprite();
    void moveSprite(Sprite* agent);
    void update();
    
    void setSpeed(float sp) {
        speed = sp;
    }
    
    void setNAgents(int n) {
        nAgents = n;
    }
    
    void setNEnergy(int n) {
        nEnergy = n;
    }
    
    void setContactDistance(float dist) {
        contactDistance = dist;
    }
    
    void setSpriteRadius(float rad) {
        spriteRadius = rad;
    }
    
    void setAgentDrawnInImage(bool b) {
        agentDrawnInImage = b;
    }
    
    void setShowOutline(bool b) {
        showOutline = b;
    }
    
    void setShowHeading(bool b) {
        showHeading = b;
    }
    
    void setPlayerRef(Player *p) {
        player = p;
    }
    
    Player *player;
    float speed;
    int nAgents;
    int nEnergy;
    float contactDistance;
    float spriteRadius;
    int windowWidth = ofGetWindowWidth();
    int windowHeight = ofGetWindowHeight();
    bool agentDrawnInImage;
    bool showOutline;
    bool showHeading;
};

