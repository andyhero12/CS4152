//
//  SpawnerController.cpp
//
//  Created by Henry Gao on 2/23/24.
//
#include "SpawnerController.h"
#include <random>

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
    for(auto& spawner : _spawners) {
        spawner->update(timestep);
        if (spawner->canSpawn()){
            spawner->reloadSpawner();
            monsterController.spawnBasicEnemy(spawner->getPos(),overWorld);
        }
    }
    
    
    auto it = _spawners.begin();
    while (it != _spawners.end()){
        std::shared_ptr<Spawner> spawner = *it;
        
        if (spawner->dead()){
            it = _spawners.erase(it);
        }
        else{
            ++it;
        }
    }
    
}
void SpawnerController::update(AsteroidSet &aset, float timestep){
    for(auto& spawner : _spawners) {
        spawner->update(timestep);
        if (spawner->canSpawn()){
            spawner->reloadSpawner();
            aset.spawnAsteroid(spawner->getPos(), cugl::Vec2(generateRandomValue(-2,2), generateRandomValue(-2,2)), fmin(spawner->getTimeElapsed()/10.0,2.0) + 1);
        }
    }
    
    
    auto it = _spawners.begin();
    while (it != _spawners.end()){
        std::shared_ptr<Spawner> spawner = *it;
        
        if (spawner->dead()){
            it = _spawners.erase(it);
        }
        else{
            ++it;
        }
    }
    
}

bool SpawnerController::init(std::shared_ptr<cugl::JsonValue> data) {
    if (data) {
        _spawners.clear();
//        CULog("INIT\n");
        if (data->get("start,Rate")){
            auto spawnerValues = data->get("start,Rate")->children();
            for (auto it = spawnerValues.begin(); it != spawnerValues.end();it++){
                std::shared_ptr<cugl::JsonValue> entry = (*it);
                cugl::Vec2 pos;
                pos.x = entry->get(0)->get(0)->asFloat(0);
                pos.y = entry->get(0)->get(1)->asFloat(0);
                int spawnRate = entry->get(1)->asInt(0);
                int health = 10;
                _spawners.insert(std::make_shared<Spawner>(spawnRate,pos,health));
            }
        }
        return true;
    }
    return false;
}



void SpawnerController::setTexture(const std::shared_ptr<cugl::Texture>& value){
    _texture = value;
//    for(auto& spawner : _spawners) {
//        spawner->setTexture(value);
//        std::cout << spawner->getTexture()<< std::endl;
//    }
}

void SpawnerController::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    for(auto& spawner : _spawners) {
        cugl::Vec2 pos = spawner->getPos();
        cugl::Vec2 origin(0, 0);
        cugl::Affine2 trans;
        float scale = 2;
        trans.scale(scale);
        trans.translate(pos);
        batch->draw(_texture, origin, trans);
        
        
//        std::cout << spawner->getTexture()<< std::endl;
//        spawner->draw(batch, size);
    }
    
}
