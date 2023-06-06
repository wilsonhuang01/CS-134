#include "ofApp.h"
#include "Player.h"

void Player::draw() {
    if (showImage) {
        ofPushMatrix();
        ofMultMatrix(getMatrix());
        
        float imageScale = 0;
        float size = width > height ? width / 2 : height / 2;
        if (width > height) {
            imageScale = radius / width * 2;
        } else {
            imageScale = radius / height * 2;
        }
        scale = glm::vec3(imageScale, imageScale, 1);
        if (showOutline) {
            ofNoFill();
            ofSetColor(ofColor::red);
            ofDrawCircle(0, 0, size);
        }
        if (showHeading) {
            ofSetColor(ofColor::lightBlue);
            glm::vec3 p = glm::vec3(0, -size, 0);
            ofDrawLine(p, p + glm::vec3(0, -speed / 3, 0));
        }
        ofFill();
        ofSetColor(ofColor::white);
        image.draw(-width / 2, -height / 2);
        ofPopMatrix();
    } else {
        ofPushMatrix();
        ofMultMatrix(getMatrix());
        scale = glm::vec3(1);
        if (showOutline) {
            ofNoFill();
            ofSetColor(ofColor::red);
            ofDrawCircle(0, 0, radius);
        }
        if (showHeading) {
            ofSetColor(ofColor::lightBlue);
            glm::vec3 p = glm::vec3(0, -radius, 0);
            ofDrawLine(p, p + glm::vec3(0, -speed / 3, 0));
        }
        ofFill();
        ofSetColor(ofColor::lightBlue);
        float x = radius * 0.8;
        float y = radius * 0.6;
        glm::vec3 p1 = glm::vec3(-x, y, 0);
        glm::vec3 p2 = glm::vec3(x, y, 0);
        glm::vec3 p3 = glm::vec3(0, -radius, 0);
        ofDrawTriangle(p1, p2, p3);
        ofPopMatrix();
    }
}

bool Player::inside(glm::vec3 p) {
    // if there is no image attached, then just use triangle case.
    //
    if (!showImage) {
        return insideTriangle (p);
    }

    // if image attached, then first check if point is inside bounds of image
    // in object space.  If point is inside bounds, then make sure the point is in
    // opaque part of image.
    //
    glm::vec3 s = glm::inverse(getMatrix()) * glm::vec4(p, 1);
    int w = image.getWidth();
    int h = image.getHeight();
    cout << "calculating inside" << endl;
    if (s.x > -w / 2 && s.x < w / 2 && s.y > -h / 2 && s.y < h / 2) {
        int x = s.x + w / 2;
        int y = s.y + h / 2;
        ofColor color = image.getColor(x, y);
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

}

void Player::movePlayer() {
    rot += rotationSpeed;
    float cos = glm::cos(ofDegToRad(rotationSpeed));
    float sin = glm::sin(ofDegToRad(rotationSpeed));
    // update heading
    heading = glm::vec3(heading.x * cos - heading.y * sin, heading.x * sin + heading.y * cos, 0);
    if (mode == 0) return; // not moving forward or backward, -> idle and no need to update pos

    glm::vec3 nextPos;
    if (mode == 1) nextPos = pos + heading * speed / ofGetFrameRate();
    else nextPos = pos - heading * speed / ofGetFrameRate();
    
    if (nextPos.x >= minX && nextPos.x <= maxX && nextPos.y >= minY && nextPos.y <= maxY) {
        pos = nextPos;
    } else {
        if (nextPos.x < minX) pos.x = minX;
        else if (nextPos.x > maxX) pos.x = maxX;
        else if (nextPos.y < minY) pos.y = minY;
        else pos.y = maxY;
    }
}

