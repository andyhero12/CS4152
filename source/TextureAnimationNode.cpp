//
//  TextureAnimationNode.cpp
//  Heavan
//
//  Created by Henry Gao on 3/29/24.
//

#include "TextureAnimationNode.h"

//
bool TextureAnimationNode::initWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& textures, int x1, int y1, int x2, int y2, int freqAnimation){
    return initWithTextures(textures,  
                            static_cast<GLfloat>(x1) / textures.at(0)->getWidth(),
                            static_cast<GLfloat>(x2) / textures.at(0)->getWidth(),
                            static_cast<GLfloat>(y1) / textures.at(0)->getHeight(),
                            static_cast<GLfloat>(y2) / textures.at(0)->getHeight(), 
                            freqAnimation);
}


bool TextureAnimationNode::initWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& textures, GLfloat minS, GLfloat maxS, GLfloat minT, GLfloat maxT, int freqAnimation)
{
    _frame = 0;
    this->maxS = maxS;
    this->minS = minS;
    this->maxT = maxT;
    this->minT = minT;
    
    
    _animationTexture.resize(textures.size());
    for(size_t i = 0; i < textures.size(); i++){
        _animationTexture.at(i) = cugl::scene2::PolygonNode::allocWithTexture(textures.at(i)->getSubTexture(minS, maxS, minT, maxT));
    }
    return true;
}



void TextureAnimationNode::setAnchor(const cugl::Vec2 anchor){
    _anchor = anchor;
    for(auto &node : _animationTexture){
        node->setAnchor(anchor);
    }
}

void TextureAnimationNode::setPosition(const cugl::Vec2 &position){
    _position = position;
    for(std::shared_ptr<cugl::scene2::PolygonNode> &node : _animationTexture){
        node->setPosition(position);
    }
}

void TextureAnimationNode::setAngle(float angle) {
    _angle = angle;
    for(auto &node : _animationTexture){
        node->setAngle(_angle);
    }
}


bool TextureAnimationNode::frameUpdateReady() {
    if(_timeSinceLastAnim > (_animFreq)){
        _timeSinceLastAnim = 0;
        return true;
    }
    return false;
}


void TextureAnimationNode::updateAnimTime(){
    _timeSinceLastAnim += (_timeSinceLastAnim <= (_animFreq) ? 1 : 0);
}


void TextureAnimationNode::stepAnimation(){
    _frame = (_frame + 1) % _animationTexture.size();
}

void TextureAnimationNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, const cugl::Affine2& transform, cugl::Color4 tint){
    updateAnimTime();
    if (frameUpdateReady()){
        stepAnimation();
    }
    _animationTexture.at(_frame)->draw(batch, transform, tint);
}

void TextureAnimationNode::dispose(){
    for(auto &node : _animationTexture){
        node->dispose();
    }
}
