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
#include "AbstractSpawner.h"
#include "SimpleSpawner.h"
#include "OverWorld.h"


class SpawnerController{
public:
    std::shared_ptr<cugl::Texture> _texture;
    
    /* Set of spawners */
    std::unordered_set<std::shared_ptr<AbstractSpawner>> _spawners;
    std::vector<std::shared_ptr<SpriteAnimationNode>> animationNodes;

    float difficulty;
    float accumulatedTime;
    
    /* Functions */
    SpawnerController();
    
    ~SpawnerController();
    void update(MonsterController& monsterController, OverWorld& overWorld, float timestep);
    
    bool init(const std::vector<LevelModel::Spawner>& startLocs, std::shared_ptr<cugl::AssetManager> assets );
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    void setRootNode(const std::shared_ptr<scene2::SceneNode>& _worldNode, bool isHost);
    bool win(){return _spawners.empty();}
    
    std::shared_ptr<cugl::scene2::SceneNode> getSpawnerNode() const{
        return baseSpawnerNode;
    }
private:
    std::shared_ptr<cugl::scene2::SceneNode> baseSpawnerNode;
    std::shared_ptr<NetEventController> _network;
    bool _isHost;
};


#endif /* SpawnerController_hpp */
