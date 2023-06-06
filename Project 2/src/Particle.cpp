#include "Particle.h"


Particle::Particle() {
    // initialize particle with some reasonable values first;
    //
    velocity = glm::vec3(0, 0, 0);
    acceleration = glm::vec3(0, 0, 0);
    pos = glm::vec3(0, 0, 0);
    forces = glm::vec3(0, 0, 0);
    lifespan = 5;
    birthtime = 0;
    radius = .1;
    damping = .999;
    mass = 1;
    color = ofColor::aquamarine;
}

void Particle::draw() {
    ofPushMatrix();
    ofMultMatrix(getMatrix());
    
    if (type.compare("Explosion") == 0) {
        ofSetColor(ofMap(age(), 0, lifespan, 255, 10), 0, 0);
        ofDrawCircle(glm::vec3(0, 0, 0), radius);
    } else {
        float size;
        if (showImage) size = width > height ? width / 2 : height / 2;
        else size = radius;
        
        if (showImage) {
            float imageScale = width > height ? radius / width * 2 : radius / height * 2;
            scale = glm::vec3(imageScale, imageScale, 1);
            ofFill();
            ofSetColor(ofColor::white);
            image.draw(-width / 2, -height / 2);
        } else {
            scale = glm::vec3(1);
            ofFill();
            ofSetColor(type.compare("Agent") == 0 ? ofColor::gray : ofColor::orange);
            float x = radius * 0.8;
            float y = radius * 0.6;
            glm::vec3 p1 = glm::vec3(-x, y, 0);
            glm::vec3 p2 = glm::vec3(x, y, 0);
            glm::vec3 p3 = glm::vec3(0, -radius, 0);
            ofDrawTriangle(p1, p2, p3);
            verts.push_back(p1);
            verts.push_back(p2);
            verts.push_back(p3);
        }
        if (showOutline) {
            ofNoFill();
            ofSetColor(ofColor::red);
            ofDrawCircle(0, 0, size);
        }
        if (showHeading) {
            ofSetColor(ofColor::floralWhite);
            glm::vec3 p = glm::vec3(0, -size, 0);
            ofDrawLine(p, p + glm::vec3(0, -size, 0));
        }
    }
    
    ofPopMatrix();
}

// write your own integrator here.. (hint: it's only 3 lines of code)
//
void Particle::integrate() {

    
    // interval for this step
    //
    float dt = 1.0 / ofGetFrameRate();

    // update position based on velocity
    //
    pos += (velocity * dt);

    // update acceleration with accumulated paritcles forces
    // remember :  (f = ma) OR (a = 1/m * f)
    //
    glm::vec3 accel = acceleration;    // start with any acceleration already on the particle
    accel += (forces * (1.0 / mass));
    velocity += accel * dt;

    // add a little damping for good measure
    //
    velocity *= damping;

    // clear forces on particle (they get re-added each step)
    //
    forces = glm::vec3(0, 0, 0);
    //if (type.compare("Explosion") == 0) cout << forces << endl;
}

//  return age in seconds
//
float Particle::age() {
    return (ofGetElapsedTimeMillis() - birthtime)/1000.0;
}
