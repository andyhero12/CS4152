//
//  OverWorld.hpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//
#pragma once
#ifndef OverWorld_hpp
#define OverWorld_hpp
#include <cugl/cugl.h>
#include <unordered_set>
#include "Base.h"
#include "Dog.h"
#include "Devil.h"
#include "Animation.h"
#include "Decoy.hpp"
#include "Spawner.h"
#include "BaseSet.h"
#include "InputController.h"
#include "AttackPolygons.hpp"

#endif /* OverWorld_hpp */


class OverWorld{
private:
        std::shared_ptr<Dog> _dog;
        std::shared_ptr<Devil> _devil;
        std::vector<std::shared_ptr<Decoy>> _decoys;
        std::vector<std::shared_ptr<Spawner>> _spawners;
        std::shared_ptr<BaseSet> _bases;
        std::shared_ptr<cugl::JsonValue> _constants;
        std::shared_ptr<cugl::AssetManager> _assets;
        cugl::Size _totalSize;
        AttackPolygons _attackPolygonSet;
public:
    
    OverWorld(){
        
    }
    ~OverWorld(){
        
    }
    
    void reset(cugl::Size resetSize);
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size totalSize);
    bool initDog();
    bool initDevil();
    bool initBases();
    
    void dogUpdate(InputController& _input,cugl::Size totalSize);
    void devilUpdate(InputController& _input,cugl::Size totalSize);
    void update(InputController& input, cugl::Size totalSize);
    
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,cugl::Size totalSize);
    
    
    
    std::shared_ptr<Dog> getDog(){
        return _dog;
    }
    std::shared_ptr<Devil> getDevil(){
        return _devil;
    }
    std::vector<std::shared_ptr<Decoy>> getDecoys(){
        return _decoys;
    }
    std::shared_ptr<BaseSet> getBaseSet(){
        return _bases;
    }
    AttackPolygons& getAttackPolygons(){
        return _attackPolygonSet;
    }
    void setDog(std::shared_ptr<Dog> m_dog){
        _dog = m_dog;
    }
    void setDevil(std::shared_ptr<Devil> m_devil){
        _devil = m_devil;
    }
};
