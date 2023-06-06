#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "Octree.h"
#include "Lander.h"
#include "ParticleEmitter.h"
#include <glm/gtx/intersect.hpp>



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent2(ofDragInfo dragInfo);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(ofVec3f);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void toggleSelectTerrain();
		void setCameraTarget();
		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
		bool raySelectWithOctree(ofVec3f &pointRet);
		glm::vec3 getMousePointOnPlane(glm::vec3 p , glm::vec3 n);
        string timeToString(int ms);
        int numThrusterApplied();
        float getAltitude();
        void computeScore();
        void loadThrusterVbo();
        void loadExplosionVbo();
        void autopilot();

		ofEasyCam cam;
        ofCamera below;
        ofCamera trackingCloseUp;
        ofCamera trackingHill;
        ofCamera fixedPlane;
        ofCamera fixedValley;
        ofCamera revoling;
        float revolingCamRadian;
        float revolingCamRadius;
        float maxRad = 6.28318531; // 2 * PI
        float revolingCamIncrement;
        ofCamera *currCam;
    
		ofxAssimpModelLoader moon;
        ofMesh moonMesh;
        Lander lander;
        ofVec3f landerPos; // cache
		ofLight light;
		Box boundingBox, landerBounds;
		Box testBox;
		vector<Box> colBoxList;
        vector<int> colIdxList;
		bool bLanderSelected = false;
		Octree octree;
		TreeNode* selectedNode = new TreeNode();
		glm::vec3 mouseDownPos, mouseLastPos;
		bool bInDrag = false;

		ofxIntSlider numLevels;
        ofxToggle bTimingInfo;
        ofxVec3Slider camPos;
		ofxPanel gui;
    
        ofImage backgroundImage;

		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
        bool bHide;
		bool pointSelected = false;
		bool bDisplayLeafNodes = false;
		bool bDisplayOctree = false;
		bool bDisplayBBoxes = false;
		
		bool bLanderLoaded;
		bool bTerrainSelected;
	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;

		vector<Box> bboxList;

		const float selectionRange = 4.0;
    
        bool bStarted;
    
        // fuel info
        int fuelRemainingInMs;
        bool bThruster;
        bool bXAxis;
        bool bZAxis;
        bool bRotation;
        int numThruster;
    
        // altitude info
        float altitude;
        bool bAltitude;
        bool pointIntersected;
    
        // landing area info
        glm::vec3 landingAreaCenter;
        int landingAreaWidth;
        int landingAreaHeight;
        int score;
        bool scoreComputed;
    
        // particle rendering info
        ofTexture particleTexture;
        ofVbo thrusterVbo;
        ofVbo explosionVbo;
        ofShader shader;
        float particleRadius;
    
        ParticleEmitter explosionEmitter;
        bool bAutopilot;
    
        // lights
        ofLight keyLight;
        ofLight fillLight;
        ofLight landerLight;
    
        // sounds
        ofSoundPlayer backgroundSound;
        ofSoundPlayer thrusterSound;
        ofSoundPlayer collisionSound;
    
        // font
        ofTrueTypeFont verdana12;
    
        // timing info
        //long timeCreateOctreeStart;
        //long timeCreateOctreeEnd;
        //long timeCreateOctree;
        //bool timeCreatedOctreePrinted;
    
        //vector<long> timeRaySelectionStartVec;
        //vector<long> timeRaySelectionEndVec;
        //vector<long> timeRaySelectionVec;
        //long timeRaySelection;
        //int raySelectionLastPrintIndex;
};
