//
//  Animation.cpp
//  Ship
//
//  Created by Henry Gao on 2/29/24.
//

#include "Animation.h"

Animation::Animation(int numDirections, std::vector<std::shared_ptr<cugl::SpriteSheet>> animTextures, float freq, int startFrame) :
    numAnimDirections(numDirections),
    animFreq(freq),
    animSprite(animTextures),
    animSpriteFrames(animTextures.size())
//    Animation(numDirections, animTextures, freq)
{
    std::fill(animSpriteFrames.begin(), animSpriteFrames.end(), 0);
    prevAnimDir = 0;
    currentAnimSprite = animSprite[prevAnimDir];
    std::fill(animSpriteFrames.begin(), animSpriteFrames.end(), startFrame);
    frame = startFrame;
//    currentAnimSprite->setFrame(startFrame);
    
}

/** Returns true if enough time has passed since the last animation frame update     */
bool Animation::frameUpdateReady() {
    if(timeSinceLastAnim > (1.0/animFreq)){
        timeSinceLastAnim = 0;
        return true;
    }
    return false;
}

void Animation::resetAnimation(int aimingDir){
    std::fill(animSpriteFrames.begin(), animSpriteFrames.end(), 0);
    prevAnimDir = aimingDir;
    currentAnimSprite = animSprite[prevAnimDir];
    frame = 0;
}

void Animation::updateAnimTime(float dt){
    timeSinceLastAnim += (timeSinceLastAnim <= (1.0/animFreq) ? dt : 0);
}


void Animation::stepAnimation(){
    animSpriteFrames[prevAnimDir] = (animSpriteFrames[prevAnimDir] + 1) % currentAnimSprite->getSize();
    frame = animSpriteFrames[prevAnimDir];
}

const std::shared_ptr<cugl::SpriteSheet>& Animation::getSprite() const {
    currentAnimSprite->setFrame(frame);
    return currentAnimSprite;
}
