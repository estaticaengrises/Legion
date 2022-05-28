//
//  Ray.cpp
//  Project 2
//
//  Created by Ayan Abhiranya Singh on 4/9/22.
//
#include "ofApp.h"
#include <stdio.h>

void Ray::draw() {
    ofPushMatrix();
    ofMultMatrix(getMatrix());
    if(explode) ofDrawCircle(verts[1], 45);
    ofDrawLine(verts[0], verts[1]); // draw the ray
    ofPopMatrix();
}


void Ray::update()
{
//    cout << "Position of ray: " << pos << endl;
    if(explode) cout << "Explode is: "  << explode << endl;
    accelerate(1); // accelerate along heading vector
    integrate(); // apply force along the header which is locked on the target (ie the player)
    resetForces();
}

void Ray::explosion()
{
    ofSetColor(ofColor::yellow);
    ofDrawCircle(verts[1], 45);
}

void RayEmitter::draw()
{
    if(emit)
    {
        for (auto it = rays.begin(); it != rays.end(); it++)
        {
            it->draw();
        }
    }
}

void RayEmitter::destroy()
{
    rays.clear();
    vector<Ray>().swap(rays);
}

void RayEmitter::startEmitter()
{
    emit = true;
    startTime = ofGetElapsedTimeMillis();
    ray_soundEffect.load("sounds/Shoot.wav");
    ray_soundEffect.setVolume(0.1);
//    for(int i = 0;i<nRays; i++) createAgent();
}

void RayEmitter::shootRay()
{
    if(rays.size() == nRays) return;
    
    float size = 5;
    Ray ray = Ray(glm::vec3(0, size, 0), glm::vec3(0, -size, 0));
    ray.pos = pos;
    ray.rotation = rotation;
    rays.push_back(ray);
    ray_soundEffect.play();
}

void RayEmitter::update()
{
    for (auto it = rays.begin(); it != rays.end(); it++)
    {
        if (it->getElapsedTimeInSeconds() > lifespan)
        {
            rays.erase(it--); // delete the element if the lifespan exceeds the defined lifespan and decrement iterator.
        }
        else it->update();
    }
}
