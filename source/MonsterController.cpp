//
//  MonsterController.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/21/24.
//
#include "MonsterController.h"

#define DYNAMIC_COLOR   Color4::YELLOW
int generateRandomInclusiveHighLow(int low, int high)
{
    // Static used for the seed to ensure it's only seeded once
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high); // Range is 1 to 3, inclusive
    return dis(gen);
}

bool MonsterController::init(OverWorld& overWorld,
     std::shared_ptr<cugl::scene2::SceneNode> debugNode){
    _current.clear();
    _pending.clear();
    _absorbEnem.clear();
    _debugNode = debugNode;

    for (const cugl::Vec3& cluster : overWorld.getLevelModel()->preSpawnLocs()){
        float cx = cluster.x;
        float cy = cluster.y;
//        int count = (int) round(cluster.z);
//        for(int i = 0; i < count; i++) {
            spawnStaticBasicEnemy(Vec2(cx,cy), overWorld, 1);
            spawnBombEnemy(Vec2(cx,cy), overWorld, 1);
//        spawnAbsorbEnemy(Vec2(cx,cy), overWorld, 1);
        spawnSpawnerEnemy(Vec2(cx,cy), overWorld, 1); // TODO SPAWN ENEMY IN MONSTER CONTROLLER
//        }
    }
    return true;
}
void MonsterController::postUpdate(){
    for (std::shared_ptr<AbstractEnemy> curEnemy: _pending){
        _current.insert(curEnemy);
    }
    _pending.clear();
}

void MonsterController::retargetToDecoy( OverWorld& overWorld){
    int totalTargets = overWorld.getTotalTargets(); // really brittle be careful
    for (std::shared_ptr<AbstractEnemy> enemy : getEnemies()){
        enemy->setTargetIndex(totalTargets-1); // added pending decoy to this iteration
    }
}

void MonsterController::retargetCloset( OverWorld& overWorld){
    cugl::Vec2 dogPos = overWorld.getDog()->getPosition();
    int baseSize = (int) overWorld.getBaseSet()->_bases.size();
    int decoySize = (int) overWorld.getDecoys()->getCurrentDecoys().size();
    for (std::shared_ptr<AbstractEnemy> enemy : getEnemies()){
        Vec2 enemyPos = enemy->getPosition();
        int index = 0;
        float dist = (enemyPos - dogPos).length();
        int sizeBases = baseSize;
        for (int i = 0 ;i < sizeBases;i++){
            Vec2 basePos = overWorld.getBaseSet()->_bases[i]->getPos();
            float curDist = (enemyPos -  basePos).length();
            if (curDist < dist){
                dist = curDist;
                index = i + 1;
            }
        }
        int sizeDecoys = decoySize;
        for (int i = 0 ;i < sizeDecoys; i++){
            Vec2 decoyPos = overWorld.getDecoys()->getCurrentDecoys().at(i)->getPos();
            float curDist = (enemyPos -  decoyPos).length();
            if (curDist < dist){
                dist = curDist;
                index = i + 1 + sizeBases;
            }
        }
        enemy->setTargetIndex(index);
    }
}
void MonsterController::update(float timestep, OverWorld& overWorld){
    if (!overWorld._isHost){
        return;
    }
    std::shared_ptr<DecoySet> decoySet = overWorld.getDecoys();
    if (decoySet->addedNewDecoy()){
        retargetToDecoy(overWorld);
        return;
    }
    if (decoySet->removedDecoy()){
        retargetCloset(overWorld);
        return;
    }
    
    //CULog("Boundary World Size in MonsterController: %zu", overWorld.getWorld()->getBoundaryWorld().size());
    for (std::shared_ptr<AbstractEnemy> curEnemy: _current){
        curEnemy->preUpdate(timestep, overWorld);
        if (std::shared_ptr<SpawnerEnemy> spawnerEnemy = std::dynamic_pointer_cast<SpawnerEnemy>(curEnemy)){
            if (spawnerEnemy->canAttack()){
                spawnerEnemy->resetAttack();
                spawnBasicEnemy(spawnerEnemy->getPosition() - Vec2(0.2,0.2), overWorld, 1);
            }
        }
    }
}
void MonsterController::setMeleeAnimationData(std::shared_ptr<cugl::JsonValue> data,
                           std::shared_ptr<cugl::AssetManager> _assets){
    _staticMeleeFactory = StaticMeleeFactory::alloc(data, _assets);
    _staticMeleeFactID = _network->getPhysController()->attachFactory(_staticMeleeFactory);
    _meleeFactory = MeleeFactory::alloc(data, _assets);
    _meleeFactID = _network->getPhysController()->attachFactory(_meleeFactory);
}

void MonsterController::setSpawnerAnimationData(std::shared_ptr<cugl::JsonValue> data,
                           std::shared_ptr<cugl::AssetManager> _assets){
    _spawnerEnemyFactory = SpawnerEnemyFactory::alloc(data, _assets);
    _spawnerEnemyFactID = _network->getPhysController()->attachFactory(_spawnerEnemyFactory);
}

void MonsterController::setBombAnimationData(std::shared_ptr<cugl::JsonValue> data,
                        std::shared_ptr<cugl::AssetManager> _assets){
    _bombEnemyFactory = BombFactory::alloc(data, _assets);
    _bombEnemyFactID = _network->getPhysController()->attachFactory(_bombEnemyFactory);
}
void MonsterController::setAbsorbAnimationData(std::shared_ptr<cugl::JsonValue> data,
                        std::shared_ptr<cugl::AssetManager> _assets){
    _absorbEnemyFactory = AbsorbFactory::alloc(data, _assets);
    _absorbEnemyFactID = _network->getPhysController()->attachFactory(_absorbEnemyFactory);
}
void MonsterController::setHealthBar(std::shared_ptr<cugl::scene2::ProgressBar> bar){
    _healthBar = bar;
}

void MonsterController::powerSize(float power, Size& size) {
    size *= power;
}

float MonsterController::powerHealth(float power, int hp) {
    return (int)(hp * power * power);
}

void MonsterController::spawnAbsorbEnemy(cugl::Vec2 pos, OverWorld& overWorld, float power){
    if (!overWorld._isHost){
        return;
    }
    Size mySize(1.0,1.0);
    int hp = 3;
    powerSize(power, mySize);
    hp = powerHealth(power, hp);
    auto params = _absorbEnemyFactory->serializeParams(pos, mySize, 3, 0);
    auto pair = _network->getPhysController()->addSharedObstacle(_absorbEnemyFactID, params);
//        static_enemy->setHealthBar(_healthBar);
    pair.first->setDebugScene(_debugNode);
    if (auto static_enemy = std::dynamic_pointer_cast<AbstractEnemy>(pair.first)){
        _pending.emplace(static_enemy);
        _absorbEnem.emplace(std::dynamic_pointer_cast<AbsorbEnemy>(static_enemy));
    }
}

void MonsterController::spawnBasicEnemy(cugl::Vec2 pos, OverWorld& overWorld, float power){
    if (!overWorld._isHost){
        return;
    }
    int numTargets =  overWorld.getTotalTargets();
    int chosenTarget = generateRandomInclusiveHighLow(0, numTargets-1);
    Size mySize(1,1);
    int hp = 3;
    powerSize(power, mySize);
    hp = powerHealth(power, hp);
    auto params = _meleeFactory->serializeParams(pos, mySize, hp, chosenTarget);
    auto pair = _network->getPhysController()->addSharedObstacle(_meleeFactID, params);
//        static_enemy->setHealthBar(_healthBar);
    pair.first->setDebugScene(_debugNode);
    if (auto static_enemy = std::dynamic_pointer_cast<AbstractEnemy>(pair.first)){
        _pending.emplace(static_enemy);
    }
}

void MonsterController::spawnSpawnerEnemy(cugl::Vec2 pos, OverWorld& overWorld, float power){
    if (!overWorld._isHost){
        return;
    }
    int chosenTarget = 0;
    Size mySize(1,1);
    int hp = 3;
    powerSize(power, mySize);
    hp = powerHealth(power, hp);
    auto params = _meleeFactory->serializeParams(pos, mySize, 3, chosenTarget);
    auto pair = _network->getPhysController()->addSharedObstacle(_spawnerEnemyFactID, params);
//        static_enemy->setHealthBar(_healthBar);
    pair.first->setDebugScene(_debugNode);
    if (auto static_enemy = std::dynamic_pointer_cast<AbstractEnemy>(pair.first)){
        _pending.emplace(static_enemy);
    }
}

void MonsterController::spawnStaticBasicEnemy(cugl::Vec2 pos, OverWorld& overWorld, float power){
    if (!overWorld._isHost){
        return;
    }
    Size mySize(1,1);
    int hp = 3;
    powerSize(power, mySize);
    hp = powerHealth(power, hp);
    auto params = _staticMeleeFactory->serializeParams(pos, mySize, hp, 0);
    auto pair = _network->getPhysController()->addSharedObstacle(_staticMeleeFactID, params);
//        static_enemy->setHealthBar(_healthBar);
    pair.first->setDebugScene(_debugNode);
    if (auto static_enemy = std::dynamic_pointer_cast<AbstractEnemy>(pair.first)){
        _pending.emplace(static_enemy);
    }
}

void MonsterController::spawnBombEnemy(cugl::Vec2 pos, OverWorld& overWorld, float power){
    if (!overWorld._isHost){
        return;
    }
    Size mySize(1,1);
    int hp = 3;
    powerSize(power, mySize);
    hp = powerHealth(power, hp);
    auto params = _bombEnemyFactory->serializeParams(pos, mySize, hp, 0);
    auto pair = _network->getPhysController()->addSharedObstacle(_bombEnemyFactID, params);
//        static_enemy->setHealthBar(_healthBar);
    pair.first->setDebugScene(_debugNode);
    if (auto static_enemy = std::dynamic_pointer_cast<AbstractEnemy>(pair.first)){
        _pending.emplace(static_enemy);
    }
}
void MonsterController::removeEnemy(std::shared_ptr<AbstractEnemy> enemy){
    getNetwork()->getPhysController()->removeSharedObstacle(enemy);
    enemy->getTopLevelNode()->removeFromParent();
    if (auto absorb  = std::dynamic_pointer_cast<AbsorbEnemy>(enemy)){
        _absorbEnem.erase(absorb);
    }
}
