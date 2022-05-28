//
//  Agent.cpp
//  Project 2
//
//  Created by Ayan Abhiranya Singh on 3/9/22.
//
#include "ofApp.h"

/*/ ------- AGENT FUNCTIONS ------- */

void Agent::draw() {
    
    if(showTargetVector) ofDrawLine(pos, target);
    
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
    if(showHeader)
    {
        if(showSprite)
        {
            ofDrawLine(glm::vec3(0,0,0), glm::vec3(0, -spriteHeight/2.0, 0));
        }
        else ofDrawLine(glm::vec3(0,0,0), 75*heading());
    }
    ofPopMatrix();
    
}

//bool Agent::inside(glm::vec3 p) {
//    
//    return inside(translatedPoint, verts[0], verts[1], verts[2]);
//}

//bool Agent::inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
//return false;
//}

void Agent::update(glm::vec3 playerPosition)
{
//    cout << "Position of agent: " << pos << endl;
    target = playerPosition;
    if(!lockedOn) seek(playerPosition); // improve the performace for seeking?
    else
    {
        // need to call integrate() now because we are using forces.
        // the force vector essentially is a normalized targeting vector
        chase(); // stay locked on to target
        accelerate(1); // accelerate along heading vector
        integrate(); // apply force along the header which is locked on the target (ie the player)
        resetForces();
    }
}

void Agent::seek(glm::vec3 playerPosition)
{
    
    glm::vec3 vector1 = pos;
    vector1 = vector1 - pos;
    
    glm::vec3 test = glm::vec3(3, 4, 0);
    glm::vec3 vector2 = target - pos;
    vector2 = glm::normalize(target - pos);
    
    float rads = glm::radians(rotation);
    
    float target_x_value = floorf(vector2.x * 10) / 10;
    float my_x_value = floorf(glm::sin(rads) * 10) / 10;
    
    float target_y_value = floorf(vector2.y * 10) / 10;
    float my_y_value = floorf(-glm::cos(rads) * 10) / 10;
    
    if(my_x_value == target_x_value && my_y_value == target_y_value) {
        lockedOn = true;
        return;
    }
    
    float rotateSpeed = 2.0;
    
    if(target.x >= pos.x)
    {
        if(target.y >= pos.y)
        {
            rotation -= rotateSpeed;
        }
        else
        {
            rotation += rotateSpeed;
        }
    }
    else
    {
        if(target.y >= pos.y)
        {
            rotation -= rotateSpeed;
        }
        else
        {
            rotation += rotateSpeed;
        }
    }
    
    if(rotation == 360) rotation = 0;

}

// can probably just use accelerate(1) instead of calling chase(). Look into this.
void Agent::chase(){
    
    glm::vec3 targetVector = target - pos;
    targetVector = glm::normalize(target - pos);
    
    float rads = glm::radians(rotation);
    
    float target_x_value = floorf(targetVector.x * 10) / 10;
    float my_x_value = floorf(glm::sin(rads) * 10) / 10;
    
    float target_y_value = floorf(targetVector.y * 10) / 10;
    float my_y_value = floorf(-glm::cos(rads) * 10) / 10;
    
    while(true)
    {
        rads = glm::radians(rotation);
        target_x_value = floorf(targetVector.x * 10) / 10;
        my_x_value = floorf(glm::sin(rads) * 10) / 10;
        
        target_y_value = floorf(targetVector.y * 10) / 10;
        my_y_value = floorf(-glm::cos(rads) * 10) / 10;
        
        if(my_x_value == target_x_value && my_y_value == target_y_value)
        {
            damping = 0.99;
            break;
        }
        else damping = 0.85;
        rotation +=0.5;
//        angularForce +=0.5;
    }
}

bool Agent::inside(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, ofImage targetImage) {

    glm::vec3 s = pos + glm::vec3(0, -spriteHeight/2.0, 0);
    if (s.x > p0.x && s.x < p1.x && s.y < p3.y && s.y > p1.y) {
        int x = s.x;
        int y = s.y;
        ofColor color = targetImage.getColor(x, y);
        return true;
        // return (color.a != 0);   // check if color is opaque (not the transparent background)
    }
    else
    {
        return false;
    }
}

/*/ ------- END AGENT FUNCTIONS ------- */

/*/ ------- BEGIN EMITTER FUNCTIONS ------- */

void AgentEmitter::draw()
{
    if(emit)
    {
        explosion.draw();
        for(int i =0;i<agents.size();i++) agents[i].draw();
    }
}

void AgentEmitter::update(glm::vec3 playerPosition)
{
    if(emit)
    {
        spawnAgents();
        explosion.update();
        // destroyAgents(); // deleting in checkCollisions, turn or later or implement functinoality in checkcolisons.
        updateAgents(playerPosition);
    }
}

void AgentEmitter::destroy()
{
    agents.clear();
    vector<Agent>().swap(agents);
}

void AgentEmitter::startEmitter()
{
    emit = true;
    startTime = ofGetElapsedTimeMillis();
    for(int i = 0;i<nAgents; i++) createAgent();
}

void AgentEmitter::createAgent()
{
    float create_x = ofRandom(0, ofGetWindowWidth());
    float create_y = ofRandom(0, ofGetWindowHeight());
    
    float size = 15;
    
    Agent agent = Agent(glm::vec3(-size, size, 0), glm::vec3(size, size, 0), glm::vec3(0, -size, 0), imagePath);
    
    agent.pos.x = create_x;
    agent.pos.y = create_y;
    
    agent.rotation = ofRandom(-180, 180);
    agent.showSprite = setAgentShowSprite;
    agent.showTargetVector = setAgentShowTargetVector;
    agent.color = color;
    
    agents.push_back(agent);
}

void AgentEmitter::updateAgents(glm::vec3 playerPosition)
{
    for(int i =0;i<agents.size();i++)
    {
        agents[i].update(playerPosition);
    }
}

void AgentEmitter::spawnAgents()
{
    if(getElapsedTimeInSeconds() > rate)
    {
        for(int i = 0;i<nAgents; i++) createAgent();
        
        startTime = ofGetElapsedTimeMillis();
    }
}

void AgentEmitter::destroyAgents()
{
    for (auto it = agents.begin(); it != agents.end(); it++)
    {
        if (it->getElapsedTimeInSeconds() > lifespan)
        {
            agents.erase(it--); // delete the element if the lifespan exceeds the defined lifespan and decrement iterator.
        }
    }
}

void AgentEmitter::toggleAgentSprites()
{
    for(int i=0;i<agents.size();i++)
    {
        agents[i].showSprite = !agents[i].showSprite;
    }
}

void AgentEmitter::setImagePath(string imagePath)
{
    for(int i=0;i<agents.size();i++)
    {
        agents[i].imagePath = imagePath;
    }
}

void AgentEmitter::updateParameters(int agent_spawnCount,
                                    float agent_lifespan, float agent_spawnRate, bool showTargetVector)
{
    nAgents = agent_spawnCount;
    rate = agent_spawnRate;
    lifespan = agent_lifespan;
    for(int i=0;i<agents.size();i++)
    {
        agents[i].imagePath = imagePath;
        agents[i].lifespan = agent_lifespan;
        agents[i].showSprite = setAgentShowSprite;
        agents[i].showTargetVector = showTargetVector;
    }
}

int AgentEmitter::checkCollisions(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, vector<Ray> rays)
{
    int numCollisions = 0;
    bool collision = false;
    
    rayCollisions = 0;
    
    for (auto it = agents.begin(); it != agents.end(); it++)
    {
        collision = false;
        for (auto ray = rays.begin(); ray != rays.end(); ray++)
        {
            if(it->BaseObject::inside(ray->pos))
            {
//                cout << "ray in agent" << endl;
                collision = true;
//                ray->explode = true;
//                rays[ctr].explode = true;
//                cout << "ray explode: " << ray->explode << endl;
                explosion.explode(ray->pos);
                rayCollisions++;
                rays.erase(ray--); // delete the ray
                agents.erase(it--); // delete the agent
                break;
            }
//            else cout << "nothing to report" << endl;
        }
        
        if(collision) continue;
        
        if (setAgentShowSprite && it->inside(p1, p2, p3, p4, targetImage))
        {
            //it->BaseObject::inside(glm::vec3 p, <#glm::vec3 p1#>, <#glm::vec3 p2#>, <#glm::vec3 p3#>)
            numCollisions++;
            agents.erase(it--); // delete the element if the lifespan exceeds the defined lifespan and decrement iterator.
        }
        else
        if (!setAgentShowSprite && it->BaseObject::inside(it->getPointInCanvasSpace(it->verts[2]), p1, p2, p3))
        {
            numCollisions++;
            agents.erase(it--); // delete the element if the lifespan exceeds the defined lifespan and decrement iterator.
        }
        else if (it->getElapsedTimeInSeconds() > lifespan)
        {
            agents.erase(it--); // delete the element if the lifespan exceeds the defined lifespan and decrement iterator.
        }
    }
    return numCollisions;
}


/*/ ------- END EMITTER FUNCTIONS ------- */
