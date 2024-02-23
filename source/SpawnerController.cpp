//
//  SpawnerController.cpp
//  Ship
//
//  Created by Henry Gao on 2/23/24.
//
#include "SpawnerController.h"

SpawnerController::SpawnerController(){
    
}
SpawnerController::~SpawnerController(){
    
}
void SpawnerController::update(AsteroidSet &aset){
//    CULog("num Spawns %d\n", _spawners.size());
    for(auto& spawner : _spawners) {
        spawner->update();
        CULog("cnt %d\n", spawner->getCnt());
        if (spawner->canSpawn()){
            CULog("spawned\n");
            spawner->reloadSpawner();
            aset.spawnAsteroid(spawner->getPos(), cugl::Vec2(1,1));
        }
    }
}

//void SpawnerController::draw(){
//    for(auto& spawnerPtr : spawner) {
//        spawnerPtr->draw();
//    }
//}


bool SpawnerController::init(std::shared_ptr<cugl::JsonValue> data) {
    if (data) {
        _spawners.clear();
        CULog("INIT\n");
        if (data->get("start,Rate")){
            auto spawnerValues = data->get("start,Rate")->children();
            for (auto it = spawnerValues.begin(); it != spawnerValues.end();it++){
                std::shared_ptr<cugl::JsonValue> entry = (*it);
                cugl::Vec2 pos;
                pos.x = entry->get(0)->get(0)->asFloat(0);
                pos.y = entry->get(0)->get(1)->asFloat(0);
                int spawnRate = entry->get(1)->asInt(0);
                _spawners.insert(std::make_shared<Spawner>(spawnRate,pos));
            }
        }
        return true;
    }
    return false;
}
