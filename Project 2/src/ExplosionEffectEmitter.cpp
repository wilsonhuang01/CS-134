#include "ExplosionEffectEmitter.h"


void ExplosionEffectEmitter::spawnParticle() {
    Particle particle;
    ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), 0);
    particle.pos = pos;
    particle.lifespan = lifespan;
    particle.birthtime = ofGetElapsedTimeMillis();
    particle.radius = particleRadius * ofRandom(.5, 2);
    particle.type = "Explosion";
    sys->add(particle);
}

void ExplosionEffectEmitter::update() {
    sys->update();
}

// generate explosion visual effects at pos
void ExplosionEffectEmitter::generateEffect(glm::vec3 pos) {
    setPos(pos);
    
    for (int i = 0; i < count; i++)
        spawnParticle();
    
    sound.play();
}
