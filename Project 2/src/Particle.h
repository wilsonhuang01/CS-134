#pragma once

#include "ofMain.h"
#include "Shape.h"

class Particle : public Shape {
public:
    Particle();
    void setImage(ofImage img) {
        image = img;
        showImage = true;
        width = img.getWidth();
        height = img.getHeight();
    }
    void setShowOutline(bool b) { showOutline = b; }
    void setShowHeading(bool b) { showHeading = b; }

    //glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 forces;
    float    damping;
    float   mass;
    float   lifespan;
    float   radius;
    float   birthtime;
    void    integrate();
    void    draw();
    float   age();        // sec
    ofColor color;
    ofImage image;
    bool showImage;
    bool showOutline;
    bool showHeading;
    float width;
    float height;
    string type;
    int size;
    bool forceApplied = false;
};
