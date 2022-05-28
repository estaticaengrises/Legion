//
//  Agent.h
//  Project 2
//
//  Created by Ayan Abhiranya Singh on 4/8/22.
//

#ifndef Agent_h
#define Agent_h

#include "Ray.h"
#include "Particle.h"

class Agent : public BaseObject {
public:
    Agent() {}
    Agent(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, string path_to_image) {
        objectType = 3;
        verts.push_back(p1);
        verts.push_back(p2);
        verts.push_back(p3);
        thrust = 1.4;
        showHeader = false;
        showSprite = false;
        showTargetVector = false;
        lockedOn = false;
        startTime = ofGetElapsedTimeMillis();
        target = glm::vec3(0, 50, 0);
        imagePath = path_to_image;
        image.load(imagePath); // switch this back on (uncomment) in final release
        spriteHeight = 70;
        spriteWidth = 70;
        thrust = 250;
        
        //  damping = 0.95;
    }
    
    void draw();
    void update(glm::vec3 playerPosition);
    
    glm::vec3 target;
    bool lockedOn;
    bool showTargetVector;
    
    void seek(glm::vec3 playerPosition);
    void chase();
//    bool inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    bool inside(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, ofImage targetImage);
};

class AgentEmitter : public Agent{
public:
    AgentEmitter() {
        nAgents = 1;
        lifespan = 10;
        rate = 15;
        emit = false;
        setAgentShowSprite = false;
        setAgentShowTargetVector = false;
        imagePath = "images/prey-telepath.gif";
        rayCollisions = 0;
        
        explosion = ParticleEmitter();
        explosion.startEmitter();
    }
    
    void draw();
    void update(glm::vec3 playerPosition);
    void destroy();
    
    void startEmitter();
    void createAgent();
    void spawnAgents();
    void updateAgents(glm::vec3 playerPosition);
    void destroyAgents();
    void toggleAgentSprites();
    
    void setImagePath(string imagePath);
    void updateParameters(int agent_spawnCount, float agent_lifespan, float agent_spawnRate, bool showTargetVectors);
    // name AgentEmitter and use a handler() function to handle all agents active in the vector
    int checkCollisions(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,  glm::vec3 p4, vector<Ray> rays);
    
    int nAgents;
    float rate;
    float lifespan;
    
    bool setAgentShowSprite;
    bool setAgentShowTargetVector;
    bool emit;
    int rayCollisions;
    
    ofImage targetImage;
    ParticleEmitter explosion;
    
    vector<Agent> agents;
};


#endif /* Agent_h */

