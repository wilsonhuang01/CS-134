#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    // load images
    if (!backgroundImage.load("images/background.png")) {
        cout << "Can't open background image file" << endl;
        ofExit();
    }
    if (!playerImage.load("images/earth.png")) {
        cout << "Can't open player image file" << endl;
        ofExit();
    }
    if (agentImage1.load("images/asteroid.png")) {
        agentImages.push_back(agentImage1);
    } else {
        cout << "Can't open agent image file 1" << endl;
        ofExit();
    }
    if (agentImage2.load("images/asteroid-big.png")) {
        agentImages.push_back(agentImage2);
    } else {
        cout << "Can't open agent image file 2" << endl;
        ofExit();
    }
    if (!rayImage.load("images/ray.png")) {
        cout << "Can't open ray image" << endl;
        ofExit();
    }
    if (!satelliteImage.load("images/satellite.png")) {
        cout << "Can't open satellite image" << endl;
        ofExit();
    }
    
    // load fonts
    if (!verdana12.load("fonts/verdana.ttf", 12, false)) {
        cout << "Can't open font file" << endl;
        ofExit();
    }
    if (!verdana36.load("fonts/verdana.ttf", 36, false)) {
        cout << "Can't open font file" << endl;
        ofExit();
    }
    
    // load sounds
    if (!backgroundSound.load("sounds/space.wav")) {
        cout << "Can't open space sound file" << endl;
        ofExit();
    }
    if (!thrusterSound.load("sounds/thruster.aiff")) {
        cout << "Can't open thruster sound file" << endl;
        ofExit();
    }
    if (!raySound.load("sounds/ray.wav")) {
        cout << "Can't open ray sound file" << endl;
        ofExit();
    }
    if (!collisionSound.load("sounds/collision.wav")) {
        cout << "Can't open collision sound file" << endl;
        ofExit();
    }
    backgroundSound.setLoop(true);
    thrusterSound.setVolume(.6);
    raySound.setMultiPlay(true);
    raySound.setVolume(.1);
    collisionSound.setMultiPlay(true);
    collisionSound.setVolume(.6);
    
    // setup GUI
    gui.setup();
    gui.add(level.setup("level", 1, 0, 3));
    
    gui.add(rate.setup("rate", 1, .5, 2));
    gui.add(life.setup("life", 10, 5, 20));
    gui.add(nAgents.setup("nAgents", 1, 1, 3));
    gui.add(agentSpeed.setup("agent speed", 40, 20, 100));
    gui.add(agentScale.setup("agent scale", 1, .5, 2));
    
    gui.add(playerSpeed.setup("player speed", 10, 5, 20));
    gui.add(playerScale.setup("player scale", 1, .5, 2));
    gui.add(rotationAcceleration.setup("rotational acceleration", 70, 50, 150));
    
    gui.add(rayRate.setup("ray rate", 1, .5, 2));
    gui.add(raySpeed.setup("ray speed", 50, 20, 150));
    gui.add(rayScale.setup("ray scale", 1, .5, 2));
    
    gui.add(playerDrawnInImage.setup("show player in image", false));
    gui.add(agentDrawnInImage.setup("show agents in image", false));
    gui.add(showOutline.setup("show outline", false));
    gui.add(showHeading.setup("show heading", false));

    bHide = false;
    
    // setup game
    ofSetFrameRate(60);
    applyLevelPreset(level);
    newGame();
}

//--------------------------------------------------------------
void ofApp::update() {
    windowWidth = ofGetWindowWidth();
    
    if (!spawner->started && energy != 0) {
        applyLevelPreset(level);
    }
    
    float playerRad = playerRadius * playerScale;
    float rayRad = rayRadius * rayScale;
    float agentRad = agentRadius * agentScale;
    player->setRadius(playerRad);
    player->setRayRadius(rayRad);
    player->setShowImage(playerDrawnInImage);
    player->setShowOutline(showOutline);
    player->setShowHeading(showHeading);
    player->setPlayerSpeed(playerSpeed);
    player->setSpeed(raySpeed);
    player->setRate(rayRate);
    if (!spawner->started) player->setPosition(glm::vec3(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0));
    
    minX = player->radius;
    maxX = ofGetWindowWidth() - player->radius;
    minY = player->radius;
    maxY = ofGetWindowHeight() - player->radius;
    player->setBoundary(minX, maxX, minY, maxY);
    if (spawner->started) player->update();
    
    // update spawner
    spawner->setRate(rate);
    spawner->setLifespan(life);
    spawner->setSpeed(agentSpeed);
    spawner->setNAgents(nAgents);
    spawner->setAgentRadius(agentRad);
    spawner->setPlayerRadius(playerRad);
    spawner->setRayRadius(rayRad);
    spawner->setAgentDrawnInImage(agentDrawnInImage);
    spawner->setShowOutline(showOutline);
    spawner->setShowHeading(showHeading);
    spawner->update();
    
    // update explosion effect emitter
    explosionEffect->update();
    
    // update parameters while the game is running
    if (spawner->started) {
        energy = spawner->nEnergy;
        if (energy <= 0) endGame();
        nEnergy = to_string(energy) + " / " + to_string(energyInit);
        lastTime = ofGetElapsedTimeMillis() - spawner->timeStarted;
        elapsedTime = lastTime;
        framerate = (int) ofGetFrameRate();
    }
    
    // play sounds
    if (!backgroundSound.isPlaying()) {
        backgroundSound.play();
    }
    if (bThrust && !thrusterSound.isPlaying()) {
        thrusterSound.play();
    } else if (!bThrust && thrusterSound.isPlaying()) {
        thrusterSound.stop();
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    // draw background image
    if (!spawner->started) {
        ofSetColor(ofColor::lightGray);
    } else {
        ofSetColor(ofColor::white);
    }
    backgroundImage.draw(0, 0, windowWidth, ofGetWindowHeight());

    spawner->draw();
    player->draw();
    explosionEffect->draw();
    
    if (!bHide) {
        gui.draw();
    }
    
    // display text
    if (!spawner->started) {
        ofSetColor(ofColor::lightGray);
        tmpRect = verdana36.getStringBoundingBox(gameTitle, 0, 0);
        verdana36.drawString(gameTitle, (ofGetWindowWidth() - tmpRect.width) / 2, ofGetWindowHeight() / 2 - 150);
    }
    
    if (helpMenu) {
        ofSetColor(ofColor::white);
        verdana12.drawString("Key", windowWidth - 320, 40);
        verdana12.drawString("Action", windowWidth - 170, 40);
        verdana12.drawString("-------------------------------------------", windowWidth - 320, 60);
        
        verdana12.drawString("W", windowWidth - 320, 80);
        verdana12.drawString("Move up", windowWidth - 170, 80);
        verdana12.drawString("S", windowWidth - 320, 100);
        verdana12.drawString("Move down", windowWidth - 170, 100);
        verdana12.drawString("A", windowWidth - 320, 120);
        verdana12.drawString("Move left", windowWidth - 170, 120);
        verdana12.drawString("D", windowWidth - 320, 140);
        verdana12.drawString("Move right", windowWidth - 170, 140);

        verdana12.drawString("Left arrow", windowWidth - 320, 180);
        verdana12.drawString("Rotate left", windowWidth - 170, 180);
        verdana12.drawString("Right arrow", windowWidth - 320, 200);
        verdana12.drawString("Rotate right", windowWidth - 170, 200);
        
        verdana12.drawString("Spacebar", windowWidth - 320, 240);
        verdana12.drawString("Start game", windowWidth - 170, 240);
        verdana12.drawString("R", windowWidth - 320, 260);
        verdana12.drawString("Restart game", windowWidth - 170, 260);
        
        verdana12.drawString("F", windowWidth - 320, 300);
        verdana12.drawString("Toggle fullscreen", windowWidth - 170, 300);
        verdana12.drawString("H", windowWidth - 320, 320);
        verdana12.drawString("Toggle GUI", windowWidth - 170, 320);
        verdana12.drawString("?", windowWidth - 320, 340);
        verdana12.drawString("Toggle Help menu", windowWidth - 170, 340);
    }
    
    ofSetColor(ofColor::white);
    verdana12.drawString("Press '?' to show Help menu", ofGetWindowWidth() - 250, ofGetWindowHeight() - 20);
    verdana12.drawString("Energy: " + to_string(energy) + "/" + to_string(energyInit), 20, ofGetWindowHeight() - 60);
    verdana12.drawString("Elapsed time: " + timeToString(elapsedTime), 20, ofGetWindowHeight() - 40);
    verdana12.drawString("Framerate: " + to_string(framerate), 20, ofGetWindowHeight() - 20);
    if (!spawner->started && energy == 0) {
        ofSetColor(ofColor::darkorange);
        tmpRect = verdana36.getStringBoundingBox(gameOverText, 0, 0);
        verdana36.drawString(gameOverText, (ofGetWindowWidth() - tmpRect.width) / 2, ofGetWindowHeight() / 2 + 60);
        tmpStr = "Game time: " + timeToString(lastTime);
        tmpRect = verdana36.getStringBoundingBox(tmpStr, 0, 0);
        verdana36.drawString(tmpStr, (ofGetWindowWidth() - tmpRect.width) / 2, ofGetWindowHeight() / 2 + 120);
        tmpRect = verdana12.getStringBoundingBox(restartText, 0, 0);
        verdana12.drawString(restartText, (ofGetWindowWidth() - tmpRect.width) / 2, ofGetWindowHeight() / 2 + 160);
    }
}


//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
}

void ofApp::keyPressed(int key) {
    switch (key) {
        case 'w':
            if (spawner->started) player->setVerticalMode(Player::Up);
            break;
        case 's':
            if (spawner->started) player->setVerticalMode(Player::Down);
            break;
        case 'a':
            if (spawner->started) player->setHorizontalMode(Player::Left);
            break;
        case 'd':
            if (spawner->started) player->setHorizontalMode(Player::Right);
            break;
        case OF_KEY_LEFT:
            if (spawner->started) {
                player->satellite->setRotationAcceleration(-rotationAcceleration);
                bThrust = true;
            }
            break;
        case OF_KEY_RIGHT:
            if (spawner->started) {
                player->satellite->setRotationAcceleration(rotationAcceleration);
                bThrust = true;
            }
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'h':
            bHide = !bHide;
            break;
        case '?':
            helpMenu = !helpMenu;
            break;
        case ' ': //spacebar
            if (!spawner->started && energy != 0) {
                startGame();
            }
            break;
        case 'r':
            endGame();
            applyLevelPreset(level);
            newGame();
            break;
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    switch (key) {
        case 'w':
            player->setVerticalMode(Player::IdleV);
            break;
        case 's':
            player->setVerticalMode(Player::IdleV);
            break;
        case 'a':
            player->setHorizontalMode(Player::IdleH);
            break;
        case 'd':
            player->setHorizontalMode(Player::IdleH);
            break;
        case OF_KEY_LEFT:
            player->satellite->setRotationAcceleration(0);
            bThrust = false;
            break;
        case OF_KEY_RIGHT:
            player->satellite->setRotationAcceleration(0);
            bThrust = false;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

string ofApp::timeToString(int millisec) {
    int min = millisec / 60000;
    millisec = millisec % 60000;
    string time = "";
    if (min > 0) {
        if (min < 10) time += "0";
        time += to_string(min);
    } else {
        time += "00";
    }
    time += ":";
    int sec = millisec / 1000;
    millisec = millisec % 1000;
    if (sec < 10) time += "0";
    time += to_string(sec);
    int mil = millisec / 100;
    time += ".";
    time += to_string(mil);
    return time;
}

void ofApp::startGame() {
    newGame();
    spawner->start();
    player->start();
}

void ofApp::endGame() {
    spawner->stop();
    player->stop();
}

void ofApp::newGame() {
    agentRadius = 15;
    playerRadius = 25;
    rayRadius = 8;
    
    player = new Player();
    player->setPosition(glm::vec3(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0));
    player->setParticleVelocity(glm::vec3(0, -1, 1));
    player->setPlayerImage(playerImage);
    player->setRayImage(rayImage);
    player->setSound(raySound);
    player->satellite->setImage(satelliteImage);

    explosionEffect = new ExplosionEffectEmitter();
    radialForce = new ImpulseRadialForce(explosionMagnitude);
    explosionEffect->sys->addForce(radialForce);
    explosionEffect->setEmitterType(RadialEmitter);
    explosionEffect->setSound(collisionSound);
    explosionEffect->setParticleRadius(explosionParticleRadius);
    explosionEffect->setLifespan(explosionParticleLifespan);
    explosionEffect->setCount(explosionParticleCount);
    
    spawner = new AgentSpawner();
    spawner->visible = false;
    spawner->setChildImages(agentImages);
    spawner->setExplosionEffectEmitterRef(explosionEffect);
    spawner->setPlayerRef(player);
    spawner->setNEnergy(energyInit);
    spawner->setEnergyMax(energyInit);
    spawner->setLevel(level);
    
    elapsedTime = 0;
}

void ofApp::applyLevelPreset(int level) {
    if (level == 1) {
        energyInit = 5;
        rate = 1;
        life = 10;
        nAgents = 1;
        agentSpeed = 40;
        agentScale = 1;
        playerSpeed = 10;
        playerScale = 1;
        rotationAcceleration = 70;
        rayRate = 1;
        raySpeed = 50;
        rayScale = 1;
    } else if (level == 2) {
        energyInit = 4;
        rate = 1.5;
        life = 13;
        nAgents = 1;
        agentSpeed = 50;
        agentScale = 1.2;
        playerSpeed = 12;
        playerScale = 1.1;
        rotationAcceleration = 100;
        rayRate = 1.1;
        raySpeed = 70;
        rayScale = 1.2;
    } else if (level == 3) {
        energyInit = 4;
        rate = 1.2;
        life = 15;
        nAgents = 2;
        agentSpeed = 50;
        agentScale = 1.2;
        playerSpeed = 14;
        playerScale = 1.3;
        rotationAcceleration = 130;
        rayRate = 1.3;
        raySpeed = 100;
        rayScale = 1.3;
    } else {
        return;
    }
    energy = energyInit;
}
