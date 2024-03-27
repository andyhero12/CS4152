//
//  Animation.cpp
//
//  Created by Henry Gao on 2/29/24.
//

#include "Animation.h"

Animation::Animation( std::vector<std::shared_ptr<cugl::SpriteSheet>>& animTextures, float freq, int startFrame) :
    animFreq(freq),
    animSprite(animTextures)
{
    currentAnimationDirection = 0;
    currentAnimSprite = animSprite.at(currentAnimationDirection);
    frame = startFrame;
}


bool Animation::frameUpdateReady() {
    if(timeSinceLastAnim > (animFreq)){
        timeSinceLastAnim = 0;
        return true;
    }
    return false;
}

void Animation::resetAnimation(int aimingDir){
    currentAnimationDirection = aimingDir;
    currentAnimSprite = animSprite.at(aimingDir);
    frame = 0;
}


void Animation::updateAnimTime(){
    timeSinceLastAnim += (timeSinceLastAnim <= (animFreq) ? 1 : 0);
}


void Animation::stepAnimation(){
    frame = (frame + 1)% currentAnimSprite->getSize();
}

const std::shared_ptr<cugl::SpriteSheet>& Animation::getSprite() const {
    if(currentAnimSprite == nullptr){
        return currentAnimSprite;
    }
    currentAnimSprite->setFrame(frame);
    return currentAnimSprite;
}

void Animation::setOrigin(cugl::Vec2 &origin){
    for(auto& spriteSheet : animSprite) {
        spriteSheet->setOrigin(origin);
    }
}

int Animation::convertToQuadrant(double radian) {
    int directions = static_cast<int> (animSprite.size());
    double angleInDegrees = radian * (180 / M_PI);
    float ang = 360.0f / (directions);
    int quadrant = static_cast<int>(std::floor(angleInDegrees / ang)) % directions;
    return ( quadrant + directions ) % directions;
}


void Animation::updateDirection(){
    updateAnimTime();
    if (frameUpdateReady()){
        stepAnimation();
    }
}
/*
 Takes in the vector angle
 */
void Animation::update(double radian){
    int dir_quad = convertToQuadrant(radian);
    
    if (currentAnimationDirection != dir_quad){
        resetAnimation(dir_quad);
    }
    updateDirection();
}


/*
 Takes in the vector angle
 */
void Animation::update(int prevDir){
    if (currentAnimationDirection != prevDir){
        resetAnimation(prevDir);
    }
    updateDirection();
}
