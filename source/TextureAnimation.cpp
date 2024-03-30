//
//  TextureAnimation.cpp
//  Heavan
//
//  Created by Henry Gao on 3/29/24.
//

#include "TextureAnimation.h"

TextureAnimation::TextureAnimation(std::vector<std::shared_ptr<cugl::Texture>>& animTextures, float freq):
  TextureAnimation(animTextures, freq, 0, 1, 0, 1)
{
}

TextureAnimation::TextureAnimation(std::vector<std::shared_ptr<cugl::Texture>>& animTextures, float freq, int x1, int x2, int y1, int y2) :
    TextureAnimation(animTextures, freq,
        static_cast<GLfloat>(x1) / animTextures.at(0)->getWidth(),
        static_cast<GLfloat>(x2) / animTextures.at(0)->getWidth(),
        static_cast<GLfloat>(y1) / animTextures.at(0)->getHeight(),
        static_cast<GLfloat>(y2) / animTextures.at(0)->getHeight())
{
}


TextureAnimation::TextureAnimation(std::vector<std::shared_ptr<cugl::Texture>>& animTextures, float freq, GLfloat minS, GLfloat maxS, GLfloat minT, GLfloat maxT) :
    animFreq(freq),
    allTexture(animTextures),
    minS(minS),
    maxS(maxS),
    minT(minT),
    maxT(maxT)
{
    currentAnimationDirection = 0;
    currentTexture = allTexture.at(currentAnimationDirection);
    idx = 0;
}

bool TextureAnimation::frameUpdateReady() {
    if(timeSinceLastAnim > (animFreq)){
        timeSinceLastAnim = 0;
        return true;
    }
    return false;
}


void TextureAnimation::updateAnimTime(){
    timeSinceLastAnim += (timeSinceLastAnim <= (animFreq) ? 1 : 0);
}


void TextureAnimation::stepAnimation(){
    idx = (idx + 1) % allTexture.size();
    currentTexture = allTexture.at(idx);
}

void TextureAnimation::updateTexture(){
    updateAnimTime();
    if (frameUpdateReady()){
        stepAnimation();
    }
}
