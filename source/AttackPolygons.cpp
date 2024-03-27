//
//  AttackPolygons.cpp
//
//  Created by Andrew Cheng on 3/12/24.
//

#include "AttackPolygons.h"
#define BITE_AGE 2
void ActionPolygon::update(cugl::Size size){
    _age++;
    // update animation when needed
}

ActionPolygon::ActionPolygon(Action curAction, Poly2& mintPoly, int mx)
: polygonAction{curAction}
, internalPolygon{mintPoly}
, _age{0}
, _maxage{mx}
, _scale{1.0}
{
    
}
void ActionPolygon::drawShoot(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    batch->setTexture(nullptr);
    batch->setColor(Color4::RED);
    batch->fill(internalPolygon);
}
void ActionPolygon::drawExplode(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    batch->setTexture(nullptr);
    batch->setColor(Color4::BLUE);
    batch->fill(internalPolygon);
}
void ActionPolygon::drawBite(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    batch->setTexture(nullptr);
    batch->setColor(Color4::GREEN);
    batch->fill(internalPolygon);
}
void ActionPolygon::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    switch (polygonAction){
        case Action::BITE:
            drawBite(batch,size);
            break;
        case Action::SHOOT:
            drawShoot(batch,size);
            break;
        case Action::EXPLODE:
            drawExplode(batch,size);
            break;
        default:
            CULog("action not used %d\n", polygonAction);
            break;
    };
}


AttackPolygons::AttackPolygons()
: max_age{25}
{
    
}

bool AttackPolygons::init(){
    currentAttacks.clear();
    return true;
}
void AttackPolygons::update(cugl::Size size){
    auto it =currentAttacks.begin();
    while (it != currentAttacks.end()){
        std::shared_ptr<ActionPolygon> curAction = *it;
        curAction->update(size);
        auto curIt = it++;
        if (curAction->expired()){
            currentAttacks.erase(curIt);
        }
    }
}

void AttackPolygons::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    for (const std::shared_ptr<ActionPolygon>& action : currentAttacks){
        action->draw(batch, size);
    }
}

void AttackPolygons::addShoot(const std::shared_ptr<Dog>& ship){
    Vec2 center = ship->getPosition();
    float degree = 60;
    PolyFactory curFactory;
    Poly2 resultingPolygon_shoot = curFactory.makeArc(center, ship->getShootRadius(), ship->getAngle() + degree, degree);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::SHOOT, resultingPolygon_shoot, max_age);
    currentAttacks.insert(curPtr);
}
void AttackPolygons::addExplode(const std::shared_ptr<Dog>& ship){
    Vec2 center = ship->getPosition();
    PolyFactory curFactory;
    Poly2 resultingPolygon = curFactory.makeCircle(center, ship->getExplosionRadius());
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::EXPLODE, resultingPolygon,max_age);
    currentAttacks.insert(curPtr);
}

void AttackPolygons::addBite(const std::shared_ptr<Dog>& ship){
    Vec2 center = ship->getPosition();
    PolyFactory curFactory;
    Poly2 resultingPolygon = curFactory.makeArc(center, ship->getBiteRadius(),ship->getAngle(), 180);
    std::shared_ptr<ActionPolygon> curPtr = std::make_shared<ActionPolygon>(Action::BITE, resultingPolygon,BITE_AGE);
    currentAttacks.insert(curPtr);
}
