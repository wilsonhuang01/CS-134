#pragma once
//  Kevin M. Smith - CS 134 SJSU

#include "ofMain.h"
#include "Particle.h"


//  Pure Virtual Function Class - must be subclassed to create new forces.
//
class ParticleForce {
public:
    virtual void updateForce(Particle *) = 0;
};

class ParticleSystem {
public:
    void add(const Particle &);
    void addForce(ParticleForce *);
    void remove(int);
    void update();
    int removeNear(const glm::vec3 &point, float dist);
    void draw();
    vector<Particle> particles;
    vector<ParticleForce *> forces;
};



// Some convenient built-in forces
//
class GravityForce: public ParticleForce {
    glm::vec3 gravity;
public:
    GravityForce(const glm::vec3 &gravity);
    void set(const glm::vec3 &gravity);
    void updateForce(Particle *);
};

class TurbulenceForce : public ParticleForce {
    glm::vec3 tmin, tmax;
public:
    TurbulenceForce(const glm::vec3 &min, const glm::vec3 &max);
    void set(const glm::vec3 &min, const glm::vec3 &max);
    void updateForce(Particle *);
};

class NoiseForce : public ParticleForce {
    glm::vec3 tmin, tmax;
public:
    NoiseForce(const glm::vec3 &min, const glm::vec3 &max);
    void set(const glm::vec3 &min, const glm::vec3 &max);
    void updateForce(Particle *);
};

class ImpulseRadialForce : public ParticleForce {
    float magnitude;
public:
    ImpulseRadialForce(float m);
    void updateForce(Particle *);
    void updateMagnitude(float m) { magnitude = m; }
};
