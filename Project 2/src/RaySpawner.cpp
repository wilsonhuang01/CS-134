#include "ofApp.h"
#include "RaySpawner.h"

// spawn a ray particle
void RaySpawner::spawnParticle() {
    sound.play();
    glm::vec3 h = glm::rotate(glm::radians(rayRot), glm::vec3(0, 0, 1)) * glm::vec4(0, -1, 0, 1);
    Particle ray;
    ray.setImage(rayImage);
    ray.radius = rayRadius;
    ray.velocity = speed * h;
    ray.lifespan = 10;
    ray.pos = pos + 1.5 * playerRadius * h;
    ray.birthtime = ofGetElapsedTimeMillis();
    ray.rot = rayRot;
    ray.damping = .999;
    ray.type = "Ray";
    sys->add(ray);
}

void RaySpawner::moveParticle(Particle* ray) {
    // calculate rot so the ray points in the direction of travel
    glm::vec3 h = glm::normalize(ray->velocity);
    glm::vec3 o = glm::vec3(0, -1, 0);
    float angle = glm::angle(h, o) * 180 / 3.14159;
    float dot = glm::dot(h, o);
    if (dot < 1) {
        glm::vec3 cross = glm::cross(h, o);
        if (cross.z > 0) angle = -angle;
    }
    
    // update ray particle
    ray->rot = angle;
    ray->radius = rayRadius;
    ray->showImage = showImage;
    ray->showOutline = showOutline;
    ray->showHeading = showHeading;
    ray->integrate();
}

void RaySpawner::update() {
    if (!started) return;
    
    float time = ofGetElapsedTimeMillis();
    if ((time - lastSpawned) > (1000.0 / rate)) {
        spawnParticle();
        lastSpawned = time;
    }
    
    if (sys->particles.size() == 0) return;
    vector<Particle>::iterator s = sys->particles.begin();
    vector<Particle>::iterator tmp;
    
    while (s != sys->particles.end()) {
        if (s->lifespan != -1 && s->age() > s->lifespan) {
            tmp = sys->particles.erase(s);
            s = tmp;
        } else {
            s++;
        }
    }
    
    for (int i = 0; i < sys->particles.size(); i++) {
        moveParticle(&sys->particles[i]);
    }
}
