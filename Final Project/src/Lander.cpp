#include "Lander.h"

Lander::Lander() {
    velocity.set(0);
    acceleration.set(0);
    forces.set(gravityForce);
    rotationSpeed = 0;
    rotationAcceleration = 0;
    damping = .999;
    mass = 1;
}

void Lander::update() {
    if (hasFuel) emitter.setActive(bKeyPressed);
    else emitter.setActive(false);
    emitter.setPosition(getPosition());
    emitter.setAngularVelocity(rotationSpeed);
    emitter.update();
}

void Lander::integrate() {
    // return if framerate is too low --> app is still loading
    if (ofGetFrameRate() < 10) return;
    
    // return if the lander has landed
    if (bLanded && !bThruster) return;
    
    //cout << "calling integrate(): position = " << position << "; velocity = " << velocity << "; accel = " << acceleration << "; damping = "
    //<< damping << "; framerate = " << ofGetFrameRate() << "; forces = " << forces << endl;
    
    // interval for this step
    //
    float dt = 1.0 / ofGetFrameRate();

    // update position based on velocity
    //
    ofVec3f newPos = getPosition() + velocity * dt;
    setPosition(newPos.x, newPos.y, newPos.z);
    //position += (velocity * dt);

    // clear forces on particle (they get re-added each step)
    //
    forces.set(gravityForce);
    updateForces();
    
    // update acceleration with accumulated paritcles forces
    // remember :  (f = ma) OR (a = 1/m * f)
    //
    ofVec3f accel = acceleration;    // start with any acceleration already on the particle
    accel += (forces * (1.0 / mass));
    velocity += accel * dt;

    // add a little damping for good measure
    //
    velocity *= damping;
    
    // update rotation
    float angle = rotationSpeed * dt;
    rotation += angle;
    if (rotation > 360) rotation -= 360;
    else if (rotation < 0) rotation += 360;
    int numRotation = getNumRotations();
    
    setRotation(numRotation, angle, yAxis.x, yAxis.y, yAxis.z);
    updateRotation();
    rotationSpeed += rotationAcceleration * dt;
    rotationSpeed *= damping;
}

void Lander::updateForces() {
    ofVec3f temp;
    
    switch (zMotion) {
        case Forward:
            temp = ofVec3f(0, 0, thrusterForce).rotate(rotation, yAxis);
            break;
        case Backward:
            temp = ofVec3f(0, 0, -thrusterForce).rotate(rotation, yAxis);
            break;
        case IdleZ:
        default:
            break;
    }
    forces.set(forces + temp);
    
    switch (xMotion) {
        case Left:
            temp = ofVec3f(thrusterForce, 0, 0).rotate(rotation, yAxis);
            break;
        case Right:
            temp = ofVec3f(-thrusterForce, 0, 0).rotate(rotation, yAxis);
            break;
        case IdleX:
        default:
            break;
    }
    forces.set(forces + temp);
    
    if (bThruster) {
        forces.set(forces + ofVec3f(0, thrusterForce, 0));
        bLanded = false;
    }
    
    
    // impulse force
    if (bCollision) {
        //ofVec3f newPos = getPosition() + ofVec3f(0, .05 * velocity.length(), 0);
        ofVec3f newPos = getPosition() + ofVec3f(0, 0.1, 0);
        setPosition(newPos.x, newPos.y, newPos.z);
        
        ofVec3f impulse = modifier * (restitution + 1) * -velocity.dot(normal) * normal;
        forces.set(forces + impulse);
        float magnitude = impulse.length();
        if (magnitude < modifier / 1.5) bLanded = true;
        if (magnitude > 200) {
            if (!bCrashed) {
                ofVec3f explosionForce = magnitude * ofVec3f(ofRandom(-1, 1), 1, ofRandom(-1, 1));
                forces.set(forces + explosionForce);
                
                cout << "oops lander crashed..." << endl;
            }
            
            bCrashed = true;
        }
    }
}

void Lander::updateRotation() {
    switch (rMotion) {
        case Clockwise:
            rotationAcceleration = -50;
            break;
        case Counterclockwise:
            rotationAcceleration = 50;
            break;
        case IdleR:
        default:
            rotationAcceleration = 0;
            break;
    }
}
