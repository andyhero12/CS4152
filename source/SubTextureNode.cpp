//
//  SubTextureNode.cpp
//  Heaven
//
//  Created by Henry Gao on 4/3/24.
//
#include "SubTextureNode.h"
using namespace cugl::scene2;


bool SubTextureNode::initWithTexture(const std::shared_ptr<cugl::Texture>& texture){
    original = texture;
    PolygonNode::initWithTexture(original);
    return true;
}

void SubTextureNode::setSubtexture(GLfloat minS, GLfloat maxS, GLfloat minT, GLfloat maxT){
    
    // fix texture
    setTexture(original->getSubTexture(minS, maxS, minT, maxT));
    
    // fix polygon
    cugl::Size size = _texture == nullptr ? cugl::Size::ZERO : _texture->getSize();
    setPolygon(cugl::Rect(cugl::Vec2::ZERO,size));

}
