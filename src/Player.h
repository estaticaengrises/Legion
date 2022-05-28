//
//  Player.h
//  Project 2
//
//  Created by Ayan Abhiranya Singh on 4/8/22.
//

#ifndef Player_h
#define Player_h


#endif /* Player_h */

class TriangleShape : public BaseObject {
public:
    TriangleShape() {}
    TriangleShape(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
        verts.push_back(p1);
        verts.push_back(p2);
        verts.push_back(p3);
    }
    void draw();
    bool inside(glm::vec3 p);
};

class Player : public BaseObject {
public:
    Player() {}
    Player(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
        objectType = 3;
        verts.push_back(p1);
        verts.push_back(p2);
        verts.push_back(p3);
        showHeader = false;
        showSprite = false;
        setFill = true;
        thrust = 100;
        nEnergy = 3;
        imagePath = "images/ellie_tlou.png";
        image.load(imagePath); // switch this back on (uncomment) in final release

    }
    void draw();
    void update();
    void setup();
    void destroy();
//    bool inside(glm::vec3 p);
//    bool inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    
    int nEnergy;
};

class PolyLineShape : public BaseObject {

};
