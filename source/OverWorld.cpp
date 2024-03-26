//
//  OverWorld.cpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#include "OverWorld.h"

#define WORLD_SIZE 3

void OverWorld::reset(cugl::Size resetSize) {
    _dog->setPosition(resetSize/2);
    _dog->setAbsorbValue(0);
    _dog->setAngle(0);
    _dog->setVelocity(cugl::Vec2::ZERO);
    _dog->setHealth(_constants->get("ship")->getInt("health",0));
    _devil->setPosition(resetSize/2);
    _bases->init(_constants->get("base"));
    _decoys->init();
    _attackPolygonSet.init();
}

bool OverWorld::initDog(){
    _dog = std::make_shared<Dog>(_totalSize/2, _constants->get("ship"));
    
    // medium dog assets
    std::vector<std::shared_ptr<cugl::Texture>> textures;
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightrun"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftrun"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontrun"));
    _dog->setRunTextureMedium(textures);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontbite"));
    _dog->setBiteTextureMedium(textures);
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightidle"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftidle"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogfrontidle"));
    _dog->setIdleTextureMedium(textures);
    
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("mediumdogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("mediumdogleftshoot"));
    _dog->setShootTextureMedium(textures);
    
    // small dog assets
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftshoot"));
    _dog->setShootTextureSmall(textures);
    
    
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightidle"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftidle"));
    _dog->setIdleTextureSmall(textures);
    
    
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightrun"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftrun"));
    _dog->setRunTextureSmall(textures);
    
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("smalldogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("smalldogfrontbite"));
    _dog->setBiteTextureSmall(textures);
    
    // large dog assets
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("largedogrightshoot"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftshoot"));
    _dog->setShootTextureLarge(textures);
    
    
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("largedogrightidle"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftidle"));
    _dog->setIdleTextureLarge(textures);
    
    
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("largedogrightrun"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftrun"));
    _dog->setRunTextureLarge(textures);
    
    
    textures.clear();
    textures.push_back(_assets->get<cugl::Texture>("largedogrightbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogleftbite"));
    textures.push_back(_assets->get<cugl::Texture>("largedogfrontbite"));
    _dog->setBiteTextureLarge(textures);
    
    return true;
}

bool OverWorld::initDevil(){
    _devil = std::make_shared<Devil>(_dog, _totalSize/2, _constants->get("devil"));
    std::vector<std::shared_ptr<cugl::Texture>> textures;
    textures.push_back(_assets->get<cugl::Texture>("player0"));
    textures.push_back(_assets->get<cugl::Texture>("player1"));
    _devil->setRunTexture(textures);
    return true;
}

bool OverWorld::initBases(){
    _bases = std::make_shared<BaseSet>();
    _bases->init(_constants->get("base"));
    _bases->setTexture(_assets->get<cugl::Texture>("base"));
    return true;
}

bool OverWorld::initDecoys(){
    _decoys = std::make_shared<DecoySet>();
    _decoys->setTexture(_assets->get<cugl::Texture>("base"));
    return true;
}

bool OverWorld::init(const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size totalSize){
    _assets = assets;
    _constants = assets->get<cugl::JsonValue>("constants");
    _totalSize = totalSize;
    initDog();
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
        _dog->setBite();
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
