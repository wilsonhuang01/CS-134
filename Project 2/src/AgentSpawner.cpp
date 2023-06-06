#include "ofApp.h"
#include "AgentSpawner.h"

// used for spawning Asteroids
void AgentSpawner::spawnParticle(int size) {
    // generate random pos
    int x = ofRandom(windowWidth);
    int y = ofRandom(windowHeight);
    glm::vec3 spritePos = glm::vec3(x, y, 1);
    
    // generate random velocity
    glm::vec3 v = speed * glm::normalize(glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), 1));
    
    // initialize Asteroid
    Particle agent;
    if (haveChildImages) agent.setImage(childImages[size - 1]);
    agent.radius = sqrt(size) * agentRadius;
    agent.velocity = v;
    agent.mass = size;
    agent.lifespan = lifespan;
    agent.pos = spritePos;
    agent.birthtime = ofGetElapsedTimeMillis();
    agent.type = "Agent";
    agent.size = size;
    sys->add(agent);
}

// used for spawning Asteroids when a bigger Asteroid is hit
void AgentSpawner::spawnParticles(glm::vec3 pos, glm::vec3 velocity) {
    // initialize the first Asteroid
    glm::vec3 v1;
    v1.x = -cos(10) * velocity.x + sin(10) * velocity.y;
    v1.y = -sin(10) * velocity.x - cos(10) * velocity.y;
    Particle agent1;
    if (haveChildImages) agent1.setImage(childImages[0]);
    agent1.radius = agentRadius;
    agent1.velocity = v1;
    agent1.mass = 1;
    agent1.lifespan = lifespan;
    agent1.pos = pos;
    agent1.birthtime = ofGetElapsedTimeMillis();
    agent1.type = "Agent";
    agent1.size = 1;
    sys->add(agent1);
    
    // initialize the second Asteroid
    glm::vec3 v2;
    v2.x = -cos(-10) * velocity.x + sin(-10) * velocity.y;
    v2.y = -sin(-10) * velocity.x - cos(-10) * velocity.y;
    Particle agent2;
    if (haveChildImages) agent2.setImage(childImages[0]);
    agent2.radius = agentRadius;
    agent2.velocity = v2;
    agent2.mass = 1;
    agent2.lifespan = lifespan;
    agent2.pos = pos;
    agent2.birthtime = ofGetElapsedTimeMillis();
    agent2.type = "Agent";
    agent2.size = 1;
    sys->add(agent2);
}

void AgentSpawner::moveParticle(Particle* agent) {
    // calculate rot so the Asteroid points in the direction of travel
    glm::vec3 h = glm::normalize(agent->velocity);
    glm::vec3 o = glm::vec3(0, -1, 0);
    float angle = glm::angle(h, o) * 180 / 3.14159;
    float dot = glm::dot(h, o);
    if (dot < 1) {
        glm::vec3 cross = glm::cross(h, o);
        if (cross.z > 0) angle = -angle;
    }
    
    // update Asteroid
    agent->radius = sqrt(agent->size) * agentRadius;
    agent->rot = angle;
    agent->showImage = agentDrawnInImage;
    agent->showOutline = showOutline;
    agent->showHeading = showHeading;
    agent->forces = speed * glm::normalize(player->pos - agent->pos);
    agent->integrate();
}

void AgentSpawner::update() {
    if (!started) return;
    
    float time = ofGetElapsedTimeMillis();
    if ((time - lastSpawned) > (1000.0 / rate)) {
        // has a chance to spawn a bigger Asteroid when level = 2 or 3
        if (level == 0 || level == 1) {
            for (int i = 0; i < nAgents; i++) {
                spawnParticle(1);
            }
        } else if (level == 2 || level == 3) {
            for (int i = 0; i < nAgents; i++) {
                spawnParticle((int) ofRandom(1.0, 3.0));
            }
        }
        lastSpawned = time;
    }
    
    if (sys->particles.size() == 0) return;
    vector<Particle>::iterator agent = sys->particles.begin();
    vector<Particle>::iterator tempAgent;
    
    while (agent != sys->particles.end()) {
        bool remove = false;
        float distPlayer = glm::sqrt(glm::pow(player->pos.x - agent->pos.x, 2) + glm::pow(player->pos.y - agent->pos.y, 2));
        if (agent->lifespan != -1 && agent->age() > agent->lifespan) {
            // exceed lifespan
            //cout << "agent exceed lifespan" << endl;
            tempAgent = sys->particles.erase(agent);
            agent = tempAgent;
        } else if (agent->size == 1 ? distPlayer < contactDistPlayer1 : distPlayer < contactDistPlayer2) {
            // agent collide with player
            explosionEffectEmitter->generateEffect(agent->pos);
            if (nEnergy > 0) {
                nEnergy = max(0, nEnergy - agent->size);
            }
            //cout << "agent collide with player at " << agent->pos << endl;
            tempAgent = sys->particles.erase(agent);
            agent = tempAgent;
        } else {
            agent++;
        }
    }
    
    // remove agent if collided with a ray particle
    vector<glm::vec3> positions;
    vector<glm::vec3> velocities;
    if (player->sys->particles.size() != 0) {
        agent = sys->particles.begin();
        while (agent != sys->particles.end()) {
            vector<Particle>::iterator ray = player->sys->particles.begin();
            vector<Particle>::iterator tempRay;
            bool collided = false;
            
            while (ray != player->sys->particles.end()) {
                float distRay = glm::sqrt(glm::pow(agent->pos.x - ray->pos.x, 2) + glm::pow(agent->pos.y - ray->pos.y, 2));
                if (agent->size == 1 ? distRay < contactDistRay1 : distRay < contactDistRay2) {
                    //cout << "within contact distance, dist = " << dist << ", contactDistance = " << contactDistanceRay << endl;
                    collided = true;
                    explosionEffectEmitter->generateEffect(agent->pos);
                    if (nEnergy < energyMax) {
                        nEnergy++;
                    }
                    tempRay = player->sys->particles.erase(ray);
                    ray = tempRay;
                } else {
                    ray++;
                }
            }
            
            if (collided) {
                //cout << "agent collide with ray at " << agent->pos << endl;
                if (agent->size == 2) {
                    positions.push_back(agent->pos);
                    velocities.push_back(agent->velocity);
                }
                tempAgent = sys->particles.erase(agent);
                agent = tempAgent;
            } else {
                agent++;
            }
        }
    }
    
    for (int i = 0; i < positions.size(); i++) {
        spawnParticles(positions[i], velocities[i]);
    }
    
    for (int i = 0; i < sys->particles.size(); i++) {
        moveParticle(&sys->particles[i]);
    }
}

