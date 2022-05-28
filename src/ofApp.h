#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "BaseObject.h"
#include "Player.h"
#include "Agent.h"
#include "Ray.h"
#include "Particle.h"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void loadBackgroundImage();
        void update();
        void updateToggleParameters();
        void cycleSprites(bool custom);
        void advancedUpdate();
        void handleInput();
        void draw();
        void restart();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
    
        // player variables.
        Player pl;
        AgentEmitter emitter;
        RayEmitter rayEmitter;
        bool startGame = false;
        bool gameOver = false;
        bool canDrag = false;
        bool toggleRotate = false;
        bool showPanel = true;
        bool cycleImages = false;
        bool playMusic = false;
        float lastTime = 9999;
        int second = 0;
        int frame = 0;
        string timeString;
        int defeatedAgents;
        string imagePath;
        ofImage backgroundImage;
    
        map<int, bool> keyMap;  // map to handle player input

        ofTrueTypeFont    verdana14;
        ofTrueTypeFont    verdana14A;
        ofSoundPlayer bgm;
        ofSoundPlayer thrust_sfx;

        void displayStartGameMessage();
        void displayGameOverMessage();
        void displayGameStats();
    
        void guiSetup();
        void bgm_buttonPressed();
        void scaleChanged(float &scale);
        void speedChanged(float &speed);
        void energyLevelChanged(int &speed);
        void playBackgroundMusic();
        void playThrustSoundEffect();

        /*/ ------- BEGIN PARAMETER ADJUSTMENT TOOLS DEFINITION ------- */
    
        ofxFloatSlider thrustSlider;
        ofxFloatSlider scaleSlider;
        ofxIntSlider   player_energyLevels;
        ofxIntSlider   agent_spawnCount;
        ofxFloatSlider agent_lifespan;
        ofxFloatSlider agent_spawnRate;

        /*/ ------- END PARAMETER ADJUSTMENT TOOLS DEFINITION ------- */

        ofxColorSlider color;
        ofxVec2Slider center;
        ofxIntSlider circleResolution;
        ofxToggle filled;
        ofxToggle showSprite;
        ofxToggle showHeader;
        ofxToggle showTargetVector;
        ofxToggle showCustomSprites;
        ofxToggle godMode;
        ofxButton bgm_button;
        ofxLabel screenSize;

        ofxPanel gui;
    
    private:
    
        string getTimeString(float timeInSeconds)
        {
            string timeString = "";
            
            int hours = 0;
            int minutes = 0;
            
            if(timeInSeconds>=3600)
            {
                hours = timeInSeconds/60;
                timeString += to_string(hours);
                timeString += " h ";
                timeInSeconds = fmod(timeInSeconds, 60);
            }
            if(timeInSeconds>60)
            {
                minutes = timeInSeconds/60;
                timeString += to_string(minutes);
                timeString += " m ";
                timeInSeconds = fmod(timeInSeconds, 60);
            }
        
            std::stringstream secondStream;
            secondStream << std::fixed << std::setprecision(2) << timeInSeconds;
                        
            timeString += secondStream.str();
            timeString += " s";
            
            return timeString;
        }
        
};
