#include "ofApp.h"

void ofApp::guiSetup()
{
    scaleSlider.addListener(this, &ofApp::scaleChanged);
    thrustSlider.addListener(this, &ofApp::speedChanged);
    player_energyLevels.addListener(this, &ofApp::energyLevelChanged);
    bgm_button.addListener(this,&ofApp::bgm_buttonPressed);
    
    gui.setup(); // most of the time you don't need a name
    gui.add(bgm_button.setup("play/pause music"));
    gui.add(filled.setup("fill", true));
    gui.add(thrustSlider.setup("thrust slider", 500, 1, 1000));
    gui.add(scaleSlider.setup("scale slider", 1, 1, 10));
    gui.add(showSprite.setup("show sprite", false)); 
    gui.add(showCustomSprites.setup("custom sprites", false));
    gui.add(showHeader.setup("show header", false));
    gui.add(showTargetVector.setup("show targeting vector", false));
    gui.add(godMode.setup("god mode", false));  // set to true if the player should never be damaged
    gui.add(player_energyLevels.setup("player energy", 3, 1, 10));
    gui.add(agent_spawnCount.setup("agents spawned", 1, 1, 20));
    gui.add(agent_lifespan.setup("agent lifespan", 7, 1, 60));
    gui.add(agent_spawnRate.setup("agent spawn rate", 10, 0.5, 60));
    gui.add(color.setup("color", ofColor(140, 140, 140), ofColor(0, 0), ofColor(255, 255)));
        
    pl.color = ofColor(140, 140, 140);
}

void ofApp::bgm_buttonPressed(){
    // cout << "clicked" << endl;
    playMusic = !playMusic;
    if(!playMusic) bgm.stop();
}


void ofApp::scaleChanged(float &scale){
    pl.scale = glm::vec3(scale, scale, 1);
}

void ofApp::speedChanged(float &thrust){
    
    float frameRate = ofGetFrameRate();
    
//    pl.thrust = thrust/frameRate;
}

void ofApp::energyLevelChanged(int &scale){
    pl.nEnergy = scale;
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofTrueTypeFont::setGlobalDpi(72);

    verdana14.load("fonts/verdana.ttf", 14, true, true);
    verdana14.setLineHeight(18.0f);
    verdana14.setLetterSpacing(1.037);
    
    verdana14A.load("fonts/verdana.ttf", 14, false);
    verdana14A.setLineHeight(18.0f);
    verdana14A.setLetterSpacing(1.037);
    
    bgm.setVolume(0.7);
    bgm.load("sounds/ParagonX9 - Chaos Fantasy.mp3");
    // bgm.load("sounds/FantasyWav.wav");
//     bgm.load("sounds/Midnight.mp3");
    
    thrust_sfx.setVolume(0.5);
    // bgm.load("sounds/ParagonX9 - Chaos Fantasy.mp3");
    // thrust.load("sounds/i4_accel_hz.mp3");
//    thrust.load("sounds/i4_accel2_hz.mp3"); // i4_accel2_hz
    thrust_sfx.load("sounds/thrust.mp3"); // i4_accel2_hz

    keyMap.insert(pair<int, bool>(57357, false));  // up
    keyMap.insert(pair<int, bool>(57359, false));  // down
    keyMap.insert(pair<int, bool>(57356, false));  // left
    keyMap.insert(pair<int, bool>(57358, false));  // right
    
    defeatedAgents = 0;
    imagePath = "images/citybg2.png";
//    pl = Player(glm::vec3(-50, 50, 0), glm::vec3(50, 50, 0), glm::vec3(0, -50, 0));
    pl = Player(glm::vec3(-20, 20, 0), glm::vec3(20, 20, 0), glm::vec3(0, -40, 0));
    emitter = AgentEmitter();
    rayEmitter = RayEmitter();
    pl.setup();
    guiSetup();
}

void ofApp::loadBackgroundImage(){
    backgroundImage.load(imagePath);
    backgroundImage.resize(ofGetWindowWidth(), ofGetWindowHeight()); // resizing the image
    backgroundImage.draw(0,0); // draw the image centered at origin
}


//--------------------------------------------------------------
void ofApp::update(){
    
    if(gameOver || !startGame)
    {
        return;
    }
    
    if(playMusic) playBackgroundMusic();
    handleInput();
    playThrustSoundEffect();
    
    pl.update();
    
    rayEmitter.pos = pl.pos;
    rayEmitter.pos = pl.getPointInCanvasSpace(pl.verts[2]);
    
    rayEmitter.rotation = pl.rotation;

    emitter.updateParameters(agent_spawnCount, agent_lifespan, agent_spawnRate, showTargetVector);
    emitter.update(pl.pos);
    rayEmitter.update();
    updateToggleParameters();
    advancedUpdate();
}

void ofApp::updateToggleParameters()
{
    if(toggleRotate)
    {
        pl.rotation += 2;
    }
    
    pl.showHeader = showHeader;
    
    pl.setFill = filled;
    pl.color = color;
    emitter.color = color;
    
    if(cycleImages != showCustomSprites)
    {
        cycleSprites(showCustomSprites);
        cycleImages = showCustomSprites;
    }
    
    pl.showSprite = showSprite;
    
    emitter.targetImage = pl.image;
    emitter.setAgentShowSprite = showSprite;
}

void ofApp::cycleSprites(bool custom)
{
    if(custom)
    {
        pl.imagePath = "images/birdSprite.png";
        // emitter.setImagePath("images/snakeSprite.png");
        emitter.imagePath = "images/snakeSprite.png";
        imagePath = "images/backgroundImageMountains.png";
    }
    else
    {
        pl.imagePath = "images/ellie_tlou.png";
        // emitter.setImagePath("images/prey-telepath.gif");
        emitter.imagePath = "images/prey-telepath.gif";
        imagePath = "images/citybg2.png";
    }
    
    pl.loadImage();
    backgroundImage.load(imagePath);
}

void ofApp::advancedUpdate()
{
    int numCollisions = 0;
    
    if(showSprite)
    {
        glm::vec3 p0 = pl.pos + glm::vec3(-pl.spriteWidth/2.0, -pl.spriteHeight/2.0, 0);
        glm::vec3 p1 = pl.pos + glm::vec3(pl.spriteWidth/2.0, -pl.spriteHeight/2.0, 0);
        glm::vec3 p2 = pl.pos + glm::vec3(-pl.spriteWidth/2.0, pl.spriteHeight/2.0, 0);
        glm::vec3 p3 = pl.pos + glm::vec3(pl.spriteWidth/2.0, pl.spriteHeight/2.0, 0);
        numCollisions = emitter.checkCollisions(p0, p1, p2, p3, rayEmitter.rays);
    }
    else
    {
        glm::vec3 p0 = pl.getPointInCanvasSpace(pl.verts[0]);
        glm::vec3 p1 = pl.getPointInCanvasSpace(pl.verts[1]); // the translated header point.
        glm::vec3 p2 = pl.getPointInCanvasSpace(pl.verts[2]); // the translated header point.
        
        numCollisions = emitter.checkCollisions(p0, p1, p2, pl.pos, rayEmitter.rays);
    }
    
    if(!godMode)
    {
        if(pl.nEnergy - numCollisions <= 0) pl.nEnergy = 0;
        else pl.nEnergy -= numCollisions;
        
        if(pl.nEnergy + emitter.rayCollisions >= player_energyLevels) pl.nEnergy = player_energyLevels;
        else pl.nEnergy += emitter.rayCollisions;
    }
    
    if(numCollisions > 0) pl.collision.play();
    
    if(pl.nEnergy == 0) gameOver = true;
}

void ofApp::handleInput()
{
    if(true)
    {
        if(keyMap[57357]) pl.accelerate(1);  // up arrow
        if(keyMap[57359]) pl.accelerate(-1); // down arrow
        if(keyMap[57358]) pl.turn(1); // +=speed; // right arrow
        if(keyMap[57356]) pl.turn(-1); // -=speed; // left arrow
        if(keyMap[122]) rayEmitter.shootRay(); // shoot the ray if the z key is pressed.
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(gameOver)
    {
        displayGameOverMessage();
        return;
    }
    
    if(! startGame)
    {
        displayStartGameMessage();
        return;
    }
    
    loadBackgroundImage();
    pl.draw();
    emitter.draw();
    rayEmitter.draw();
    if(showPanel) gui.draw();
    displayGameStats();
}

void ofApp::restart(){
    
    emitter.destroy();
    pl.destroy();
    rayEmitter.destroy();
    
    toggleRotate = false;
    cycleImages = false;
    startGame = false;
    gameOver = false;
    playMusic = !playMusic;
    
    setup(); // setup the game again.
}

void ofApp::playBackgroundMusic()
{
    if(startGame)
    {
        if(!bgm.isPlaying())
        {
            bgm.play(); // play the background music uncomment in final release
        }
    }
}

void ofApp::playThrustSoundEffect()
{
    if(startGame && !gameOver)
    {
        float adjustedVolume;
        float magnitude = glm::length(pl.velocity)/ 1000.0;
//        cout << "pl.force: " << pl.force << endl;

        if(magnitude <= 0.0) adjustedVolume = 0.0;
        else
        {
            adjustedVolume = magnitude;
            if(adjustedVolume > 0.6) adjustedVolume = 0.6;
            // cout << "Thrust volume: " << adjustedVolume << endl;
//            cout << "Magnitude volume: " << magnitude << endl;
        }
        thrust_sfx.setVolume(adjustedVolume);
        if(!thrust_sfx.isPlaying())
        {
            thrust_sfx.play(); // play the background music uncomment in final release
        }
    }
}

void ofApp::displayStartGameMessage()
{
    ofPushMatrix();
        string title = "DYNAMIC PURSUIT";
        ofSetColor(245, 58, 135);
        ofTranslate(ofGetWindowWidth()/2.0 - 75, ofGetWindowHeight()/2.0, 0);
        ofScale(3.0, 3.0, 1.0);
        verdana14.drawString(title, - 40, - 40);
    ofPopMatrix();
    
    ofPushMatrix();
        ofSetColor(225);
        string startGameMessage = "PRESS SPACE TO BEGIN!";
        ofRectangle bounds = verdana14A.getStringBoundingBox(startGameMessage, 0, 0);

        ofTranslate(ofGetWindowWidth()/2.0 + bounds.width/2 - 100, ofGetWindowHeight()/2.0 + bounds.height / 2, 0);
        ofScale(2.0 + cos(ofGetElapsedTimef()), 2.0 + cos(ofGetElapsedTimef()), 1.0);

        verdana14A.drawString(startGameMessage, -bounds.width/2, bounds.height/2 );
    ofPopMatrix();
}

void ofApp::displayGameOverMessage()
{
    string gameOverMessage = "GAME OVER";
    string gameOverSubtitle = "You survived for " + timeString + ".";
    string agentsDefeatedSubtitle = "You defeated " + std::to_string(defeatedAgents) + " agents.";
    string restartGameSubtitle = "Press SPACE to restart";
    
    ofPushMatrix();
        ofSetColor(245, 58, 135);
        ofRectangle bounds = verdana14A.getStringBoundingBox(gameOverMessage, 0, 0);

        ofTranslate(ofGetWindowWidth()/2.0 + bounds.width/2, ofGetWindowHeight()/2.0 + bounds.height / 2, 0);
        ofScale(3.0, 3.0, 1.0);

        verdana14A.drawString(gameOverMessage, -bounds.width/2, bounds.height/2 );
        
    ofPopMatrix();
    ofSetColor(225);
    verdana14.drawString(gameOverSubtitle, ofGetWindowWidth()/2.0 + bounds.width/2 - 100, ofGetWindowHeight()/2.0 + bounds.height / 2 + 40);
    verdana14.drawString(agentsDefeatedSubtitle, ofGetWindowWidth()/2.0 + bounds.width/2 - 100, ofGetWindowHeight()/2.0 + bounds.height / 2 + 60);
    verdana14.drawString(restartGameSubtitle, ofGetWindowWidth()/2.0 + bounds.width/2 - 100, ofGetWindowHeight()/2.0 + bounds.height / 2 + 80);
}

void ofApp::displayGameStats(){
    
    string currentPlayerEnergy = "Player energy: ";
    string energyString = std::to_string(pl.nEnergy);
    energyString = energyString + "/";
    energyString = energyString + std::to_string(player_energyLevels);
    
    string elapsedTime = "Play time: ";
    timeString = getTimeString(pl.getElapsedTimeInSeconds());
    
    string currentFrameRate = "Frame rate: ";
    
    string agentsDefeated = "Agents defeated: ";
    defeatedAgents += emitter.rayCollisions;
    
    float margin_height_offset = 35;
    float margin_width_offset = 250;
    
    ofSetColor(225);
    verdana14.drawString(currentPlayerEnergy, ofGetWindowWidth() - margin_width_offset, margin_height_offset);
    verdana14.drawString(elapsedTime, ofGetWindowWidth() - margin_width_offset, margin_height_offset + 20);
    verdana14.drawString(currentFrameRate, ofGetWindowWidth() - margin_width_offset, margin_height_offset + 40);
    verdana14.drawString(agentsDefeated, ofGetWindowWidth() - margin_width_offset, margin_height_offset + 60);

    ofSetColor(255, 0, 0);
    verdana14.drawString(energyString, ofGetWindowWidth() - margin_width_offset + 115, margin_height_offset);
    verdana14.drawString(timeString, ofGetWindowWidth() - margin_width_offset + 115, margin_height_offset + 20);
    verdana14.drawString(std::to_string(ofGetFrameRate()), ofGetWindowWidth() - margin_width_offset + 115, margin_height_offset + 40);
    verdana14.drawString(std::to_string(defeatedAgents), ofGetWindowWidth() - margin_width_offset + 135, margin_height_offset + 60);
    
    ofSetColor(225);
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 32) // space bar
    {
        if(gameOver)
        {
            restart();
            startGame = true;
            loadBackgroundImage();
            pl.startTime = ofGetElapsedTimeMillis();
            emitter.startEmitter();
            rayEmitter.startEmitter();
        }
        
        if(!startGame)
        {
            startGame = true;
            loadBackgroundImage();
            pl.startTime = ofGetElapsedTimeMillis();
            emitter.startEmitter();
            rayEmitter.startEmitter();
        }
    }
    
    if(key == 112)  // p
    {
        showPanel = !showPanel;
    }
    
    if(key == 114) // r
    {
        toggleRotate = !toggleRotate;
    }
    
    if(key == 57357) // up arrow
    {
        keyMap[57357] = true;
    }
    else if (key == 57359) // down arrow
    {
        keyMap[57359] = true;
    }
    else if (key == 122) // z key to shoot rays
    {
        keyMap[122] = true;
    }
    
    if(!toggleRotate)
    {
        if(key == 57358) // right arrow
        {
            keyMap[57358] = true;
        }
        else if (key == 57356) // left arrow
        {
            keyMap[57356] = true;
        }
    }
    
    if(key == 61)
    {
        pl.scale = glm::vec3(pl.scale.x+.1, pl.scale.y+.1, 1);
    }
    else if (key == 45)
    {
        pl.scale = glm::vec3(pl.scale.x-.1, pl.scale.y-.1, 1);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if(key == 57356) // left arrow
    {
        keyMap[57356] = false;
    }
    else
    if(key == 57357) // up arrow
    {
        keyMap[57357] = false;
    }
    else
    if(key == 57358) // right arrow
    {
        keyMap[57358] = false;
    }
    else
    if(key == 57359) // down arrow
    {
        keyMap[57359] = false;
    }
    else
    if(key == 122) // down arrow
    {
        keyMap[122] = false;
    }

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(canDrag)
    {
        if(x!=pl.last_x || y!=pl.last_y)
        {
            pl.pos += glm::vec3(x-pl.last_x, y-pl.last_y, 0);
            
            if(pl.pos.x < 0) pl.pos.x = 1;
            if(pl.pos.x > ofGetWindowWidth()) pl.pos.x = ofGetWindowWidth() - 1;
            if(pl.pos.y < 0) pl.pos.y = 1;
            if(pl.pos.y > ofGetWindowHeight()) pl.pos.y = ofGetWindowHeight() - 1;
            
            pl.last_x = x;
            pl.last_y = y;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    cout << "clickdd: " << glm::vec3(x,y,0) << endl;
    
    if(pl.inside(glm::vec3(x,y,0)))
    {
        ofSetColor(ofColor::white);
        ofDrawLine(pl.pos, pl.header);
        pl.last_x = x;
        pl.last_y = y;
        canDrag = true;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    canDrag = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
