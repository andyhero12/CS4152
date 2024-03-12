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
#include "SLCollisionController.h"

/** Impulse for giving collisions a slight bounce. */
#define COLLISION_COEFF     0.1f

using namespace cugl;

bool CollisionController::healFromBaseCollsion( BaseSet& bset, std::shared_ptr<Ship> ship){
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
void CollisionController::hugeBlastCollision(cugl::Poly2& blastRectangle, AsteroidSet& ast){
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
void CollisionController::resolveBlowup(const std::shared_ptr<Ship>& ship, AsteroidSet& ast, std::unordered_set<std::shared_ptr<Spawner>>& spawners){
    float distanceCutoff = 100.0;
    auto itA = ast.current.begin();
    while (itA != ast.current.end()){
        const std::shared_ptr<AsteroidSet::Asteroid>& rock = *itA;
        Vec2 norm = ship->getPosition() - rock->position;
        float distance = norm.length();
        auto curA = itA++;
//        CULog("distance %f\n", distance);
        if (distance < distanceCutoff){
            ast.current.erase(curA);
        }
    }
    
    auto itS = spawners.begin();
    while (itS != spawners.end()){
        const std::shared_ptr<Spawner>& rock = *itS;
        Vec2 norm = ship->getPosition() - rock->getPos();
        float distance = norm.length();
        if (distance < distanceCutoff){
            itS = spawners.erase(itS);
        }
        else{
            ++itS;
        }
    }
    
    
}

bool CollisionController::resolveCollision( PhotonSet& pset, AsteroidSet& aset, std::shared_ptr<Ship> ship){
    bool collision = false;
//
    auto itP = pset.current.begin();
    while (itP != pset.current.end()){
        std::shared_ptr<PhotonSet::Photon> shot = *itP;
        auto itA = aset.current.begin();
        bool hitSomething = false;
        while ( itA != aset.current.end()){
            const std::shared_ptr<AsteroidSet::Asteroid>& rock = *itA;
            Vec2 norm = shot->position - rock->position;
            float distance = norm.length();
            float impactDistance =
            pset.getRadius() * shot->getScale() + aset.getRadius()*rock->getScale();
            
            // If this normal is too small, there was a collision
            auto curA = itA++;
//            if (distance < impactDistance && ship->getAbsorb()+10 > rock->getType()*8) {
            if (distance < impactDistance) {
                hitSomething = true;
                collision = true;
                rock->setHealth(rock->getHealth() - 1);
                if(rock->getHealth() <= 0){
                    ship->addAbsorb((*curA)->getAbsorbValue());
                    aset.current.erase(curA);
                }
            }
        }
        auto curP = itP++;
        if (hitSomething){
            pset.current.erase(curP);
        }
    }
//    collision = true;
    return collision;
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
            
            // This loop finds the NEAREST collision if we include wrap for the asteroid/ship
            for(int ii = -1; ii <= 1; ii++) {
                for(int jj = -1; jj <= 1; jj++) {
                    Vec2 pos = rock->position;
                    pos.x += (ii)*_size.width;
                    pos.y += (jj)*_size.height;
                    pos = base->getPos() - pos;
                    float dist = pos.length();
                    if (dist < distance) {
                        distance = dist;
                        norm = pos;
                    }
                }
            }
            
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
bool CollisionController::resolveCollision(const std::shared_ptr<Ship>& ship, AsteroidSet& aset) {
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
        
        // This loop finds the NEAREST collision if we include wrap for the asteroid/ship
        
        // If this normal is too small, there was a collision
        auto curA = it++;
        if (distance < impactDistance) {
            // "Roll back" time so that the ships are barely touching (e.g. point of impact).
            norm.normalize();
            Vec2 temp = norm * ((impactDistance - distance) / 2);
            ship->setPosition(ship->getPosition()+temp);
            rock->position = rock->position-temp;

            // Now it is time for Newton's Law of Impact.
            // Convert the two velocities into a single reference frame
            Vec2 vel = ship->getVelocity()-rock->velocity;

            // Compute the impulse (see Essential Math for Game Programmers)
            float impulse = (-(1 + COLLISION_COEFF) * norm.dot(vel)) /
                            (norm.dot(norm) * (1.0f / ship->getMass() + 1.0f / (ship->getMass()*rock->getScale())));
            if (norm.dot(norm) == 0) {
                // Just use the coefficient if the impulse is degenerate.
                impulse = COLLISION_COEFF;
            }

            // Change velocity of the two ships using this impulse
            temp = norm * (impulse/ship->getMass());
            ship->setVelocity(ship->getVelocity()+temp);

            temp = norm * (impulse/(ship->getMass()*rock->getScale()));
            rock->velocity = rock->velocity - temp;
            
            // Damage the ship as the last step
//            ship->setHealth(ship->getHealth()-aset.getDamage());
//            if(rock->getType() == 1 ){
////                ship->addAbsorb(rock->getAbsorbValue());
//                ship->addAbsorb(5);
//                aset.current.erase(curA);
//            }else if (rock->getType() == 2 && ship->getAbsorb() > 10){
//                ship->addAbsorb(5);
////                ship->addAbsorb(rock->getAbsorbValue());
//                aset.current.erase(curA);
//            }else if (rock->getType() == 3 && ship->getAbsorb() > 20){
////                ship->addAbsorb(rock->getAbsorbValue());
//                ship->addAbsorb(5);
//                aset.current.erase(curA);
//            }else{
                ship->setHealth(ship->getHealth()-rock->getDamage());
//            }
                      
        }
    }
    return collision;
}


