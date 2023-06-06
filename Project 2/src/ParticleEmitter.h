#pragma once
//  Kevin M. Smith - CS 134 SJSU

#include "Shape.h"
#include "ParticleSystem.h"

typedef enum { DirectionalEmitter, RadialEmitter, SphereEmitter } EmitterType;

//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class ParticleEmitter : public Shape {
public:
    ParticleEmitter();
    ParticleEmitter(ParticleSystem *s);
    ~ParticleEmitter();
    void init();
    void draw();
    void start();
    void stop();
    void setLifespan(const float life)   { lifespan = life; }
    void setParticleVelocity(const glm::vec3 &vel) { particleVelocity = vel; }
    void setRate(const float r) { rate = r; }
    void setDamping(const float d) { damping = d; }
    void setParticleRadius(const float r) { particleRadius = r; }
    void setEmitterType(EmitterType t) { type = t; }
    void setChildImages(vector<ofImage>);
    void update();
    ParticleSystem *sys;
    float rate;         // per sec
    glm::vec3 particleVelocity;
    float lifespan;     // sec
    float damping;
    bool started;
    float lastSpawned;  // ms
    float timeStarted;
    float particleRadius;
    float radius;
    bool visible;
    bool createdSys;
    EmitterType type;
    vector<ofImage> childImages;
    bool haveChildImages;
};
