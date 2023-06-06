#pragma once

#include "Shape.h"



// Base class for a Sprite. Can be instantiated on it's own (deafult)
// or subclassed to create a custom sprite.  Includes Shape transformations.
// If the sprite has no image set on it, then a simple triangle will be drawn.
//

class Sprite : public Shape {
public:
    void draw();

    float age() {
        return (ofGetElapsedTimeMillis() - birthtime);
    }

    void setImage(ofImage img) {
        spriteImage = img;
        showImage = true;
        width = img.getWidth();
        height = img.getHeight();
    }

    virtual bool inside(const glm::vec3 p);
    virtual void update() {}

    bool insideTriangle(const glm::vec3 p);
    

    void setSelected(bool state) { bSelected = state; }
    void setHighlight(bool state) { bHighlight = state; }
    bool isSelected() { return bSelected; }
    bool isHighlight() { return bHighlight; }


    bool bHighlight = false;
    bool bSelected = false;
    bool showImage = false;
    bool showOutline;
    bool showHeading;

    glm::vec3 velocity = glm::vec3(0, 0, 0);
    float speed;
    float rotationSpeed = 0.0;
    float birthtime = 0; // elapsed time in ms
    float lifespan = -1;  //  time in ms
    string name =  "UnammedSprite";
    float width = 40;
    float height = 40;
    float radius;

    ofImage spriteImage;
};

