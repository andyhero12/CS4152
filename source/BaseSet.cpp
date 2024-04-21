//
//  BaseSet.cpp
//
//  Created by Andrew Cheng on 2/26/24.
//
#include "BaseSet.h"

BaseSet::BaseSet()
{
}
BaseSet::~BaseSet()
{
}

void BaseSet::update()
{
    for (auto &base : _bases)
    {
        base->update();
//        base->getSceneNode()->update();
    }
    for (auto& animation: animationNodes){
        animation->update();
    }
}
int BaseSet::getFirstHealth()
{
    return (*_bases.begin())->getHealth();
}
bool BaseSet::init(const std::vector<cugl::Vec3>& basePoses, std::shared_ptr<cugl::AssetManager> assets)
{
    _bases.clear();
    animationNodes.clear();
    baseSetNode = cugl::scene2::SceneNode::alloc();
    for (const cugl::Vec3& base : basePoses){
        auto baseObj = std::make_shared<Base>(base.z, cugl::Vec2(base.x,base.y));
        _bases.emplace_back(baseObj);
        auto drawNode = SpriteAnimationNode::allocWithSheet(assets->get<cugl::Texture>("gate"), 3,5, 5);
//        drawNode->setContentSize(cugl::Size(4,4));
        drawNode->setScale(cugl::Size(1,1)/48);
        drawNode->setPosition(baseObj->getPos());
        drawNode->setAnchor(cugl::Vec2::ANCHOR_CENTER);
        baseSetNode->addChild(drawNode);
        animationNodes.push_back(drawNode);
    }
    return true;
}
bool BaseSet::baseLost()
{
    for (auto &base : _bases)
    {
        if (base->lost())
        {
            return true;
        }
    }
    return false;
}
