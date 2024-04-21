//
//  AttackPolygons.cpp
//
//  Created by Andrew Cheng on 3/12/24.
//

#include "AttackPolygons.h"
#define BITE_AGE 2

#include "NLDog.h"
// used to translate attacks to originate from the dog's head
#define BITE_HEAD_OFFSET_RATIO 0.8f
#define SHOOT_HEAD_OFFSET_RATIO 1.5f
#define ANIM_FREQ 1
#define BITE_SCALE 1
#define BITE_FRAME 5
#define OFFSET_SCALE 1/0.0234375f


bool ActionPolygon::dealDamage(){
    if(!_polygon && polygonAction == Action::BITE){
        return spriteActionNode->getFrame() == BITE_FRAME;
    }
    else{
        return true;
    }
}

void ActionPolygon::update(){
    if(!_polygon){
        _expired = (spriteActionNode->getFrame() + 1) == spriteActionNode->getSpan();
        if(_expired){
            return;
        }
        spriteActionNode->update();
    }
    else{
        _age++;
        _expired = _age >= _maxage;
    }
    // update animation when needed
}


std::shared_ptr<cugl::scene2::SceneNode> ActionPolygon::getActionNode(){
    if(_polygon){
        return polyActionNode;
    }
    else{
        return spriteActionNode;
    }
}


ActionPolygon::ActionPolygon( std::shared_ptr<SpriteAnimationNode> actionSprite, Action curAction, Poly2& mintPoly, int mx, float scale, float ang, Vec2 center)
: ActionPolygon(curAction, mintPoly, mx, scale, ang, center)
{
    _expired = false;
    _polygon = false;
    spriteActionNode = actionSprite;
    _scale = scale * BITE_SCALE;
    spriteActionNode->setScale(_scale);
}

ActionPolygon::ActionPolygon(Action curAction, Poly2& mintPoly, int mx, float scale, float ang, Vec2 center)
: polygonAction{curAction}
, internalPolygon{mintPoly}
, _age{0}
, _maxage{mx}
, _scale{scale}
, _polygon(true)
, _freq{0}
, _ang{ang}
, _center(center)
{
    polyActionNode = cugl::scene2::SpriteNode::allocWithPoly(mintPoly);
}


AttackPolygons::AttackPolygons()
: max_age{25}
{
    
}

bool AttackPolygons::init(){
    currentAttacks.clear();
    backAttackPolygonNode = cugl::scene2::SceneNode::alloc();
    frontAttackPolygonNode = cugl::scene2::SceneNode::alloc();
    return true;
}
void AttackPolygons::update(){
    auto it =currentAttacks.begin();
    while (it != currentAttacks.end()){
        std::shared_ptr<ActionPolygon> curAction = *it;
        curAction->update();
        auto curIt = it++;
        if (curAction->expired()){
            curAction->getActionNode()->removeFromParent();
            currentAttacks.erase(curIt);
        }
    }
}

void AttackPolygons::addShoot(Vec2 center, float angle, float shootRadius){
    float degree = 60;
    PolyFactory curFactory;
    Poly2 resultingPolygon_shoot = curFactory.makeArc(center, shootRadius, angle + degree, degree);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::SHOOT, resultingPolygon_shoot, max_age, shootRadius, angle, center);
    backAttackPolygonNode->addChild(curPtr->getActionNode());
    Vec2 offset = Vec2(SDL_cosf((angle + 90) * 3.14f / 180), SDL_sinf((angle + 90) * 3.14f / 180)) * DOG_SIZE.x * SHOOT_HEAD_OFFSET_RATIO;
    curPtr->getActionNode()->setScale(OFFSET_SCALE);
    curPtr->getActionNode()->setPosition(offset * OFFSET_SCALE);
    currentAttacks.insert(curPtr);
}

void AttackPolygons::addExplode(Vec2 center, float explosionRad){
    PolyFactory curFactory;
    Poly2 resultingPolygon = curFactory.makeCircle(center, explosionRad);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::EXPLODE, resultingPolygon, max_age, explosionRad, 0, center);
    backAttackPolygonNode->addChild(curPtr->getActionNode());
    curPtr->getActionNode()->setScale(OFFSET_SCALE);
    currentAttacks.insert(curPtr);
}

void AttackPolygons::addBite(Vec2 center, float angle, float explosionRad, float scale){
    std::shared_ptr<cugl::Texture> bite;
    
    bool front = true;
    float ang = angle;
    if (angle >= 45 && angle <= 135) {
         bite = biteLeftTexture;
         ang -= 90;
     } else if (angle > 135 && angle < 225) {
         bite = biteFrontTexture;
         ang -= 180;
     } else if (angle >= 225 && angle <= 315) {
         bite = biteRightTexture;
         ang -= 270;
     } else{
         bite = biteBackTexture;
         front = false;
     }
    
    std::shared_ptr<SpriteAnimationNode> biteSprite = SpriteAnimationNode::allocWithSheet(bite, 3, 5, 15, ANIM_FREQ);
    //biteSprite->setAngle(ang);
    PolyFactory curFactory;
    Poly2 resultingPolygon = curFactory.makeArc(center, explosionRad * (1 + scale), angle, 180);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(biteSprite, Action::BITE, resultingPolygon, BITE_AGE, 1 + scale, angle, center);
    if(front){
        frontAttackPolygonNode->addChild(curPtr->getActionNode());
    }
    else{
        backAttackPolygonNode->addChild(curPtr->getActionNode());
    }
    Vec2 offset = Vec2(SDL_cosf((angle + 90) * 3.14f / 180), SDL_sinf((angle + 90) * 3.14f / 180)) * DOG_SIZE.x * BITE_HEAD_OFFSET_RATIO;
    curPtr->getActionNode()->setPosition(offset * OFFSET_SCALE);
    currentAttacks.insert(curPtr);
}

bool AttackPolygons::setTexture(const std::shared_ptr<cugl::Texture> &biteL, const std::shared_ptr<cugl::Texture> &biteR, const std::shared_ptr<cugl::Texture> &biteF, const std::shared_ptr<cugl::Texture> &biteB){
    biteRightTexture = biteR;
    biteLeftTexture = biteL;
    biteFrontTexture = biteF;
    biteBackTexture = biteF;
    return true;
}
