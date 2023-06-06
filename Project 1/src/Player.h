#pragma once


// the Emitter emits sprites towards this object
class Player : public Shape {
public:
    
    void draw();
    bool inside(glm::vec3);
    bool insideTriangle(glm::vec3);
    void movePlayer();
    
    void setHeading(glm::vec3 h) {
        heading = h;
    }
    
    void setSpeed(float sp) {
        speed = sp * speedMult;
    }
    
    void setMovementMode(int m) {
        mode = m;
    }
    
    void setRadius(float rad) {
        radius = rad;
    }
    
    void setDrawnInImage(bool b) {
        showImage = b;
    }
    
    void setShowOutline(bool b) {
        showOutline = b;
    }
    
    void setShowHeading(bool b) {
        showHeading = b;
    }
    
    void setImage(ofImage img) {
        image = img;
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
    
    void setRotationSpeed(int sp) {
        rotationSpeed = sp;
    }
    
    glm::vec3 heading = glm::vec3(0, -1, 0);
    int minX;
    int maxX;
    int minY;
    int maxY;
    int mode;
    float speed;
    float speedMult = 5;
    float width = 40;
    float height = 40;
    float radius = 20;
    int rotationSpeed;
    bool showImage;
    bool showOutline;
    bool showHeading;
    ofImage image;
};
