//
//  RadialEmitter.cpp
//  Project 2
//
//  Created by Ayan Abhiranya Singh on 4/10/22.
//
#include "ofApp.h"
#include <stdio.h>


void Particle::draw() {
    ofDrawCircle(pos, radius);

    ofPushMatrix();
    ofMultMatrix(getMatrix());
//    cout << "Position of particle in draw(): " << pos << endl;
//    ofDrawCircle(pos, radius);
    ofPopMatrix();
}


void Particle::update()
{
//    cout << "Position of particle: " << pos << endl;
//    if(explode) cout << "Explode is: "  << explode << endl;
    accelerate(1); // accelerate along heading vector
    addTurbulence(); // add turbulence to add randomness to particle trajectories
    integrate(); // apply force along the header
    resetForces();
}

void Particle::addTurbulence()
{
    force *= ofRandom(200);
    force.x *= ofRandom(0.1, 0.9);
    force.y *= ofRandom(0.1, 0.9);
    force.z *= ofRandom(0.1, 0.9);

}

void ParticleEmitter::draw()
{
    if(emit)
    {
        for (auto it = particles.begin(); it != particles.end(); it++)
        {
            it->draw();
        }
    }
}

void ParticleEmitter::destroy()
{
    particles.clear();
    vector<Particle>().swap(particles);
}

void ParticleEmitter::startEmitter()
{
    emit = true;
    startTime = ofGetElapsedTimeMillis();
}

void ParticleEmitter::generateParticles()
{
    //if(particles.size() == nParticles) return;
    
    // float degrees = 360.0/nParticles;
    float degrees = ofRandom(360.0);
    
    float randomRadius = size + ofRandom(4);
    
    for(int i = 1; i <= nParticles; i++)
    {
        degrees = degrees * i;
        Particle particle = Particle(pos, randomRadius);
        particle.pos = pos;
        particle.rotation = degrees;
        particles.push_back(particle);
    }
}

void ParticleEmitter::explode(glm::vec3 epicenter)
{
    pos = epicenter;
     // pos = glm::vec3(512, 384, 0);
//    cout << "Position of particle: " << pos << endl;
    generateParticles();
    explosion_soundEffect.play();
}


void ParticleEmitter::update()
{
    for (auto it = particles.begin(); it != particles.end(); it++)
    {
        if (it->getElapsedTimeInSeconds() > lifespan)
        {
            particles.erase(it--); // delete the element if the lifespan exceeds the defined lifespan and decrement iterator.
        }
        else it->update();
    }
}
