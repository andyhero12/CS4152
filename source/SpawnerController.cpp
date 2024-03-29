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
        spawner->update(monsterController, overWorld, timestep);
//        if (spawner->canSpawn()){
//            spawner->reloadSpawner();
//            monsterController.spawnBasicEnemy(spawner->getPos(),overWorld);
//        }
    }
    
    
    auto it = _spawners.begin();
    while (it != _spawners.end()){
        std::shared_ptr<AbstractSpawner> spawner = *it;
        
        if (spawner->dead()){
            it = _spawners.erase(it);
        }
        else{
            ++it;
        }
    }
    
}

bool SpawnerController::init(std::shared_ptr<cugl::JsonValue> data,const std::vector<cugl::Vec2>& startLocs) {
    if (data) {
        _spawners.clear();
        // CULog("INIT\n");
        if (data->get("start,Rate")){
            auto spawnerValues = data->get("start,Rate")->children();
            for (int i =0; i< startLocs.size(); i++){
                std::shared_ptr<cugl::JsonValue> entry = spawnerValues.at(i);
                cugl::Vec2 pos;
                // TEMPORARY MAGIC NUMBER STUFF
                pos.x = startLocs.at(i).x / 64;
                pos.y = startLocs.at(i).y / 64;
                //pos.x = 0;
                //pos.y = 0;
                int spawnRate = 200;
                int health = 10;
                std::shared_ptr<MeleeSpawner> curSpawner = std::make_shared<MeleeSpawner>(spawnRate,pos,health,0);
                curSpawner->setTexture(_texture);
                _spawners.insert(curSpawner);
            }
        }
        return true;
    }
    return false;
}

void SpawnerController::setTexture(const std::shared_ptr<cugl::Texture>& value ){
    _texture = value;
}

void SpawnerController::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    for(const std::shared_ptr<AbstractSpawner>& spawner : _spawners) {
        //std::cout << "draw\n";
        spawner->draw(batch, size);
        // why didnt we use spawner draw?
        /*
        cugl::Vec2 pos = spawner->getPos();
        cugl::Vec2 origin(0, 0);
        cugl::Affine2 trans;
        float scale = 2;
        trans.scale(scale);
        trans.translate(pos);
        spawner->draw(batch, size);
        */
    }
    
}
