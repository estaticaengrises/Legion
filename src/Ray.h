//
//  Ray.h
//  Project 2
//
//  Created by Ayan Abhiranya Singh on 4/9/22.
//

#ifndef Ray_h
#define Ray_h


class Ray : public BaseObject {
public:
    Ray() {}
    Ray(glm::vec3 p1, glm::vec3 p2) {
        objectType = 2;
        verts.push_back(p1);
        verts.push_back(p2);
        thrust = 1.4;
        showHeader = false;
        showSprite = false;
        showTargetVector = false;
        lockedOn = false;
        startTime = ofGetElapsedTimeMillis();
        // target = glm::vec3(0, 50, 0);
        // imagePath = path_to_image;
        // image.load(imagePath); // switch this back on (uncomment) in final release
        spriteHeight = 70;
        spriteWidth = 70;
        thrust = 550;
        explode = false;
        //  damping = 0.95;
    }
    
    void draw();
    void update();
    void explosion();
    
    glm::vec3 target;
    bool lockedOn;
    bool explode;
    bool showTargetVector;
};

class RayEmitter : public Ray{
public:
    RayEmitter() {
        nRays = 100;
        lifespan = 3;
        rate = 15;
        emit = false;
    }
    
    void draw();
    void destroy();
    
    void startEmitter();
    void shootRay();
    void update();
    
    int nRays;
    float rate;
    float lifespan;
    bool emit;
    
    vector<Ray> rays;
    ofSoundPlayer ray_soundEffect;
};

#endif /* Ray_h */
