//
//  OverWorld.cpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#include "OverWorld.hpp"

#define WORLD_SIZE 3

void OverWorld::reset(cugl::Size resetSize) {
    _dog->setPosition(resetSize/2);
    _dog->setAbsorbValue(0);
    _dog->setAngle(0);
    _dog->setVelocity(cugl::Vec2::ZERO);
    _dog->setHealth(_constants->get("ship")->getInt("health",0));
    _devil->setPosition(resetSize/2);
    
//    _asteroids.init(_constants->get("asteroids"),_ship);
//    _spawnerController.init(_constants->get("spawner"));
//    _bases.init(_constants->get("base"));
//    _attackPolygonSet.init();
}

bool OverWorld::init(const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size totalSize){
    _assets = assets;
    // Get the background image and constant values
    _constants = assets->get<cugl::JsonValue>("constants");
    _dog = std::make_shared<Dog>(totalSize/2, _constants->get("ship"));
    _devil = std::make_shared<Devil>(_dog, totalSize/2, _constants->get("ship"));
    _totalSize = totalSize;
    std::vector<std::shared_ptr<cugl::Texture>> textures;
    textures.push_back(assets->get<cugl::Texture>("shipleftidle"));
    textures.push_back(assets->get<cugl::Texture>("shiprightidle"));
    _dog->setRunTexture(textures);
    _devil->setRunTexture(textures);
    
    textures.clear();
    textures.push_back(assets->get<cugl::Texture>("shipleftbite"));
    textures.push_back(assets->get<cugl::Texture>("shiprightbite"));
    _dog->setBiteTexture(textures);
    
    return true;
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

void OverWorld::devilUpdate(InputController& _input,cugl::Size totalSize){
    _devil->move(totalSize * WORLD_SIZE);
}
void OverWorld::update(InputController& _input, cugl::Size totalSize){
    dogUpdate(_input,totalSize);
    devilUpdate(_input, totalSize);
}

void OverWorld::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,cugl::Size totalSize){
    _dog->draw(batch, totalSize);
    _devil->draw(batch, totalSize);
}
