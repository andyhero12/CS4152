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
    if (monsterDogCollision(overWorld.getDog(), monsterController.getEnemies())){
        CULog("MONSTER DOG COLLISION DETECTED\n");
    }
}
bool CollisionController::monsterDogCollision(std::shared_ptr<Dog> curDog, std::unordered_set<std::shared_ptr<AbstractEnemy>>& curEnemies){
    bool collision = false;
    auto it = curEnemies.begin();
    while (it != curEnemies.end()){
        std::shared_ptr<AbstractEnemy> enemy = *it;
        Vec2 norm = curDog->getPosition() - enemy->getPos();
        float distance = norm.length();
        float impactDistance = curDog->getRadius() + enemy->getRadius();
        it++;
        if (distance < impactDistance) {
            norm.normalize();
            Vec2 temp = norm * ((impactDistance - distance) / 2);
            curDog->setPosition(curDog->getPosition()+temp);
            enemy->setPos(enemy->getPos() - temp);
            if (enemy)
            curDog->setHealth(curDog->getHealth()-enemy->getDamage());
        }
    }
    return collision;
    
    
    return false;
}

void CollisionController::resolveDecoyDamage(AsteroidSet& aset){
    for (std::shared_ptr<Decoy> curDecoy : aset._currentDecoys){
        for (std::shared_ptr<AsteroidSet::Asteroid> asteroid: aset.current){
            Vec2 norm = curDecoy->getPos() - asteroid->position;
            float distance = norm.length();
            float impactDistance = aset.getRadius()*asteroid->getScale();
            if (distance < impactDistance){ // need noise
                curDecoy->subHealth(asteroid->getDamage());
            }
        }
    }
}
void CollisionController::resolveAttacks(AttackPolygons& attacks,AsteroidSet& aset, std::unordered_set<std::shared_ptr<Spawner>>& spawners, std::shared_ptr<Dog> ship){
    for (const std::shared_ptr<ActionPolygon>& action: attacks.currentAttacks){
        switch (action->getAction()){
            case (Action::SHOOT):
                hugeBlastCollision(action->getPolygon(), aset); // Play blast sound
                break;
            case (Action::EXPLODE):
                resolveBlowup(action->getPolygon(),aset, spawners); // play boom sound
                break;
            case (Action::BITE):
                resolveBiteAttack(action->getPolygon(),aset, ship);
                break;
            default:
                CULog("Action not used in Collisions\n");
        };
    }
    
}

void CollisionController::resolveBiteAttack(const cugl::Poly2& bitePolygon, AsteroidSet& aset, std::shared_ptr<Dog> ship){
    auto itA = aset.current.begin();
    bool hitSomething = false;
    while ( itA != aset.current.end()){
        const std::shared_ptr<AsteroidSet::Asteroid>& rock = *itA;
        auto curA = itA++;
        if (bitePolygon.contains(rock->position)){
            hitSomething = true;
            rock->setHealth(rock->getHealth() - 1);
            if(rock->getHealth() <= 0){
                ship->addAbsorb((*curA)->getAbsorbValue());
                aset.current.erase(curA);
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
void CollisionController::hugeBlastCollision(const cugl::Poly2& blastRectangle, AsteroidSet& ast){
    auto itA = ast.current.begin();
    while (itA != ast.current.end()){
        const std::shared_ptr<AsteroidSet::Asteroid>& rock = *itA;
        Vec2 enemyPos = rock->position;
        auto curA = itA++;
        if (blastRectangle.contains(enemyPos)){
            ast.current.erase(curA);
        }
    }
}
void CollisionController::resolveBlowup(const cugl::Poly2& blastCircle, AsteroidSet& ast, std::unordered_set<std::shared_ptr<Spawner>>& spawners){
    auto itA = ast.current.begin();
    while (itA != ast.current.end()){
        const std::shared_ptr<AsteroidSet::Asteroid>& rock = *itA;
        auto curA = itA++;
        if (blastCircle.contains(rock->position)){
            ast.current.erase(curA);
        }
    }
    
    auto itS = spawners.begin();
    while (itS != spawners.end()){
        const std::shared_ptr<Spawner>& spawn = *itS;
        auto curS = itS++;
        if (blastCircle.contains(spawn->getPos())){
            spawners.erase(curS);
        }
    }
    
    
}

bool CollisionController::resolveCollision( BaseSet& bset, AsteroidSet& aset){
    bool collision = false;
//
    auto itP = bset._bases.begin();
    while (itP != bset._bases.end()){
        std::shared_ptr<Base> base = *itP;
        auto itA = aset.current.begin();
        bool hitSomething = false;
        while ( itA != aset.current.end()){
            const std::shared_ptr<AsteroidSet::Asteroid>& rock = *itA;
            Vec2 norm = base->getPos() - rock->position;
            float distance = norm.length();
            float impactDistance = aset.getRadius()*rock->getScale();

            
            // If this normal is too small, there was a collision
            auto curA = itA++;
            if (distance < impactDistance) {
                hitSomething = true;
                collision = true;
                aset.current.erase(curA);
            }
        }
        auto curP = itP++;
        if (hitSomething){
            (*curP)->reduceHealth(5);
        }
    }
//    collision = true;
    return collision;
}

/**
 * Returns true if there is a ship-asteroid collision
 *
 * In addition to checking for the collision, this method also resolves it.
 * That means it applies damage to the ship for EACH asteroid encountered.
 * It does not, however, play the sound. That happens in the main controller
 *
 * Note that this method must take wrap into consideration as well. If the
 * asteroid/ship can be drawn at multiple points on the screen, then it can
 * collide at multiple places as well.
 *
 * @param ship  The players ship
 * @param aset  The asteroid set
 *
 * @return true if there is a ship-asteroid collision
 */
bool CollisionController::resolveCollision(const std::shared_ptr<Dog>& ship, AsteroidSet& aset) {
    bool collision = false;
    auto it = aset.current.begin();
    while (it != aset.current.end()){
//    for(auto it = aset.current.begin(); it != aset.current.end(); ++it) {
        // Calculate the normal of the (possible) point of collision
        std::shared_ptr<AsteroidSet::Asteroid> rock = *it;
        
        // This loop finds the NEAREST collision if we include wrap for the asteroid/ship
        Vec2 norm = ship->getPosition()-rock->position;
        float distance = norm.length();
        float impactDistance = (ship->getRadius() + aset.getRadius()*rock->getScale());
        it++;
        if (distance < impactDistance) {
            // "Roll back" time so that the ships are barely touching (e.g. point of impact).
            norm.normalize();
            Vec2 temp = norm * ((impactDistance - distance) / 2);
            ship->setPosition(ship->getPosition()+temp);
            rock->position = rock->position-temp;
            ship->setHealth(ship->getHealth()-rock->getDamage());
        }
    }
    return collision;
}


