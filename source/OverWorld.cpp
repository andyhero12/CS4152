//
//  OverWorld.cpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#include "OverWorld.hpp"

#define WORLD_SIZE 3

void OverWorld::reset(cugl::Size resetSize) {
    _dog->setPosition(0, 0);
    _dog->setAbsorbValue(0);
    _dog->setAngle(0);
    _dog->setVelocity(cugl::Vec2::ZERO);
    _dog->setHealth(_constants->get("ship")->getInt("health",0));
    _devil->setPosition(resetSize/2);
    _bases->init(_constants->get("base"));
    _decoys->init();
    _attackPolygonSet.init();
}

bool OverWorld::initDog(std::shared_ptr<cugl::physics2::ObstacleWorld> obstacleWorld){
    _dog = std::make_shared<Dog>(Vec2(0, 0), _constants->get("ship"));
    std::vector<std::shared_ptr<cugl::Texture>> textures;
    textures.push_back(_assets->get<cugl::Texture>("shipleftidle"));
    textures.push_back(_assets->get<cugl::Texture>("shiprightidle"));
    _dog->setRunTexture(textures);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("shipleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("shiprightbite"));
    _dog->setBiteTexture(textures);
    _dog->buildObstacle(obstacleWorld);
    return true;
}

bool OverWorld::initDevil(){
    _devil = std::make_shared<Devil>(_dog, Vec2(1, 1), _constants->get("devil"));
    std::vector<std::shared_ptr<cugl::Texture>> textures;
    textures.push_back(_assets->get<cugl::Texture>("player0"));
    textures.push_back(_assets->get<cugl::Texture>("player1"));
    _devil->setRunTexture(textures);
    return true;
}

bool OverWorld::initBases(){
    _bases = std::make_shared<BaseSet>();
    _bases->init(_constants->get("base"));
    _bases->setTexture(_assets->get<Texture>("base"));
    return true;
}

bool OverWorld::initDecoys(){
    _decoys = std::make_shared<DecoySet>();
    _decoys->setTexture(_assets->get<Texture>("base"));
    return true;
}

bool OverWorld::init(const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size totalSize,std::shared_ptr<cugl::physics2::ObstacleWorld> obstacleWorld){
    std::cout << "Overworld physics world address " << obstacleWorld <<std::endl;
    _assets = assets;
    _constants = assets->get<cugl::JsonValue>("constants");
    _totalSize = totalSize;
    initDog(obstacleWorld);
    initDevil();
    initBases();
    initDecoys();
    return true;
}

void OverWorld::dogUpdate(InputController& _input, cugl::Size totalSize){
    if(_input.didChangeMode() && _dog->canChangeMode()){
        _dog->toggleMode();
        _dog->reloadMode();
    }
    if (_input.didPressFire() && _dog->canFireWeapon()){
        _dog->reloadWeapon();
        _dog->setAttack();
        _attackPolygonSet.addBite(_dog);
    }
    if (_input.didPressSpecial() && _dog->canFireWeapon()){
        _dog->reloadWeapon();
        if (_dog->getMode() == "SHOOT" && _dog->getAbsorb() > 5){
            _dog->subAbsorb(5);
            _attackPolygonSet.addShoot(_dog);
        }else if (_dog->getMode() == "BUILD" && _dog->getAbsorb() > 5 ){
            _dog->subAbsorb(5);
            _decoys->addNewDecoy(_dog->getPosition());
//            _decoys.emplace(std::make_shared<Decoy>(_dog->getPosition()));
        }else if (_dog->getMode() == "EXPLODE" && _dog->getAbsorb() > 10){
            _dog->subAbsorb(10);
            _attackPolygonSet.addExplode(_dog);
        }else {
            CULog("NOTHING\n");
        }
    }
    _dog->move( _input.getForward(),  _input.getTurn(),_input.getVelocity(), _input.getControllerState(),_input.getKeyboardState(), totalSize * WORLD_SIZE);
}

void OverWorld::devilUpdate(InputController& _input,cugl::Size totalSize){
    _devil->move(totalSize * WORLD_SIZE);
}
void OverWorld::update(InputController& _input, cugl::Size totalSize, float timestep){
    dogUpdate(_input,totalSize);
    devilUpdate(_input, totalSize);
    _attackPolygonSet.update(totalSize);
    _bases->update();
    _decoys->update(timestep);
}

void OverWorld::postUpdate(){
    _decoys->postUpdate();
}
void OverWorld::draw(const std::shared_ptr<cugl::SpriteBatch>& batch,cugl::Size totalSize){
    _attackPolygonSet.draw(batch,totalSize);
    _bases->draw(batch,totalSize);
    _dog->draw(batch, totalSize);
    _devil->draw(batch, totalSize);
    _decoys->draw(batch);
}
