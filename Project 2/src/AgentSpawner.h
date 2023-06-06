#pragma once
#include "ParticleEmitter.h"
#include "ExplosionEffectEmitter.h"
#include "Player.h"

//  "Custom" Particle Emitter
//  Emits Particle sprites which will have our own custom behavior
//
class AgentSpawner : public ParticleEmitter {
public:

    void spawnParticle(int size);
    void spawnParticles(glm::vec3 pos, glm::vec3 velocity);
    void moveParticle(Particle* agent);
    void update();
    
    void setSpeed(float sp) { speed = sp; }
    void setNAgents(int n) { nAgents = n; }
    void setNEnergy(int n) { nEnergy = n; }
    void setEnergyMax(int n) { energyMax = n; }
    void setLevel(int l) { level = l; }
    void setPlayerRadius(float r) {
        playerRadius = r;
        contactDistPlayer1 = playerRadius + agentRadius;
        contactDistPlayer2 = playerRadius + sqrt(2) * agentRadius;
    }
    void setRayRadius(float r) {
        rayRadius = r;
        contactDistRay1 = rayRadius + agentRadius;
        contactDistRay2 = rayRadius + sqrt(2) * agentRadius;
    }
    void setAgentRadius(float rad) { agentRadius = rad; }
    void setAgentDrawnInImage(bool b) { agentDrawnInImage = b; }
    void setShowOutline(bool b) { showOutline = b; }
    void setShowHeading(bool b) { showHeading = b; }
    void setPlayerRef(Player *p) { player = p; }
    void setExplosionEffectEmitterRef(ExplosionEffectEmitter *emitter) { explosionEffectEmitter = emitter; }
    
    ExplosionEffectEmitter *explosionEffectEmitter;
    Player *player;
    float speed;
    int nAgents;
    int nEnergy;
    int energyMax;
    int level;
    float playerRadius;
    float rayRadius;
    float contactDistPlayer1;
    float contactDistPlayer2;
    float contactDistRay1;
    float contactDistRay2;
    float agentRadius;
    int windowWidth = ofGetWindowWidth();
    int windowHeight = ofGetWindowHeight();
    bool agentDrawnInImage;
    bool showOutline;
    bool showHeading;
};
