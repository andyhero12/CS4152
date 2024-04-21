//
//  SpriteAnimationNode.cpp
//  Heaven
//
//  Created by Henry Gao on 4/17/24.
//

#include "SpriteAnimationNode.h"


bool SpriteAnimationNode::initWithSheet(const std::shared_ptr<cugl::Texture>& texture, int rows, int cols, int size, int freq)
{
    cugl::scene2::SpriteNode::initWithSheet(texture, rows, cols, size);
    _animFreq = freq;
    _timeSinceLastAnim = 0;
    return true;
}


bool SpriteAnimationNode::frameUpdateReady() {
    if(_timeSinceLastAnim > (_animFreq)){
        _timeSinceLastAnim = 0;
        return true;
    }
    return false;
}

void SpriteAnimationNode::updateAnimTime(){
    _timeSinceLastAnim += (_timeSinceLastAnim <= (_animFreq) ? 1 : 0);
}

void SpriteAnimationNode::stepAnimation(){
    _frame = (_frame + 1)% _size;
    setFrame(_frame);
}

void SpriteAnimationNode::update(){
    updateAnimTime();
    if (frameUpdateReady()){
        stepAnimation();
    }
}
