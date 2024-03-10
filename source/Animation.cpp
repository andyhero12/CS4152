//
//  Animation.cpp
//  Ship
//
//  Created by Henry Gao on 2/29/24.
//

#include "Animation.h"

Animation::Animation(int numDirections, std::vector<std::shared_ptr<cugl::SpriteSheet>>& animTextures, float freq, int startFrame) :
    numAnimDirections(numDirections),
    animFreq(freq),
    animSprite(animTextures),
    animSpriteFrames(animTextures.size())
{
    std::fill(animSpriteFrames.begin(), animSpriteFrames.end(), 0);
    currentAnimationDirection = 0;
    currentAnimSprite = animSprite[currentAnimationDirection];
    std::fill(animSpriteFrames.begin(), animSpriteFrames.end(), startFrame);
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
    std::fill(animSpriteFrames.begin(), animSpriteFrames.end(), 0);
    currentAnimationDirection = aimingDir;
    currentAnimSprite = animSprite[currentAnimationDirection];
    frame = 0;
}


void Animation::updateAnimTime(){
    timeSinceLastAnim += (timeSinceLastAnim <= (animFreq) ? 1 : 0);
}


void Animation::stepAnimation(){
    animSpriteFrames[currentAnimationDirection] = (animSpriteFrames[currentAnimationDirection] + 1) % currentAnimSprite->getSize();
    frame = animSpriteFrames[currentAnimationDirection];
}

const std::shared_ptr<cugl::SpriteSheet>& Animation::getSprite() const {
    currentAnimSprite->setFrame(frame);
    return currentAnimSprite;
}

void Animation::setOrigin(cugl::Vec2 &origin){
    for(auto& spriteSheet : animSprite) {
        spriteSheet->setOrigin(origin);
    }
}
