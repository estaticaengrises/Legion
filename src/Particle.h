//
//  Particle.h
//  Project 2
//
//  Created by Ayan Abhiranya Singh on 4/10/22.
//

#ifndef Particle_h
#define Particle_h


class Particle : public BaseObject {
public:
    Particle() {}
    Particle(glm::vec3 p, float rad) {
        objectType = 1;
        verts.push_back(glm::vec3(0, rad, 0));
        pos = p;
        radius = rad;
        startTime = ofGetElapsedTimeMillis();
        thrust = 50;
    }
    
    void draw();
    void update();
    void addTurbulence();
    
    glm::vec3 target;
    bool lockedOn;
    bool explode;
    bool showTargetVector;
    float radius;
};

class ParticleEmitter : public Ray{
public:
    ParticleEmitter() {
        nParticles = 10;
        lifespan = 3;
        rate = 15;
        size = 5;
        emit = false;
        explosion_soundEffect.load("sounds/ChunkyExplosion.mp3");
        explosion_soundEffect.setVolume(0.5);
    }
    
    void draw();
    void destroy();
    
    void startEmitter();
    void generateParticles();
    void explode(glm::vec3 epicenter);
    void update();
    
    int nParticles;
    float rate;
    float size;
    float lifespan;
    bool emit;
    
    vector<Particle> particles;
    ofSoundPlayer explosion_soundEffect;
};

#endif /* Particle_h */
