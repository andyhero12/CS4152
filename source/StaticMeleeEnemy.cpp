//
//  StaticMeleeEnemy.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//

#include "StaticMeleeEnemy.h"
#define DISTANCE_CUTOFF 5

StaticMeleeEnemy::StaticMeleeEnemy(cugl::Vec2 m_pos, int m_health, float m_radius, int m_targetIndex)
: MeleeEnemy(m_pos, m_health, m_radius, m_targetIndex)
, original_pos{m_pos}
{
    
}
void StaticMeleeEnemy::update(float dt, OverWorld& overWorld){
    if (_attackCooldown < 60){
        _attackCooldown++;
    }
    cugl::Vec2 dog_pos = overWorld.getDog()->getPosition();
    
    
    cugl::Vec2 org_dist = dog_pos - original_pos;
    float distance = org_dist.length();
    cugl::Vec2 direction;
    if (distance > DISTANCE_CUTOFF){ // too far from origin return
        direction = original_pos - position;
        if (direction.lengthSquared() >= 1){
            position += direction.normalize() * 0.03;
        }
    }else{ // chase dog
        direction = dog_pos - position;
        position += direction.normalize() * 0.03;
    }
    // Animate
    cugl::Size size = overWorld.getTotalSize();
    _walkingAnimations.update(direction.getAngle() + 67.5f);
    
    while (position.x > size.width) {
        position.x = size.width;
    }
    while (position.x < 0) {
        position.x = 0;
    }
    while (position.y > size.height) {
        position.y = size.height;
    }
    while (position.y < 0) {
        position.y = 0;
    }
}
