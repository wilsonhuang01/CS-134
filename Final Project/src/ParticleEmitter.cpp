
//  Kevin M. Smith - CS 134 SJSU

#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter() {
	sys = new ParticleSystem();
	createdSys = true;
	init();
}

ParticleEmitter::ParticleEmitter(ParticleSystem *s) {
	if (s == NULL)
	{
		cout << "fatal error: null particle system passed to ParticleEmitter()" << endl;
		ofExit();
	}
	sys = s;
	createdSys = false;
	init();
}

ParticleEmitter::~ParticleEmitter() {

	// deallocate particle system if emitter created one internally
	//
	if (createdSys) delete sys;
}

void ParticleEmitter::init() {
	rate = 1;
	velocity = ofVec3f(0, 20, 0);
	lifespan = 3;
	mass = 1;
	randomLife = false;
	lifeMinMax = ofVec3f(2, 4);
	started = false;
	oneShot = false;
	fired = false;
	lastSpawned = 0;
	radius = 1;
	particleRadius = .1;
	visible = true;
	type = DirectionalEmitter;
	groupSize = 1;
	damping = .99;
    active = false;
}



void ParticleEmitter::draw() {
	if (visible) {
		switch (type) {
		case DirectionalEmitter:
			ofDrawSphere(position, radius);  // just draw a small sphere for point emitters
			break;
		case SphereEmitter:
		case RadialEmitter:
			ofDrawSphere(position, radius);  // just draw a small sphere as a placeholder
			break;
		default:
			break;
		}
	}
	sys->draw();  
}
void ParticleEmitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void ParticleEmitter::stop() {
	started = false;
	fired = false;
}
void ParticleEmitter::update() {
	float time = ofGetElapsedTimeMillis();
	if (oneShot && started) {
		if (!fired) {

			// spawn a new particle(s)
			//
			for (int i = 0; i < groupSize; i++)
				spawn(time);

			lastSpawned = time;
            fired = true;
		}
		//stop();
	}

	else if (((time - lastSpawned) > (1000.0 / rate)) && started) {
        if (active) {
            // spawn a new particle(s)
            //
            for (int i= 0; i < groupSize; i++) {
                spawn(time);
                //cout << "spawned" << endl;
            }

        
            lastSpawned = time;
        }
	}

	sys->update();
}

// spawn a single particle.  time is current time of birth
//
void ParticleEmitter::spawn(float time) {
	Particle particle;

	// set initial velocity and position
	// based on emitter type
	//
	switch (type) {
        case RadialEmitter: {
            ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
            float speed = velocity.length();
            particle.velocity = dir.getNormalized() * speed;
            particle.position.set(position);
            break; }
        case SphereEmitter:
            break;
        case DirectionalEmitter:
            particle.velocity = velocity;
            particle.position.set(position);
            break;
        case CircularDirectionalEmitter: {
            float r = radius * sqrt(ofRandom(1));
            float theta = ofRandom(2) * 3.14159;
            float x = r * cos(theta);
            float z = r * sin(theta);
            particle.position.set(position.x + x, position.y, position.z + z);
            particle.velocity = velocity + sqrt(abs(angularVelocity)) * ofVec3f(x, 0, z);
            break; }
        case HalfSphereEmitter: {
            ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(0, 1), ofRandom(-1, 1));
            float speed = velocity.length() * ofRandom(1);
            particle.velocity = dir.getNormalized() * speed;
            particle.position.set(position);
            break; }
	}

	// other particle attributes
	//
	if (randomLife) {
		particle.lifespan = ofRandom(lifeMinMax.x, lifeMinMax.y);
	}
	else particle.lifespan = lifespan;
	particle.birthtime = time;
	particle.radius = particleRadius;
	particle.mass = mass;
	particle.damping = damping;

	// add to system
	//
	sys->add(particle);
}
