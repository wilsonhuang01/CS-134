#include "ofApp.h"
#include "AgentSpawner.h"

void AgentSpawner::spawnSprite() {
    // generate random pos
    int x = ofRandom(windowWidth);
    int y = ofRandom(windowHeight);
    glm::vec3 spritePos = glm::vec3(x, y, 1);
    
    // generate random rot
    glm::vec3 v = speed * glm::normalize(player->pos - spritePos);
    float angle = ofRandom(360);

    Sprite agent;
    if (haveChildImages) agent.setImage(childImages[ofRandom(childImages.size())]);
    agent.radius = spriteRadius;
    agent.velocity = v;
    agent.lifespan = lifespan;
    agent.pos = spritePos;
    agent.birthtime = ofGetElapsedTimeMillis();
    agent.rot = angle;
    agent.showImage = agentDrawnInImage;
    sys->add(agent);
}

void AgentSpawner::moveSprite(Sprite* agent) {
    // calculate rot so the agent points in the direction of the player
    glm::vec3 v = speed * glm::normalize(player->pos - agent->pos);
    glm::vec3 v0 = glm::vec3(0, -1, 1);
    float dot = glm::dot(v, v0);
    float a = glm::sqrt(glm::pow(v.x, 2) + glm::pow(v.y, 2));
    float b = glm::sqrt(glm::pow(v0.x, 2) + glm::pow(v0.y, 2));
    float angle = glm::acos(dot / (a * b)) * 180 / PI;
    if (v.x < 0) {
        angle = 360 - angle;
    }
    agent->radius = spriteRadius;
    agent->velocity = v;
    agent->speed = speed;
    agent->rot = angle;
    agent->pos += agent->velocity / ofGetFrameRate();
    agent->showImage = agentDrawnInImage;
    agent->showOutline = showOutline;
    agent->showHeading = showHeading;
}

void AgentSpawner::update() {
    if (!started) return;

    float time = ofGetElapsedTimeMillis();
    if ((time - lastSpawned) > (1000.0 / rate)) {
        // call virtual to spawn a new sprite
        //
        for (int i = 0; i < nAgents; i++) {
            spawnSprite();
        }
        lastSpawned = time;
    }

    // update sprite list
    //
    if (sys->sprites.size() == 0) return;
    vector<Sprite>::iterator s = sys->sprites.begin();
    vector<Sprite>::iterator tmp;

    // check which sprites have exceed their lifespan and delete
    // from list.  When deleting multiple objects from a vector while
    // traversing at the same time, use an iterator.
    //
    while (s != sys->sprites.end()) {
        if (s->lifespan != -1 && s->age() > s->lifespan) {
            tmp = sys->sprites.erase(s);
            s = tmp;
        } else if (glm::sqrt(glm::pow(player->pos.x - s->pos.x, 2) + glm::pow(player->pos.y - s->pos.y, 2)) < contactDistance) {
            tmp = sys->sprites.erase(s);
            s = tmp;
            if (nEnergy > 0) {
                nEnergy--;
            }
        } else {
            s++;
        }
    }

    for (int i = 0; i < sys->sprites.size(); i++) {
        moveSprite(&sys->sprites[i]);
    }
}
