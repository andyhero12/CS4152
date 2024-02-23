//
//  SpawnerController.hpp
//  Ship
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
#include "SLAsteroidSet.h"


class SpawnerController{
public:
    /* Monster Textures */
//    std::shared_ptr<cugl::Texture> _monsterTexture;
//    
//    /* Spawner Textures */
//    std::shared_ptr<cugl::Texture> _spawnerTexture;
    
    
    /* Set of spawners */
    std::unordered_set<std::shared_ptr<Spawner>> _spawners;
    
    /* Functions */
    SpawnerController();
    
    ~SpawnerController();
//    
//    void setMonsterTexture(std::shared_ptr<cugl::Texture> &monster){
//        _monsterTexture = monster;
//    }
//    
//    void setSpawnerTexture(std::shared_ptr<cugl::Texture> &spawn){
//        _spawnerTexture = spawn;
//    }
//    
    
    
    void update(AsteroidSet &aset);
    
    bool init(std::shared_ptr<cugl::JsonValue> data);
    
//    void draw();
    
    
private:
    
    

    
    
};


#endif /* SpawnerController_hpp */
