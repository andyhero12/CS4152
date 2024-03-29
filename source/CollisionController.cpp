//
//  GLCollisionController.h
//  Programming Lab
//
//  Unless you are making a point-and-click adventure game, every single
//  game is going to need some sort of collision detection.  In a later
//  lab, we will see how to do this with a physics engine. For now, we use
//  custom physics.
//
//  You might ask why we need this file when we have Box2d. That is because
//  we are trying to make this code as close to that of 3152 as possible. At
//  this point in the semester of 3152, we had not covered Box2d.
//
//  However, you will notice that this is NOT A CLASS.  The collision
//  controller in 3152 did not have any state to speak of (it had some cache
//  objects that are completely unnecessary in C++.  So we can just do this
//  as a collection of functions.  But if you do that, we recommend that
//  you put the functions together in a namespace, like we have done here.
//
//  Author: Walker M. White
//  Based on original GameX Ship Demo by Rama C. Hoetzlein, 2002
//  Version: 2/21/21
//
#include "CollisionController.h"

/** Impulse for giving collisions a slight bounce. */
#define COLLISION_COEFF     0.1f

using namespace cugl;
void CollisionController::intraOverWorldCollisions(OverWorld& overWorld){
    if (healFromBaseCollsion(*overWorld.getBaseSet(), overWorld.getDog())){
        CULog("HEAL DETECTED\n");
    }
}

void CollisionController::overWorldMonsterControllerCollisions(OverWorld& overWorld, MonsterController& monsterController){
    return;
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& monsterEnemies = monsterController.getEnemies();
    
    if (monsterDogCollision(overWorld.getDog(), monsterEnemies)){
        // CULog("MONSTER DOG COLLISION DETECTED\n");
    }
    if (monsterDecoyCollision(overWorld.getDecoys(), monsterEnemies)){
        // CULog("MONSTER DECOY COLLISION DETECTED\n");
    }
    if (monsterBaseCollsion(overWorld.getBaseSet(), monsterEnemies)){
        CULog("Monster Base COLLISION DETECTED\n");
    }

}
void CollisionController::attackCollisions(OverWorld& overWorld, MonsterController& monsterController, SpawnerController& spawnerController){
    AttackPolygons& attacks = overWorld.getAttackPolygons();
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& monsterEnemies = monsterController.getEnemies();
    std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners = spawnerController._spawners;
    std::shared_ptr<Dog> dog = overWorld.getDog();
    for (const std::shared_ptr<ActionPolygon>& action: attacks.currentAttacks){
        switch (action->getAction()){
            case (Action::SHOOT):
                hugeBlastCollision(action->getPolygon(), monsterEnemies); // Play blast sound
                break;
            case (Action::EXPLODE):
                resolveBlowup(action->getPolygon(),monsterEnemies, spawners); // play boom sound
                break;
            case (Action::BITE):
                resolveBiteAttack(action->getPolygon(), monsterEnemies, overWorld);
//                resolveBiteAttack(action->getPolygon(),monsterEnemies, dog);
                break;
            default:
                CULog("Action not used in Collisions\n");
        };
    }
}
bool CollisionController::monsterBaseCollsion(std::shared_ptr<BaseSet> curBases, std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies){
    bool collision = false;
    auto itP = curBases->_bases.begin();
    while (itP != curBases->_bases.end()){
        std::shared_ptr<Base> base = *itP;
        auto itA = curEnemies.begin();
        bool hitSomething = false;
        while ( itA != curEnemies.end()){
            const std::shared_ptr<AbstractEnemy>& enemy = *itA;
            Vec2 norm = base->getPos() - enemy->getPos();
            float distance = norm.length();
            float impactDistance = enemy->getRadius()+ 10;
            auto curA = itA;
            itA++;
            if (distance < impactDistance) {
                hitSomething = true;
                collision = true;
                curEnemies.erase(curA);
            }
        }
        auto curP = itP++;
        if (hitSomething){
            (*curP)->reduceHealth(5);
        }
    }
    return collision;
}
bool CollisionController::monsterDecoyCollision(std::shared_ptr<DecoySet> decoySet, std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies){
    bool collide = false;
    std::vector<std::shared_ptr<Decoy>> decoys = decoySet->getCurrentDecoys();
    for (std::shared_ptr<Decoy> curDecoy: decoys){
        for (std::shared_ptr<AbstractEnemy> enemy: curEnemies){
            Vec2 norm = curDecoy->getPos() - enemy->getPos();
            float distance = norm.length();
            float impactDistance = enemy->getRadius() +5;
            if (distance < impactDistance){ // need noise
                if (enemy->canAttack()){
                    collide = true;
                    enemy->resetAttack();
                    curDecoy->subHealth(enemy->getDamage());
                }
            }
        }
    }
    return collide;
}
bool CollisionController::monsterDogCollision(std::shared_ptr<Dog> curDog, std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies){
    bool collision = false;
    auto it = curEnemies.begin();
    while (it != curEnemies.end()){
        std::shared_ptr<AbstractEnemy> enemy = *it;
        Vec2 norm = curDog->getPosition() - enemy->getPos();
        float distance = norm.length();
        float impactDistance = enemy->getRadius();
        it++;
        if (distance < impactDistance) {
            norm.normalize();
            Vec2 temp = norm * ((impactDistance - distance) / 2);
            curDog->setPosition(curDog->getPosition()+temp);
            enemy->setPos(enemy->getPos() - temp);
            if (enemy->canAttack()){
                collision = true;
                enemy->resetAttack();
                curDog->setHealth(curDog->getHealth()-enemy->getDamage());
            }
        }
    }
    return collision;
    
    
    return false;
}

void CollisionController::resolveBiteAttack(const cugl::Poly2& bitePolygon, std::unordered_set<std::shared_ptr<AbstractEnemy>>& monsterEnemies,
                                            OverWorld& overWorld){
    auto itA = monsterEnemies.begin();
    bool hitSomething = false;
    while ( itA != monsterEnemies.end()){
        const std::shared_ptr<AbstractEnemy>& enemy = *itA;
        auto curA = itA;
        itA++;
        if (bitePolygon.contains(enemy->getPos())){
            hitSomething = true;
            enemy->setHealth(enemy->getHealth() - 1);
            if(enemy->getHealth() <= 0){
                enemy->executeDeath(overWorld);
                overWorld.getDog()->addAbsorb((*curA)->getAbsorbValue());
                monsterEnemies.erase(curA);
            }
        }
    }
}
bool CollisionController::healFromBaseCollsion( BaseSet& bset, std::shared_ptr<Dog> ship){
    if (!ship->canHeal()|| ship->getHealth() >= ship->getMaxHealth()){
        return false;
    }
    auto itP = bset._bases.begin();
    float healDistanceCutoff = 50.0;
    while (itP != bset._bases.end()){
        std::shared_ptr<Base> base = *itP;
        Vec2 norm = base->getPos() - ship->getPosition();
        float distance = norm.length();
        itP++;
        if (distance < healDistanceCutoff){
            ship->resetHeal();
            ship->setHealth(ship->getHealth()+base->getHealValue());
            return true;
        }
    }
    return false;
}
void CollisionController::hugeBlastCollision(const cugl::Poly2& blastRectangle, std::unordered_set<std::shared_ptr<AbstractEnemy>>& enemies){
    auto itA = enemies.begin();
    while (itA != enemies.end()){
        const std::shared_ptr<AbstractEnemy>& enemy = *itA;
        Vec2 enemyPos = enemy->getPos();
        auto curA = itA;
        itA++;
        if (blastRectangle.contains(enemyPos)){
            enemies.erase(curA);
        }
    }
}
void CollisionController::resolveBlowup(const cugl::Poly2& blastCircle, std::unordered_set<std::shared_ptr<AbstractEnemy>>& monsterEnemies, std::unordered_set<std::shared_ptr<AbstractSpawner>>& spawners){
    auto itA = monsterEnemies.begin();
    while (itA != monsterEnemies.end()){
        const std::shared_ptr<AbstractEnemy>& enemy = *itA;
        auto curA = itA++;
        if (blastCircle.contains(enemy->getPos())){
            monsterEnemies.erase(curA);
        }
    }
    auto itS = spawners.begin();
    while (itS != spawners.end()){
        const std::shared_ptr<AbstractSpawner>& spawn = *itS;
        auto curS = itS;
        itS++;
        if (blastCircle.contains(spawn->getPos())){
            spawners.erase(curS);
        }
    }
}
