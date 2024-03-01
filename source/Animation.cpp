//
//  Animation.cpp
//  Ship
//
//  Created by Henry Gao on 2/29/24.
//

#include "Animation.h"

Animation::Animation(int numDirections, std::vector<std::shared_ptr<cugl::SpriteSheet>> animTextures, float freq): numAnimDirections(numDirections),
    animFreq(freq), 
    animSprite(animTextures),
    animSpriteFrames(animTextures.size())
{

    std::fill(animSpriteFrames.begin(), animSpriteFrames.end(), 0);
        prevAnimDir = 0;
        currentAnimSprite = animTextures[prevAnimDir];
        currentAnimSprite->setFrame(0);
}

Animation::Animation(int numDirections, std::vector<std::shared_ptr<cugl::SpriteSheet>> animTextures, float freq, int startFrame) :
    Animation(numDirections, animTextures, freq)
{
    std::fill(animSpriteFrames.begin(), animSpriteFrames.end(), startFrame);
    currentAnimSprite->setFrame(startFrame);
    
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
    currentAnimSprite->setFrame(0);
}

void Animation::updateAnimTime(float dt){
    timeSinceLastAnim += (timeSinceLastAnim <= (1.0/animFreq) ? dt : 0);
}
