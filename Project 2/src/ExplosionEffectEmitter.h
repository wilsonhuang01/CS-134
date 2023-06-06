#pragma once
#include "ParticleEmitter.h"

class ExplosionEffectEmitter : public ParticleEmitter {
public:
    ExplosionEffectEmitter() {};
    void spawnParticle();
    void update();
    void generateEffect(glm::vec3 pos);
    void setPos(glm::vec3 p) { pos = p; }
    void setCount(int c) { count = c; }
    void setSound(ofSoundPlayer s) { sound = s; }
    
    int count;
    
    ofSoundPlayer sound;
};
