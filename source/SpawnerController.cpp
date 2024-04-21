//
//  SpawnerController.cpp
//
//  Created by Henry Gao on 2/23/24.
//
#include "SpawnerController.h"
#include <random>
#include <cmath>

SpawnerController::SpawnerController(){
    
}
SpawnerController::~SpawnerController(){
    
}

// Function to generate a random value between 1 and 3
int generateRandomValue(int left, int right) {
    // Static used for the seed to ensure it's only seeded once
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(left, right); // Range is 1 to 3, inclusive
    return dis(gen);
}

void SpawnerController::update(MonsterController& monsterController, OverWorld& overWorld, float timestep){
    //_difficulty *= 1.00077046f;
   // _difficulty *= 1.00003851f;
    accumulatedTime += timestep;
    //cout << (std::to_string(difficulty));
    for(auto& spawner : _spawners) {
        spawner->update(monsterController, overWorld, timestep, (float)std::pow(1.00231316, accumulatedTime) + difficulty);
        
    }
    for (auto& spawner : animationNodes){
        spawner->update();
    }
    
    auto it = _spawners.begin();
    while (it != _spawners.end()){
        std::shared_ptr<AbstractSpawner> spawner = *it;
        
        if (spawner->dead()){
            //CULog("nani");
            it = _spawners.erase(it);
            spawner->getSpawnerNode()->removeFromParent();
            difficulty += 0.1;
        }
        else{
            ++it;
        }
    }
    
}

bool SpawnerController::init(const std::vector<LevelModel::Spawner>& startLocs, std::shared_ptr<cugl::AssetManager> assets) {
    _spawners.clear();
    animationNodes.clear();
    baseSpawnerNode = cugl::scene2::SceneNode::alloc();
    for (int i =0; i< startLocs.size(); i++){
        LevelModel::Spawner spawner = startLocs.at(i);
        cugl::Vec2 pos = Vec2(spawner.spawnerX, spawner.spawnerY);
        int health = spawner.hp;
        std::shared_ptr<SimpleSpawner> curSpawner = std::make_shared<SimpleSpawner>(spawner.regularDelay,pos,health,spawner.initDelay,spawner.primaryEnemy, spawner.secondaryEnemy, spawner.tertiaryEnemy);
//        auto drawNode = cugl::scene2::PolygonNode::allocWithTexture(assets->get<cugl::Texture>("spawner"));
        auto drawNode = SpriteAnimationNode::allocWithSheet(assets->get<cugl::Texture>("spawner"), 1,1,1);
        drawNode->setScale(cugl::Size(1,1)/48);
//        drawNode->setContentSize(cugl::Size(1,1));
        drawNode->setPosition(pos);
        drawNode->setAnchor(cugl::Vec2::ANCHOR_CENTER);
//        animationNodes.push_back(drawNode);
        baseSpawnerNode->addChild(drawNode);
        curSpawner->setSpawnerNode(drawNode);
        _spawners.insert(curSpawner);
    }
    difficulty = 0;
    accumulatedTime = 0;
    return true;
}
void SpawnerController::setRootNode(const std::shared_ptr<scene2::SceneNode>& _worldNode, bool isHost){
    
    _isHost = isHost;
    // Add the Base Spawner Node
    _worldNode->addChild(baseSpawnerNode);
    
    
}
