//
//  SpawnerController.hpp
//  Dog
//
//  Created by Henry Gao on 2/23/24.
//
#pragma once

#ifndef SpawnerController_hpp
#define SpawnerController_hpp

#include "Spawner.h"
#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>
#include "MonsterController.h"
#include "OverWorld.hpp"


class SpawnerController{
public:
    /* Monster Textures */
//    std::shared_ptr<cugl::Texture> _monsterTexture;
//    
//    /* Spawner Textures */
    std::shared_ptr<cugl::Texture> _texture;
    
    
    /* Set of spawners */
    std::unordered_set<std::shared_ptr<Spawner>> _spawners;
    
    /* Functions */
    SpawnerController();
    
    ~SpawnerController();

    void setTexture(const std::shared_ptr<cugl::Texture>& value);
    void update(MonsterController& monsterController, OverWorld& overWorld, float timestep);
    
    bool init(std::shared_ptr<cugl::JsonValue> data);
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    
    bool win(){return _spawners.empty();}
    
private:
    
    

    
    
};


#endif /* SpawnerController_hpp */
