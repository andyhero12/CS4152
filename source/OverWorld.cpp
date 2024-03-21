//
//  OverWorld.cpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#include "OverWorld.hpp"

#define WORLD_SIZE 3

void OverWorld::reset(std::shared_ptr<cugl::JsonValue> _constants,
                      cugl::Size resetSize) {
    _dog->setPosition(resetSize/2);
    _dog->setAbsorbValue(0);
    _dog->setAngle(0);
    _dog->setVelocity(cugl::Vec2::ZERO);
    _dog->setHealth(_constants->get("ship")->getInt("health",0));
//    _devil->setPosition(getSize()/2);
//    _asteroids.init(_constants->get("asteroids"),_ship);
//    _spawnerController.init(_constants->get("spawner"));
//    _bases.init(_constants->get("base"));
//    _attackPolygonSet.init();
}

void OverWorld::dogUpdate(InputController& _input, cugl::Size totalSize){
    if(_input.didChangeMode() && _dog->canChangeMode()){
        _dog->toggleMode();
        _dog->reloadMode();
    }
//    if (_input.didPressFire() && _ship->canFireWeapon()){
//        _ship->setAttack();
//        _attackPolygonSet.addBite(_ship);
//        AudioEngine::get()->play("laser", _laser, false, _laser->getVolume(), true);
//    }
//    if (_input.didPressSpecial() && _ship->canFireWeapon()){
////        _ship->reloadWeapon();
//        if (_ship->getMode() == "SHOOT" && _ship->getAbsorb() > 5){
//            _ship->subAbsorb(5);
//            _attackPolygonSet.addShoot(_ship);
//        }else if (_ship->getMode() == "BUILD" && _ship->getAbsorb() > 5 ){
//            _ship->subAbsorb(5);
//            _asteroids.createDecoy();
//        }else if (_ship->getMode() == "EXPLODE" && _ship->getAbsorb() > 10){
//            _ship->subAbsorb(10);
//            _attackPolygonSet.addExplode(_ship);
//        }else {
//            CULog("NOTHING\n");
//        }
//    }
    
    _dog->move( _input.getForward(),  _input.getTurn(), totalSize * WORLD_SIZE);
}
void OverWorld::update(InputController& _input, cugl::Size totalSize){
    dogUpdate(_input,totalSize);
}
