#pragma once
#include "RaySpawner.h"
#include "Shape.h"

class Satellite : public Shape {
public:
    Satellite() {};
    void draw();
    void setImage(ofImage img) {
        image = img;
        width = img.getWidth();
        height = img.getHeight();
    }
    void setShowImage(bool b) { showImage = b; }
    void setRadius(float r) { radius = r; }
    void setPos(glm::vec3 p) { pos = p; }
    void setPlayerRadius(float r) { playerRadius = r; }
    void setRotationAcceleration(float accel) { rotationAcceleration = accel; }
    void integrate();
    
    ofImage image;
    bool showImage;
    float width;
    float height;
    float radius;
    float playerRadius;
    float s;
    
    // rotation physics
    float rotationSpeed;
    float rotationAcceleration;
    float damping = .99;
};


// the Emitter emits sprites towards this object
class Player : public RaySpawner {
public:
    typedef enum {IdleV, Up, Down} VerticalMode;
    typedef enum {IdleH, Left, Right} HorizontalMode;
    
    Player() {};
    void draw();
    bool inside(glm::vec3);
    bool insideTriangle(glm::vec3);
    void update();
    
    void setPosition(glm::vec3 p) {
        pos = p;
        satellite->setPos(p);
    }
    void setVerticalMode(VerticalMode m) { vMode = m; }
    void setHorizontalMode(HorizontalMode m) { hMode = m; }
    void setHeading(glm::vec3 h) { heading = h; }
    void setPlayerSpeed(float sp) { speed = sp * speedMult; }
    void setRadius(float rad) {
        radius = rad;
        satellite->setPlayerRadius(rad);
        satellite->setRadius(rad / 2);
        setPlayerRadius(rad);
    }
    void setShowImage(bool b) {
        showImage = b;
        RaySpawner::setShowImage(b);
        satellite->setShowImage(b);
    }
    void setShowOutline(bool b) {
        showOutline = b;
        RaySpawner::setShowOutline(b);
    }
    void setShowHeading(bool b) {
        showHeading = b;
        RaySpawner::setShowHeading(b);
    }
    void setPlayerImage(ofImage img) {
        playerImage = img;
        showImage = true;
        width = img.getWidth();
        height = img.getHeight();
    }
    void setBoundary(int minX, int maxX, int minY, int maxY) {
        this->minX = minX;
        this->maxX = maxX;
        this->minY = minY;
        this->maxY = maxY;
    }
    void integrate();
    
    glm::mat4 getMatrix() {
        glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(pos));
        glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(rot), glm::vec3(0, 0, 1));
        glm::mat4 S = glm::scale(glm::mat4(1.0), scale);      // added this scale if you want to change size of object
        return T*R*S;
    }
    
    Satellite *satellite = new Satellite();
    glm::vec3 heading = glm::vec3(0, -1, 0);
    glm::vec3 up = glm::vec3(0, -1, 0);
    glm::vec3 down = glm::vec3(0, 1, 0);
    glm::vec3 left = glm::vec3(-1, 0, 0);
    glm::vec3 right = glm::vec3(1, 0, 0);
    int minX;
    int maxX;
    int minY;
    int maxY;
    VerticalMode vMode;
    HorizontalMode hMode;
    float speed;
    float speedMult = 5;
    float width = 40;
    float height = 40;
    float radius = 20;
    bool showImage;
    bool showOutline;
    bool showHeading;
    ofImage playerImage;
    
    // motion physics
    glm::vec3 acceleration = glm::vec3(0);
    glm::vec3 velocity = glm::vec3(0);
    glm::vec3 force = glm::vec3(0);
    float mass = 1.0;
    float damping = 0.99;
};

