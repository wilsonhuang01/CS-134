#include "ofApp.h"
#include "Player.h"

void Satellite::draw() {
    ofPushMatrix();
    ofMultMatrix(getMatrix());
    
    if (showImage) {
        float imageScale = width > height ? radius / width * 2 : radius / height * 2;
        scale = glm::vec3(imageScale, imageScale, 1);
        ofFill();
        ofSetColor(ofColor::white);
        float x = -width / 2;
        float y = -1.7 * playerRadius / scale.y;
        image.draw(x, y);
    } else {
        scale = glm::vec3(1);
        ofFill();
        ofSetColor(ofColor::mediumSpringGreen);
        glm::vec3 p1 = glm::vec3(radius, -playerRadius, 0);
        glm::vec3 p2 = glm::vec3(-radius, -playerRadius, 0);
        glm::vec3 p3 = glm::vec3(0, -radius - playerRadius, 0);
        ofDrawTriangle(p1, p2, p3);
        verts.push_back(p1);
        verts.push_back(p2);
        verts.push_back(p3);
    }
    
    ofPopMatrix();
}

void Satellite::integrate() {
    float framerate = ofGetFrameRate();
    float dt = 1.0 / framerate;
    
    rot += rotationSpeed * dt;
    rotationSpeed += rotationAcceleration * dt;
    rotationSpeed *= damping;
}


void Player::draw() {
    ofPushMatrix();
    ofMultMatrix(getMatrix());
    
    float size;
    if (showImage) size = width > height ? width / 2 : height / 2;
    else size = radius;
    
    if (showImage) {
        float imageScale = width > height ? radius / width * 2 : radius / height * 2;
        scale = glm::vec3(imageScale, imageScale, 1);
        ofFill();
        ofSetColor(ofColor::white);
        playerImage.draw(-width / 2, -height / 2);
    } else {
        scale = glm::vec3(1);
        ofFill();
        ofSetColor(ofColor::lightBlue);
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
        ofSetColor(ofColor::lightBlue);
        glm::vec3 n = glm::normalize(velocity);
        glm::vec3 p = glm::vec3(n.x * size, n.y * size, 0);
        ofDrawLine(p, p + p);
    }
    
    ofPopMatrix();
    sys->draw();
    satellite->draw();
}

bool Player::inside(glm::vec3 p) {
    // if there is no image attached, then just use triangle case.
    //
    if (!showImage) {
        return insideTriangle(p);
    }

    // if image attached, then first check if point is inside bounds of image
    // in object space.  If point is inside bounds, then make sure the point is in
    // opaque part of image.
    //
    glm::vec3 s = glm::inverse(getMatrix()) * glm::vec4(p, 1);
    int w = playerImage.getWidth();
    int h = playerImage.getHeight();
    cout << "calculating inside" << endl;
    if (s.x > -w / 2 && s.x < w / 2 && s.y > -h / 2 && s.y < h / 2) {
        int x = s.x + w / 2;
        int y = s.y + h / 2;
        ofColor color = playerImage.getColor(x, y);
        bool r = color.a != 0;
        if (r)
            cout << "inside image, returned true" << endl;
        else
            cout << "inside image, returned false" << endl;
        return r;
    }
    cout << "outside image" << endl;
    return false;
}

// to be implemented
bool Player::insideTriangle(const glm::vec3 p) {
    // transform the screen space point p to the triangle's local
    // oordinate system  (object space);  this will take into account any
    // rotation, translation or scale on the object.
    //
    glm::vec4 p2 = glm::inverse(getMatrix()) * glm::vec4(p, 1);
    
    glm::vec3 v1 = glm::normalize(verts[0] - p2);
    glm::vec3 v2 = glm::normalize(verts[1] - p2);
    glm::vec3 v3 = glm::normalize(verts[2] - p2);
    float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
    float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
    float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
    if (a1 < 0 && a2 < 0 && a3 < 0) return true;
    else return false;
     
    return false;
}

void Player::update() {
    // update pos
    glm::vec3 f = glm::vec3(0);
    int speedMod = 250;
    if (vMode == Up) {
        f += speedMod * up * speed / ofGetFrameRate();
    }
    if (vMode == Down) {
        f += speedMod * down * speed / ofGetFrameRate();
    }
    if (hMode == Left) {
        f += speedMod * left * speed / ofGetFrameRate();
    }
    if (hMode == Right) {
        f += speedMod * right * speed / ofGetFrameRate();
    }
    
    glm::vec3 nextPos = pos + velocity / ofGetFrameRate();
    if (nextPos.x >= minX && nextPos.x <= maxX && nextPos.y >= minY && nextPos.y <= maxY) {
        force = f;
        integrate();
    } else {
        if (nextPos.x < minX) pos.x = minX;
        else if (nextPos.x > maxX) pos.x = maxX;
        else if (nextPos.y < minY) pos.y = minY;
        else pos.y = maxY;
        velocity = glm::vec3(0);
    }
    satellite->s = scale.y;
    satellite->setPos(pos);
    setRayRot(satellite->rot);
    RaySpawner::update();
}

void Player::integrate() {
    float framerate = ofGetFrameRate();
    float dt = 1.0 / framerate;
    
    pos += velocity * dt;
    glm::vec3 accel = acceleration;
    accel += force / mass;
    velocity += accel * dt;
    velocity *= damping;
    
    satellite->integrate();
}

