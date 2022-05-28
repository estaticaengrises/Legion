//
//  Emitter.h
//  Project 2
//
//  Created by Ayan Abhiranya Singh on 4/8/22.
//

#ifndef BaseObject_h
#define BaseObject_h


#endif /* BaseObject_h */


class BaseObject {
public:
    BaseObject() {}
    virtual void draw() {}
    
    bool inside(glm::vec3 p) {
        glm::vec3 translatedPoint = glm::inverse(getMatrix()) * glm::vec4(p, 1);
        if(showSprite)
        {
            if(translatedPoint.x >= -1 * spriteWidth/2.0 && translatedPoint.x<= spriteWidth/2.0
               && translatedPoint.y>= -1 * spriteHeight/2.0 && translatedPoint.y<= spriteHeight/2.0)
            {
                int x = translatedPoint.x + spriteWidth / 2;
                int y = translatedPoint.y + spriteHeight / 2;
                ofColor color = image.getColor(x, y);
                return (color.a != 0);   // check if color is opaque (not the transparent background)
            }
            return false;
        }
        
        return inside(translatedPoint, verts[0], verts[1], verts[2]);
    }

    bool inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {

        glm::vec3 v1 = glm::normalize(p1 - p);
        glm::vec3 v2 = glm::normalize(p2 - p);
        glm::vec3 v3 = glm::normalize(p3 - p);
        float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
        float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
        float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
        if (a1 < 0 && a2 < 0 && a3 < 0) return true;
        else return false;
    }
    
    float getElapsedTimeInSeconds(){
        
        float currTime = ofGetElapsedTimeMillis();
        float seconds = 0.0;
        
        while(currTime>startTime+100)
        {
            seconds+=0.1;
            currTime-=100;
        }
        
        return seconds;
    }
    
    glm::mat4 getMatrix()
    {
        glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(pos));
        glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
        glm::mat4 sc = glm::scale(glm::mat4(1.0), scale);

        glm::mat4 T = trans * rot * sc;
        
        return T;
    }
    
    glm::vec3 heading()
    {
        glm::vec3 header;
        if(objectType == 1) // circle.
        {
            header = glm::vec4(verts[0], -1);
        }
        else if(objectType == 2) // line.
        {
            header = glm::vec4(verts[1], -1); // the translated header point.
        }
        else if(objectType == 3) // triangle.
        {
            header = glm::vec4(verts[2], -1); // the translated header point.
        }
        
        header = glm::normalize(header);
        return header;
    }
    
    glm::vec3 getPointInCanvasSpace(glm::vec3 p)
    {
        glm::mat4 sc = glm::scale(glm::mat4(1.0), scale);
        glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0,0,1));
        glm::vec3 p0 = pos + (sc * rot * glm::vec4(p, -1)); // the translated header point.
        return p0;
    }
    
    void loadImage()
    {
        image.load(imagePath);
    }
    
    void resetForces()
    {
        // zero out forces
        force = glm::vec3(0, 0, 0);
        angularForce = 0.0;
    }
    
    void setMovementToWindow()
    {
        moveWithinWindow = true;
        collision.load("sounds/Hit.wav");
        collision.setVolume(0.1);
    }
    
    // Physics data goes here  (for integrate())
    // to implement motion using forces
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    glm::vec3 acceleration = glm::vec3(0, 0, 0);
    glm::vec3 force = glm::vec3(0, 0, 0);
    float mass = 1.0;
    float damping = .99;
    float angularForce = 0;
    float angularVelocity = 0.0;
    float angularAcceleration = 0.0;
    float thrust = 100.0;
    float restitution;
    bool moveWithinWindow = false;
    
    glm::vec3 pos;
    float rotation = 0.0;    // degrees
    glm::vec3 scale = glm::vec3(1, 1, 1);
    float startTime;
    
    int last_x = -1;
    int last_y = -1;
    
    void accelerate(int direction){
        
        glm::vec3 rotationalHeader = getMatrix() * glm::vec4(heading(), -1);
        rotationalHeader = pos + rotationalHeader;
        rotationalHeader = thrust * direction * glm::normalize(rotationalHeader);
        force.x+=rotationalHeader.x;
        force.y+=rotationalHeader.y;
    }
    
    void turn(int direction){
        //  1 turn right
        // -1 turn left
        angularForce += thrust * (float) direction;
//        rotation+= (thrust * direction);
//        angularForce += thrust * (float) direction;
    }
    
    void integrate() {

        // init current framerate (or you can use ofGetLastFrameTime())
        //
        float framerate = ofGetFrameRate();
        float dt = 1.0 / framerate;

        // linear motion
        //
        
        // only increment position if inside screen boundary...
        if(moveWithinWindow) restrictMovementToWindow(velocity*dt);
        else pos+=velocity*dt;
        
        glm::vec3 accel = acceleration;
        accel += (force * 1.0 / mass);
        velocity += accel * dt;
        velocity *= damping;

        rotation += (angularVelocity * dt);

        if(isCollision())
        {
            color = ofColor::red;
            rotation -= (angularVelocity * dt);
            angularForce = 0.0;
            angularVelocity = 0.0;
        }

        float a = angularAcceleration;
        a += (angularForce * 1.0 / mass);
        angularVelocity += a * dt;
        angularVelocity *= damping;
        
        while(rotation >= 360) rotation-=360;
    }
    
    
    bool insideScreen(glm::vec3 p)
    {
        if(p.x <= ofGetWindowWidth() && p.x >= 0 && p.y <= ofGetWindowHeight()
           && p.y >= 0) return true;
        else
        {
            return false;
        }
    }

    bool isCollision()
    {
        glm::vec3 vertex;
        
        for(int vertexID = 0; vertexID < 3; vertexID++)
        {
            vertex =  (getMatrix() * glm::vec4(verts[vertexID], 1));
            
            if(vertex.x <= 0.0
               || vertex.x >= ofGetWindowWidth()
               || vertex.y <= 0.0
               || vertex.y >= ofGetWindowHeight()) return true;
        }
        
        return false;
    }

    float getBoundaryOffset_X(int vertexID, glm::vec3 delta)
    {
        float framerate = ofGetFrameRate();
        float dt = 1.0 / framerate;
        
        glm::vec3 nextFrameVertex = (delta) + (getMatrix() * glm::vec4(verts[vertexID], 1));
        
        glm::vec3 currentVertex = getMatrix() * glm::vec4(verts[vertexID], 1); // the header point in translated object space.

        if(nextFrameVertex.x < 0) return 0.0 - currentVertex.x;
        if(nextFrameVertex.x > ofGetWindowWidth()) return ofGetWindowWidth() - currentVertex.x;
        
        return 0.0;
    }

    float getBoundaryOffset_Y(int vertexID, glm::vec3 delta)
    {
        float framerate = ofGetFrameRate();
        float dt = 1.0 / framerate;
        
        glm::vec3 nextFrameVertex = (delta) + (getMatrix() * glm::vec4(verts[vertexID], 1));
        
        glm::vec3 currentVertex = getMatrix() * glm::vec4(verts[vertexID], 1); // the header point in translated object space.

        if(nextFrameVertex.y < 0) return 0.0 - currentVertex.y;
        if(nextFrameVertex.y > ofGetWindowHeight()) return ofGetWindowHeight() - currentVertex.y;
        
        return 0.0;
    }

    int getCollisionBoundary(glm::vec3 vertex)
    {
        if(vertex.y <= 0) return 1;
        if(vertex.x >= ofGetWindowWidth()) return 2;
        if(vertex.y >= ofGetWindowHeight()) return 3;
        if(vertex.x <= 0) return 4;
        
        return 0.0;
    }

    void restrictMovementToWindow(glm::vec3 delta)
    {
        bool collided = false;
        int boundary = -1;
        
        // calculate offset for how much it has overshot the screen boundaries and reset.
        
        glm::vec3 vertex_0 =  (delta) + (getMatrix() * glm::vec4(verts[0], 1));
        glm::vec3 vertex_1 =  (delta) + (getMatrix() * glm::vec4(verts[1], 1));
        glm::vec3 vertex_2 =  (delta) + (getMatrix() * glm::vec4(verts[2], 1));

        if(!insideScreen(vertex_0))
        {
            pos.x+=getBoundaryOffset_X(0, delta);
            pos.y+=getBoundaryOffset_Y(0, delta);
            boundary = getCollisionBoundary(vertex_0);
            collided = true;
        }
        
        if(!insideScreen(vertex_1))
        {
            pos.x+=getBoundaryOffset_X(1, delta);
            pos.y+=getBoundaryOffset_Y(1, delta);
            boundary = getCollisionBoundary(vertex_1);
            collided = true;
        }
        
        if(!insideScreen(vertex_2))
        {
            pos.x+=getBoundaryOffset_X(2, delta);
            pos.y+=getBoundaryOffset_Y(2, delta);
            boundary = getCollisionBoundary(vertex_2);
            collided = true;
        }
        
        if(collided)
        {
            collision.play();
            collisionResponse(boundary);
            return;
        }
        
        pos+=(delta);
    }

    void collisionResponse(int boundary)
    {
        float framerate = ofGetFrameRate();
        float dt = 1.0 / framerate;
        float e = restitution;
        
        glm::vec3 normalVector = glm::vec3(0, 0, 0);
        
        switch(boundary)
        {
            case 1: normalVector = glm::vec3(1, -1, 0);
                break;
            case 2: normalVector = glm::vec3(-1, 1, 0);
                break;
            case 3: normalVector = glm::vec3(1, -1, 0);
                break;
            case 4: normalVector = glm::vec3(-1, 1, 0);
                break;
        }
        
        force.x = normalVector.x * velocity.x;
        force.y = normalVector.y * velocity.y;
        
        force = (1+e) * thrust * force * dt;
        
         velocity = glm::vec3(0,0,0);
    }

    int objectType = 0; // designating shape where value = number of vertices (1, 2, 3, etc).
    glm::vec3 header;
    bool showHeader;
    bool showSprite;
    bool setFill;
    ofImage image;
    string imagePath;
    ofColor color;
    ofSoundPlayer collision;

    float spriteHeight;
    float spriteWidth;
    float lifespan;
    vector<glm::vec3> verts;
};
