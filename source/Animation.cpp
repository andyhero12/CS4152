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
    currentAnimSprite = animSprite[currentAnimationDirection];
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
    currentAnimSprite = animSprite[aimingDir];
    frame = 0;
}


void Animation::updateAnimTime(){
    timeSinceLastAnim += (timeSinceLastAnim <= (animFreq) ? 1 : 0);
}


void Animation::stepAnimation(){
    frame = (frame + 1)% currentAnimSprite->getSize();
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
