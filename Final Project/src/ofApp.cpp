
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Octree Test - startup scene
// 
//
//  Student Name:   < Your Name goes Here >
//  Date: <date of last version>


#include "ofApp.h"
#include "Util.h"


//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
    // load image
    if (!backgroundImage.load("images/background2.png")) {
        cout << "Can't open background image file" << endl;
        ofExit();
    }
    
    // load texture
    ofDisableArbTex();
    if (!ofLoadImage(particleTexture, "images/dot.png")) {
        cout << "Can't open dot texture file" << endl;
        ofExit();
    }
    
    // load shader
#ifdef TARGET_OPENGLES
    shader.load("shaders_gles/shader");
#else
    shader.load("shaders/shader");
#endif
    
    // load sounds
    if (!backgroundSound.load("sounds/space.wav")) {
        cout << "Can't open space sound file" << endl;
        ofExit();
    }
    if (!thrusterSound.load("sounds/thruster.aiff")) {
        cout << "Can't open thruster sound file" << endl;
        ofExit();
    }
    if (!collisionSound.load("sounds/collision.wav")) {
        cout << "Can't open collision sound file" << endl;
        ofExit();
    }
    backgroundSound.setLoop(true);
    thrusterSound.setVolume(.6);
    collisionSound.setMultiPlay(true);
    collisionSound.setVolume(.6);
    
    // load font
    if (!verdana12.load("fonts/verdana.ttf", 12, false)) {
        cout << "Can't open verdana font file" << endl;
        ofExit();
    }
    
	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
	bTerrainSelected = true;
//	ofSetWindowShape(1024, 768);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofEnableDepthTest();

	// setup rudimentary lighting 
	//
	initLightingAndMaterials();

    // load mars terrain
	//mars.loadModel("geo/mars-low-5x-v2.obj");
    moon.loadModel("geo/moon-houdini.obj");
	moon.setScaleNormalization(false);
    moonMesh = moon.getMesh(0);
    
    // load lander
    lander.loadModel("geo/lander.obj");
    bLanderLoaded = true;
    lander.setScaleNormalization(false);
    lander.setPosition(-200, 67, -200);
    //lander.setPosition(0, 10, 0);
    landerPos = lander.getPosition();
    lander.velocity = ofVec3f(15, 0, 15);
    lander.setRotation(0, 45, 0, 1, 0);
    lander.rotation = 45;
    bboxList.clear();
    for (int i = 0; i < lander.getMeshCount(); i++) {
        bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
    }
    glm::vec3 min = lander.getSceneMin();
    glm::vec3 max = lander.getSceneMax();
    float offset = (max.y - min.y) / 2.0;
    landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

	// create sliders for testing
	//
	gui.setup();
	//gui.add(numLevels.setup("Number of Octree Levels", 1, 1, 15));
    //gui.add(bTimingInfo.setup("Print timing info", false));
    //gui.add(camPos.setup("Cam", ofVec3f(0, 0, 0), ofVec3f(-300, 0, -300), ofVec3f(300, 50, 300)));
	bHide = false;

	//  Create Octree for testing.
	//
    //timeCreateOctreeStart = ofGetElapsedTimeMillis();
	octree.create(moon.getMesh(0), 20);
    //timeCreateOctreeEnd = ofGetElapsedTimeMillis();
	
	cout << "Number of Verts: " << moon.getMesh(0).getNumVertices() << endl;

	testBox = Box(Vector3(3, 3, 0), Vector3(5, 5, 2));
    
    // set up lights
    keyLight.setup();
    keyLight.enable();
    keyLight.setPointLight();
    keyLight.setAmbientColor(ofFloatColor(.1));
    keyLight.setDiffuseColor(ofFloatColor(.6));
    keyLight.setSpecularColor(ofFloatColor(.8));
    keyLight.setPosition(-100, 70, -250);
    
    fillLight.setup();
    fillLight.enable();
    fillLight.setPointLight();
    fillLight.setAmbientColor(ofFloatColor(.33));
    fillLight.setDiffuseColor(ofFloatColor(.5));
    fillLight.setSpecularColor(ofFloatColor(.7));
    fillLight.setPosition(200, 300, 80);
    
    landerLight.setup();
    landerLight.enable();
    landerLight.setAreaLight(1, 1);
    landerLight.setAmbientColor(ofFloatColor(.2));
    landerLight.setDiffuseColor(ofFloatColor(.6));
    landerLight.setSpecularColor(ofFloatColor(1));
    landerLight.rotate(-90, ofVec3f(1, 0, 0));
    landerLight.setPosition(landerPos.x, landerPos.y, landerPos.z);
    
    ofSetFrameRate(60);
    fuelRemainingInMs = 120000; // 2 minutes
    
    landingAreaCenter = glm::vec3(0, -0.5, 0);
    landingAreaWidth = 10;
    landingAreaHeight = 1;
    
    particleRadius = 10;
    lander.emitter.setEmitterType(CircularDirectionalEmitter);
    lander.emitter.setGroupSize(100);
    lander.emitter.setLifespan(2);
    lander.emitter.setRate(100);
    lander.emitter.setRadius(.5);
    lander.emitter.setVelocity(ofVec3f(0, -5, 0));
    lander.emitter.start();
    
    explosionEmitter.setEmitterType(HalfSphereEmitter);
    explosionEmitter.setGroupSize(10000);
    explosionEmitter.setOneShot(true);
    explosionEmitter.setLifespan(8);
    explosionEmitter.setVelocity(ofVec3f(5));
    GravityForce *gravity = new GravityForce(ofVec3f(0, -1.62, 0));
    explosionEmitter.sys->addForce(gravity);
    
    // set up cameras
    cam.setNearClip(.1);
    cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
    cam.setTarget(landerPos);
    cam.setDistance(10);
    cam.disableMouseInput();
    
    below.setPosition(landerPos.x + 2, landerPos.y + 4.5, landerPos.z + 2);
    below.lookAt(glm::vec3(landerPos.x + 2, landerPos.y - 5, landerPos.z + 2));
    below.setNearClip(.1);
    below.setFov(65.5);
    
    trackingCloseUp.setPosition(0, 1, 20);
    trackingCloseUp.lookAt(landerPos);
    trackingCloseUp.setNearClip(.1);
    trackingCloseUp.setFov(80);
    
    trackingHill.setPosition(193, 7, 194);
    trackingHill.lookAt(landerPos);
    trackingHill.setNearClip(.1);
    trackingHill.setFov(30);
    
    fixedPlane.setPosition(-107, 1, 160);
    fixedPlane.lookAt(glm::vec3(0, 3, 0));
    fixedPlane.setNearClip(.1);
    fixedPlane.setFov(65);
    
    fixedValley.setPosition(-165, 14, -113);
    fixedValley.lookAt(glm::vec3(0, 3, 0));
    fixedValley.setNearClip(.1);
    fixedValley.setFov(50);
    
    revoling.setPosition(landerPos.x + 7, landerPos.y + 4, landerPos.z);
    revoling.lookAt(glm::vec3(landerPos.x, landerPos.y + 2, landerPos.z));
    revoling.setNearClip(.1);
    revoling.setFov(65.5);
    revolingCamRadius = 7;
    revolingCamIncrement = 0.01;
    
    currCam = &cam;
}
 
//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
    /*
    if (bTimingInfo) {
        // print time to create octree if not already printed
        if (!timeCreatedOctreePrinted) {
            timeCreateOctree = timeCreateOctreeEnd - timeCreateOctreeStart;
            cout << "Time to create octree: " << timeCreateOctree << " ms (" << timeCreateOctree / 1000.0 << " s)" << endl;
            timeCreatedOctreePrinted = true;
        }
        
        // print time to test ray selection is not already printed
        while (raySelectionLastPrintIndex < timeRaySelectionStartVec.size()) {
            timeRaySelection = timeRaySelectionEndVec[raySelectionLastPrintIndex] - timeRaySelectionStartVec[raySelectionLastPrintIndex];
            timeRaySelectionVec.push_back(timeRaySelection);
            cout << "Time to search with ray selection: " << timeRaySelection << " µs (" << timeRaySelection / 1000.0 << " ms)" << endl;
            raySelectionLastPrintIndex++;
        }
    }*/
    
    // update lander
    if (bLanderLoaded && bStarted) {
        landerPos = lander.getPosition();
        if (bAutopilot && fuelRemainingInMs > 0 && !scoreComputed) autopilot();
        lander.setHasFuel(fuelRemainingInMs > 0);
        lander.integrate();
        lander.update();
        altitude = getAltitude();
        explosionEmitter.setPosition(landerPos);
        explosionEmitter.update();
        landerLight.setPosition(landerPos.x, landerPos.y, landerPos.z);
        
        ofVec3f dist = ofVec3f(landingAreaCenter.x - landerPos.x, 0, landingAreaCenter.z - landerPos.z);
        //cout << "dist: " << dist.length() << "         " << altitude << endl;
        
        // update cameras
        below.setPosition(landerPos.x + 2, landerPos.y + 4.5, landerPos.z + 2);
        below.lookAt(glm::vec3(landerPos.x + 2, landerPos.y - 5, landerPos.z + 2));
        trackingCloseUp.lookAt(landerPos);
        trackingHill.lookAt(landerPos);

        revolingCamRadian += revolingCamIncrement;
        float x = revolingCamRadius * cos(revolingCamRadian);
        float z = revolingCamRadius * sin(revolingCamRadian);
        if (revolingCamRadian > maxRad) revolingCamRadian -= maxRad;
        revoling.setPosition(landerPos.x + revolingCamRadius * cos(revolingCamRadian), landerPos.y + 4, landerPos.z + revolingCamRadius * sin(revolingCamRadian));
        revoling.lookAt(glm::vec3(landerPos.x, landerPos.y + 2, landerPos.z));
        
        // update collision box
        ofVec3f min = lander.getSceneMin() + landerPos;
        ofVec3f max = lander.getSceneMax() + landerPos;
        Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

        // compute collision response
        colBoxList.clear();
        colIdxList.clear();
        octree.intersect(bounds, octree.root, colBoxList, colIdxList);
        
        if (colBoxList.size() > 0) {
            ofVec3f sum;
            for (int i = 0; i < colIdxList.size(); i++) {
                sum += moonMesh.getNormal(colIdxList[i]);
            }
            lander.setNormal(sum.getNormalized());
            lander.setCollision(true);
        } else {
            lander.setCollision(false);
        }
        
        // compute score
        if ((lander.bLanded || lander.bCrashed) && !scoreComputed) {
            computeScore();
        }
    }
    
    // update fuel remaining
    numThruster = numThrusterApplied();
    if (numThruster > 0) {
        // framerate = 60/s
        // called 60 times/s
        // 1s = 1000ms
        // -= 1000/framerate = 1000/60 = 16.667ms
        float time = 1000 / ofGetFrameRate() * numThruster;
        if (fuelRemainingInMs - time >= 0) {
            fuelRemainingInMs -= time;
        } else {
            fuelRemainingInMs = 0;
        }
    }
    
    // play sounds
    if (!backgroundSound.isPlaying()) {
        backgroundSound.play();
    }
    if (numThruster > 0 && !thrusterSound.isPlaying()) {
        thrusterSound.play();
    } else if (numThruster == 0 && thrusterSound.isPlaying()) {
        thrusterSound.stop();
    }
}
//--------------------------------------------------------------
void ofApp::draw() {
    
	ofBackground(ofColor::black);
    
    // draw background
	glDepthMask(false);
    
    ofSetColor(ofColor::white);
    backgroundImage.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    // draw UI
    ofSetColor(ofColor::white);
	//if (!bHide) verdana12.drawString("Framerate: " + to_string((int) ofGetFrameRate()), 60, 40);
    verdana12.drawString("Fuel Remaining: " + timeToString(fuelRemainingInMs), ofGetWindowWidth() - 300, 40);
    if (bAltitude) verdana12.drawString("Altitude: " + to_string(altitude), ofGetWindowWidth() - 300, 60);
    if (scoreComputed && !bHide) verdana12.drawString("Score: " + to_string(score), ofGetWindowWidth() / 2, 40);
	glDepthMask(true);
    
	currCam->begin();
    
    // draw models
	ofPushMatrix();
	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		moon.drawWireframe();
		if (bLanderLoaded) {
            lander.drawWireframe();
            if (!bTerrainSelected) drawAxis(landerPos);
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		moon.drawFaces();
		ofMesh mesh;
		if (bLanderLoaded) {
            lander.drawFaces();
            if (!bTerrainSelected) drawAxis(landerPos);
			if (bDisplayBBoxes) {
				ofNoFill();
				ofSetColor(ofColor::white);
				for (int i = 0; i < lander.getNumMeshes(); i++) {
					ofPushMatrix();
                    ofMultMatrix(lander.getModelMatrix());
					Octree::drawBox(bboxList[i]);
					ofPopMatrix();
				}
			}
            
            // draw colliding boxes
            //
            ofSetColor(ofColor::lightBlue);
            ofNoFill();
            for (int i = 0; i < colBoxList.size(); i++) {
                Octree::drawBox(colBoxList[i]);
            }

			if (bLanderSelected) {
                ofVec3f min = lander.getSceneMin() + landerPos;
                ofVec3f max = lander.getSceneMax() + landerPos;

				Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
				ofSetColor(ofColor::white);
                ofNoFill();
				//Octree::drawBox(bounds);
			}
		}
	}
	if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));

	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		moon.drawVertices();
	}

	// highlight selected point (draw sphere around selected point)
	//
	if (bPointSelected) {
		ofSetColor(ofColor::blue);
		ofDrawSphere(selectedPoint, .1);
	}


	// recursively draw octree
	//
	ofDisableLighting();
	int level = 0;
	//	ofNoFill();

	if (bDisplayLeafNodes) {
		octree.drawLeafNodes(octree.root);
		//cout << "num leaf: " << octree.numLeaf << endl;
    }
	else if (bDisplayOctree) {
		ofNoFill();
		ofSetColor(ofColor::white);
		octree.draw(numLevels, 0);
        //octree.drawLeafNodes(octree.root);
	}

	// if point selected, draw a sphere
	//
	if (pointSelected) {
		ofVec3f p = octree.mesh.getVertex(selectedNode->points[0]);
		ofVec3f d = p - cam.getPosition();
		ofSetColor(ofColor::lightGreen);
		ofDrawSphere(p, .02 * d.length());
	}
    
    // draw landing area
    ofSetColor(ofColor::yellow);
    ofNoFill();
    ofDrawBox(landingAreaCenter, landingAreaWidth, landingAreaHeight, landingAreaWidth);
    
    // draw particle emitter
    loadThrusterVbo();
    ofSetColor(255, 40, 40);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();
    shader.begin();
    particleTexture.bind();
    thrusterVbo.draw(GL_POINTS, 0, (int) lander.emitter.sys->particles.size());
    particleTexture.unbind();
    
    if (lander.bCrashed) {
        if (!explosionEmitter.started) explosionEmitter.start();
        if (!collisionSound.isPlaying()) collisionSound.play();
        loadExplosionVbo();
        particleTexture.bind();
        explosionVbo.draw(GL_POINTS, 0, (int) explosionEmitter.sys->particles.size());
        particleTexture.unbind();
    }

    ofPopMatrix();
    cam.end();
    shader.end();
    
    ofDisablePointSprites();
    ofDisableBlendMode();
    ofEnableAlphaBlending();
}


// 
// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
	

	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {
	switch (key) {
    case 'Q':
    case 'q':
        bAutopilot = !bAutopilot;
        break;
	case 'B':
	case 'b':
		bDisplayBBoxes = !bDisplayBBoxes;
		break;
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
        bHide = !bHide;
		break;
	case 'L':
	case 'l':
		bDisplayLeafNodes = !bDisplayLeafNodes;
		break;
	case 'O':
	case 'o':
		bDisplayOctree = !bDisplayOctree;
		break;
    case 'Z':
    case 'z':
        bAltitude = !bAltitude;
        break;
    case 'X':
    case 'x':
        scoreComputed = false;
        break;
    case 'r':
		cam.reset();
		break;
	case 'p':
		savePicture();
		break;
	case 't':
		setCameraTarget();
		break;
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 'i':
		toggleWireframeMode();
		break;
    case 'w':
    case 'W': // move forward
        if (fuelRemainingInMs > 0) lander.setZMotion(Lander::Forward);
        bZAxis = true;
        break;
    case 's':
    case 'S': // move backward
        if (fuelRemainingInMs > 0) lander.setZMotion(Lander::Backward);
        bZAxis = true;
        break;
    case 'a':
    case 'A': // move left
        if (fuelRemainingInMs > 0) lander.setXMotion(Lander::Left);
        bXAxis = true;
        break;
    case 'd':
    case 'D': // move right
        if (fuelRemainingInMs > 0) lander.setXMotion(Lander::Right);
        bXAxis = true;
        break;
    case OF_KEY_UP: // apply thruster
        bThruster = true;
        if (fuelRemainingInMs > 0) lander.setThruster(bThruster);
        break;
    case OF_KEY_LEFT: // rotate counter clockwise
        if (fuelRemainingInMs > 0) lander.setRMotion(Lander::Counterclockwise);
        bRotation = true;
        break;
    case OF_KEY_RIGHT: // rotate clockwise
        if (fuelRemainingInMs > 0) lander.setRMotion(Lander::Clockwise);
        bRotation = true;
        break;
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
    case ' ':
        bStarted = true;
        break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
    case '1':
        currCam = &cam;
        break;
    case '2':
        currCam = &below;
        break;
    case '3':
        currCam = &trackingCloseUp;
        break;
    case '4':
        currCam = &trackingHill;
        break;
    case '5':
        currCam = &fixedPlane;
        break;
    case '6':
        currCam = &fixedValley;
        break;
    case '7':
        currCam = &revoling;
        break;
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {

	switch (key) {
    case 'w':
    case 'W': // forward
    case 's':
    case 'S': // backward
        lander.setZMotion(Lander::IdleZ);
        bZAxis = false;
        break;
    case 'a':
    case 'A': // left
    case 'd':
    case 'D': // right
        lander.setXMotion(Lander::IdleX);
        bXAxis = false;
        break;
    case OF_KEY_UP: // thruster
        bThruster = false;
        lander.setThruster(bThruster);
        break;
    case OF_KEY_LEFT: // counterclockwise
    case OF_KEY_RIGHT: // clockwise
        lander.setRMotion(Lander::IdleR);
        bRotation = false;
        break;
	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
	default:
		break;

	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

	
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	// if rover is loaded, test for selection
	//
	if (bLanderLoaded && !bStarted) {
		glm::vec3 origin = cam.getPosition();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

        ofVec3f min = lander.getSceneMin() + landerPos;
        ofVec3f max = lander.getSceneMax() + landerPos;

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		bool hit = bounds.intersect(Ray(Vector3(origin.x, origin.y, origin.z), Vector3(mouseDir.x, mouseDir.y, mouseDir.z)), 0, 10000);
		if (hit) {
			bLanderSelected = true;
            mouseDownPos = getMousePointOnPlane(landerPos, cam.getZAxis());
			mouseLastPos = mouseDownPos;
			bInDrag = true;
		}
		else {
			bLanderSelected = false;
		}
	}
	else {
		ofVec3f p;
        //timeRaySelectionStartVec.push_back(ofGetElapsedTimeMicros());
		raySelectWithOctree(p);
        //timeRaySelectionEndVec.push_back(ofGetElapsedTimeMicros());
	}
}

bool ofApp::raySelectWithOctree(ofVec3f &pointRet) {
	ofVec3f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
		Vector3(rayDir.x, rayDir.y, rayDir.z));

	pointSelected = octree.intersect(ray, octree.root, selectedNode);

	if (pointSelected) {
		pointRet = octree.mesh.getVertex(selectedNode->points[0]);
	}
	return pointSelected;
}

float ofApp::getAltitude() {
    ofVec3f pos = landerPos;
    Ray ray = Ray(Vector3(pos.x, pos.y, pos.z), Vector3(0, -1, 0));
    
    pointIntersected = octree.intersect(ray, octree.root, selectedNode);
    
    if (pointIntersected) {
        ofVec3f point = octree.mesh.getVertex(selectedNode->points[0]);
        return pos.y - point.y;
    } else {
        return -1;
    }
}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;

	if (bInDrag && !bStarted) {
		glm::vec3 mousePos = getMousePointOnPlane(landerPos, cam.getZAxis());
		glm::vec3 delta = mousePos - mouseLastPos;
	
		landerPos += delta;
        lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
		mouseLastPos = mousePos;

        ofVec3f min = lander.getSceneMin() + landerPos;
        ofVec3f max = lander.getSceneMax() + landerPos;

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

		colBoxList.clear();
		octree.intersect(bounds, octree.root, colBoxList);
	

		/*if (bounds.overlap(testBox)) {
			cout << "overlap" << endl;
		}
		else {
			cout << "OK" << endl;
		}*/


	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bInDrag = false;
}



// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {
    cam.setTarget(landerPos);
    cam.setDistance(10);
}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent2(ofDragInfo dragInfo) {

	ofVec3f point;
	mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);
	if (lander.loadModel(dragInfo.files[0])) {
		lander.setScaleNormalization(false);
//		lander.setScale(.1, .1, .1);
	//	lander.setPosition(point.x, point.y, point.z);
		lander.setPosition(1, 1, 0);

		bLanderLoaded = true;
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		//cout << "Mesh Count: " << lander.getMeshCount() << endl;
	}
	else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (lander.loadModel(dragInfo.files[0])) {
		bLanderLoaded = true;
		lander.setScaleNormalization(false);
		lander.setPosition(0, 0, 0);
		//cout << "number of meshes: " << lander.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		//		lander.setRotation(1, 180, 1, 0, 0);

				// We want to drag and drop a 3D object in space so that the model appears 
				// under the mouse pointer where you drop it !
				//
				// Our strategy: intersect a plane parallel to the camera plane where the mouse drops the model
				// once we find the point of intersection, we can position the lander/lander
				// at that location.
				//

				// Setup our rays
				//
		glm::vec3 origin = cam.getPosition();
		glm::vec3 camAxis = cam.getZAxis();
		glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPoint = origin + distance * mouseDir;

			// Now position the lander's origin at that intersection point
			//
            glm::vec3 min = lander.getSceneMin();
            glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);

			// set up bounding box for lander while we are at it
			//
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
	}


}

//  intersect the mouse ray with the plane normal to the camera 
//  return intersection point.   (package code above into function)
//
glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
	// Setup our rays
	//
	glm::vec3 origin = cam.getPosition();
	glm::vec3 camAxis = cam.getZAxis();
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}

string ofApp::timeToString(int ms) {
    int min = ms / 60000;
    ms = ms % 60000;
    string time = "";
    if (min > 0) {
        if (min < 10) time += "0";
        time += to_string(min);
    } else {
        time += "00";
    }
    time += ":";
    int sec = ms / 1000;
    ms = ms % 1000;
    if (sec < 10) time += "0";
    time += to_string(sec);
    int mil = ms / 100;
    time += ".";
    time += to_string(mil);
    return time;
}

int ofApp::numThrusterApplied() {
    int num = 0;
    
    if (bThruster) num++;
    if (bXAxis) num++;
    if (bZAxis) num++;
    if (bRotation) num++;
    
    lander.setKeyPressed(num > 0);
    return num;
}

void ofApp::computeScore() {
    int scoreSpeed = (int) (20.0 / lander.velocity.length());
    
    ofVec2f dist(landerPos.x - landingAreaCenter.x, landerPos.z - landingAreaCenter.z);
    int scoreDistance = (int) (50.0 / dist.length());
    
    if (scoreSpeed < 0) scoreSpeed = 0;
    else if (scoreSpeed > 50) scoreSpeed = 50;
    
    if (scoreDistance < 0) scoreDistance = 0;
    else if (scoreDistance > 50) scoreDistance = 50;
    
    cout << "distance: " << dist << "; distance score: " << scoreDistance << endl;
    cout << "speed: " << lander.velocity.length() << "; speed score: " << scoreSpeed << endl;
    score = scoreSpeed + scoreDistance;
    scoreComputed = true;
    bAutopilot = false;
    lander.setThruster(false);
    bThruster = false;
    lander.setZMotion(Lander::IdleZ);
    bZAxis = false;
}

// load vertex buffer in preparation for rendering
void ofApp::loadThrusterVbo() {
    if (lander.emitter.sys->particles.size() < 1) return;
    
    vector<ofVec3f> sizesThruster;
    vector<ofVec3f> pointsThruster;
    for (int i = 0; i < lander.emitter.sys->particles.size(); i++) {
        pointsThruster.push_back(lander.emitter.sys->particles[i].position);
        sizesThruster.push_back(ofVec3f(particleRadius));
    }
    
    // upload the data to the vbo=
    int totalThruster = (int) pointsThruster.size();
    thrusterVbo.clear();
    thrusterVbo.setVertexData(&pointsThruster[0], totalThruster, GL_STATIC_DRAW);
    thrusterVbo.setNormalData(&sizesThruster[0], totalThruster, GL_STATIC_DRAW);
}

void ofApp::loadExplosionVbo() {
    if (explosionEmitter.sys->particles.size() < 1) return;
    
    vector<ofVec3f> sizesExplosion;
    vector<ofVec3f> pointsExplosion;
    for (int i = 0; i < explosionEmitter.sys->particles.size(); i++) {
        pointsExplosion.push_back(explosionEmitter.sys->particles[i].position);
        sizesExplosion.push_back(ofVec3f(particleRadius));
    }
    
    // upload the data to the vbo=
    int totalExplosion = (int) pointsExplosion.size();
    explosionVbo.clear();
    explosionVbo.setVertexData(&pointsExplosion[0], totalExplosion, GL_STATIC_DRAW);
    explosionVbo.setNormalData(&sizesExplosion[0], totalExplosion, GL_STATIC_DRAW);
}

// auto land, for demo purposes only
// does not handle rotation in this version, the lander must aim perfectly at the landing area
void ofApp::autopilot() {
    ofVec3f velocity = lander.velocity;
    float gravity = -lander.gravityForce.y;
    float t = altitude / -velocity.y;
    //cout << t << endl;
    
    glm::vec3 displacement = landingAreaCenter - landerPos;
    ofVec3f dist = ofVec3f(landingAreaCenter.x - landerPos.x, 0, landingAreaCenter.z - landerPos.z);
    float distLen = dist.length();
    //glm::vec3 n = glm::normalize(dist);
    //glm::vec3 heading = glm::vec3(cos(lander.rotation * PI / 180), 0, -sin(lander.rotation * PI / 180));

    float height = 4 * sqrt(distLen);
    float speed = sqrt(distLen);
    
    // adjust height
    if (height < 15) { // close to landing
        if (height < 5) { // very close to landing
            if (velocity.y < -0.5) { // descending too fast
                lander.setThruster(true);
                bThruster = true;
            } else {
                lander.setThruster(false);
                bThruster = false;
            }
        } else {
            if (velocity.y < -1) { // descending too fast
                lander.setThruster(true);
                bThruster = true;
            } else {
                lander.setThruster(false);
                bThruster = false;
            }
        }
    } else { // follow the recommended height
        if (altitude < height) {
            lander.setThruster(true);
            bThruster = true;
        } else {
            lander.setThruster(false);
            bThruster = false;
        }
    }
    
    // adjust speed
    //cout << "vel len: " << velocity.length() << ", speed: " << speed << endl;
    if (velocity.x > 0) { // right direction - follow the recommended speed
        if (velocity.length() < speed - 0.1) {
            //cout << "forward" << endl;
            lander.setZMotion(Lander::Forward);
            bZAxis = true;
        } else if (velocity.length() > speed + 0.1){
            //cout << "backward" << endl;
            lander.setZMotion(Lander::Backward);
            bZAxis = true;
        } else {
            lander.setZMotion(Lander::IdleZ);
            bZAxis = false;
        }
    } else { // opposite direction - go forward
        //cout << "opposite" << endl;
        lander.setZMotion(Lander::Forward);
        bZAxis = true;
    }
    
    /*
    float angle = glm::angle(heading, n);
    float dot = glm::dot(heading, n);
    if (dot < 1) {
        glm::vec3 cross = glm::cross(heading, n);
        if (cross.y > 0) angle = -angle;
    }*/
    //cout << n << "; " << lander.rotation << "; " << heading << endl;
    //cout << angle << endl;
    /*
    if (velocity.y < 0) {
        lander.setThruster(true);
        bThruster = true;
    } else {
        lander.setThruster(false);
        bThruster = false;
    }*/
    /*
    if (angle < -0.09) { // about 5 degree
        lander.setRMotion(Lander::Clockwise);
        bRotation = true;
    } else if (angle > 0.09) {
        lander.setRMotion(Lander::Counterclockwise);
        bRotation = true;
    } else {
        lander.setRMotion(Lander::IdleR);
        bRotation = false;
    }*/
}
