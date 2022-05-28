//
//  Player.cpp
//  Project 2
//
//  Created by Ayan Abhiranya Singh on 3/29/22.
//

#include <stdio.h>
#include "ofApp.h"


void Player::setup(){
    pos = glm::vec3(ofGetWindowWidth()/2.0, ofGetWindowHeight()/2.0, 0);
    header = glm::vec3(0,0,0);
    thrust = 500.0;
    spriteHeight = 70;
    spriteWidth = 70;
    setMovementToWindow();
}

void Player::draw() {
    ofSetColor(color);
    
    if(setFill) ofFill();
    else ofNoFill();
    
    
    ofPushMatrix();
    ofMultMatrix(getMatrix());
    if(showSprite)
    {
        // ofNoFill();
        ofSetColor(255, 255, 255);
        image.resize(spriteWidth, spriteHeight); // resizing the image
        image.draw(-spriteWidth/2.0, -spriteHeight/2.0); // draw the image centered at origin
        ofSetColor(color);
        // add display sprite logic here!
    }
    else ofDrawTriangle(verts[0], verts[1], verts[2]);
    
    
    if(showHeader) ofDrawLine(glm::vec3(0,0,0), 75*heading());
    ofPopMatrix();

}

void Player::update() {
    integrate();
    resetForces();
}

void Player::destroy() {
    // keyMap.clear();
}
