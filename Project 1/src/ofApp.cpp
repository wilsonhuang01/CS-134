#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    // load images
    if (!backgroundImage.load("images/background.png")) {
        cout << "Can't open background image file" << endl;
        ofExit();
    }
    if (!playerImage.load("images/mouse.png")) {
        cout << "Can't open player image file" << endl;
        ofExit();
    }
    if (agentImage1.load("images/cat1.png")) {
        agentImages.push_back(agentImage1);
    } else {
        cout << "Can't open agent image file 1" << endl;
        ofExit();
    }
    if (agentImage2.load("images/cat2.png")) {
        agentImages.push_back(agentImage2);
    } else {
        cout << "Can't open agent image file 2" << endl;
        ofExit();
    }
    if (agentImage3.load("images/cat3.png")) {
        agentImages.push_back(agentImage3);
    } else {
        cout << "Can't open agent image file 3" << endl;
        ofExit();
    }
    if (!verdana12.load("fonts/verdana.ttf", 12, false)) {
        cout << "Can't open font file" << endl;
        ofExit();
    }
    if (!verdana36.load("fonts/verdana.ttf", 36, false)) {
        cout << "Can't open font file" << endl;
        ofExit();
    }
    
    // setup GUI
    gui.setup();
    gui.add(rate.setup("rate", 1, .5, 3));
    gui.add(life.setup("life", 5, 0, 15));
    gui.add(nAgents.setup("nAgents", 1, 1, 3));
    gui.add(agentSpeed.setup("agent speed", 50, 10, 200));
    gui.add(agentScale.setup("agent scale", 1, .5, 2));
    gui.add(playerSpeed.setup("player speed", 50, 10, 200));
    gui.add(playerScale.setup("player scale", 1, .5, 2));
    gui.add(playerDrawnInImage.setup("show player in image", false));
    gui.add(agentDrawnInImage.setup("show agents in image", false));
    gui.add(showOutline.setup("show outline", false));
    gui.add(showHeading.setup("show heading", false));

    bHide = false;
    
    // setup game
    applyLevelPreset(level);
    newGame();
}

//--------------------------------------------------------------
void ofApp::update() {
    // update player
    player->setRadius(playerRadius * playerScale);
    player->setDrawnInImage(playerDrawnInImage);
    player->setShowOutline(showOutline);
    player->setShowHeading(showHeading);
    player->setSpeed(playerSpeed);
    
    minX = player->radius;
    maxX = ofGetWindowWidth() - player->radius;
    minY = player->radius;
    maxY = ofGetWindowHeight() - player->radius;
    player->setBoundary(minX, maxX, minY, maxY);
    if (spawner->started) player->movePlayer();
    
    // update spawner
    float spriteMax = spriteRadius * agentScale;
    float contactDistance = spriteMax + player->radius;
    spawner->setRate(rate);
    spawner->setLifespan(life * 1000);    // convert to milliseconds
    spawner->setSpeed(agentSpeed);
    spawner->setNAgents(nAgents);
    spawner->setSpriteRadius(spriteMax);
    spawner->setContactDistance(contactDistance);
    spawner->setAgentDrawnInImage(agentDrawnInImage);
    spawner->setShowOutline(showOutline);
    spawner->setShowHeading(showHeading);
    spawner->update();
    
    // update parameters while the game is running
    if (spawner->started) {
        energy = spawner->nEnergy;
        if (energy == 0) endGame();
        nEnergy = to_string(energy) + " / " + to_string(energyInit);
        lastTime = ofGetElapsedTimeMillis() - spawner->timeStarted;
        elapsedTime = lastTime;
        framerate = (int) ofGetFrameRate();
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    // draw background image
    if (!spawner->started && energy == 0) {
        ofSetColor(ofColor::lightGray);
    } else {
        ofSetColor(ofColor::white);
    }
    backgroundImage.draw(0, 0);

    spawner->draw();
    player->draw();
    
    if (!bHide) {
        gui.draw();
    }
    
    // display relevant text
    ofSetColor(ofColor::white);
    verdana12.drawString("Press '1', '2', or '3' to select level", ofGetWindowWidth() - 300, ofGetWindowHeight() - 60);
    verdana12.drawString("Press spacebar to start", ofGetWindowWidth() - 300, ofGetWindowHeight() - 40);
    verdana12.drawString("Press 'R' to restart", ofGetWindowWidth() - 300, ofGetWindowHeight() - 20);
    verdana12.drawString("Level: " + to_string(level), 20, ofGetWindowHeight() - 80);
    verdana12.drawString("Energy: " + to_string(energy) + "/" + to_string(energyInit), 20, ofGetWindowHeight() - 60);
    verdana12.drawString("Elapsed time: " + timeToString(elapsedTime), 20, ofGetWindowHeight() - 40);
    verdana12.drawString("Framerate: " + to_string(framerate), 20, ofGetWindowHeight() - 20);
    if (!spawner->started && energy == 0) {
        ofSetColor(ofColor::darkorange);
        verdana36.drawString("Game time: " + timeToString(lastTime), ofGetWindowWidth() / 3, ofGetWindowHeight() / 2);
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
            if (spawner->started) player->setMovementMode(1);
            break;
        case 's':
            if (spawner->started) player->setMovementMode(2);
            break;
        case 'a':
            if (spawner->started) player->setRotationSpeed(-level);
            break;
        case 'd':
            if (spawner->started) player->setRotationSpeed(level);
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'h':
            bHide = !bHide;
            break;
        case '1':
            if (!spawner->started && energy != 0) {
                level = 1;
                applyLevelPreset(level);
                newGame();
            }
            break;
        case '2':
            if (!spawner->started && energy != 0) {
                level = 2;
                applyLevelPreset(level);
                newGame();
            }
            break;
        case '3':
            if (!spawner->started && energy != 0) {
                level = 3;
                applyLevelPreset(level);
                newGame();
            }
            break;
        case ' ': //spacebar
            if (!spawner->started && energy != 0) spawner->start();
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
            player->setMovementMode(0);
            break;
        case 's':
            player->setMovementMode(0);
            break;
        case 'a':
            player->setRotationSpeed(0);
            break;
        case 'd':
            player->setRotationSpeed(0);
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

void ofApp::endGame() {
    spawner->stop();
}

void ofApp::newGame() {
    spriteRadius = 20;
    playerRadius = 20;
    
    player = new Player();
    player->pos = glm::vec3(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0);
    player->setImage(playerImage);

    spawner = new AgentSpawner();
    spawner->drawable = false;
    spawner->setChildImages(agentImages);
    spawner->setPlayerRef(player);
    spawner->setNEnergy(energyInit);
}

void ofApp::applyLevelPreset(int level) {
    if (level == 1) {
        energyInit = 10;
        playerSpeed = 35;
        playerScale = 1;
        rate = 1;
        life = 5;
        nAgents = 1;
        agentSpeed = 75;
        agentScale = 1;
    } else if (level == 2) {
        energyInit = 15;
        playerSpeed = 50;
        playerScale = 1.2;
        rate = 1.2;
        life = 6;
        nAgents = 1;
        agentSpeed = 90;
        agentScale = 1.2;
    } else if (level == 3) {
        energyInit = 20;
        playerSpeed = 70;
        playerScale = 1.3;
        rate = 1.1;
        life = 6;
        nAgents = 2;
        agentSpeed = 100;
        agentScale = 1.3;
    } else {
        return;
    }
    energy = energyInit;
}
