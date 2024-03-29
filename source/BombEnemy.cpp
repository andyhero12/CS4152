//
//  BombEnemy.cpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//
#include "BombEnemy.h"

#define EXPLOSION_RADIUS 0.5

BombEnemy::BombEnemy(cugl::Vec2 m_pos, int m_health, float m_radius, int m_targetIndex)
: AbstractEnemy(m_pos, m_health, m_radius, m_targetIndex)
, _contactDamage(4)
, _baseExplosionDamage(11)
{
    
}

void BombEnemy::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size,  std::shared_ptr<cugl::Font> font){
    Vec2 pos = getPos();
    Affine2 trans;
    auto sprite = getSprite();
    trans.scale(1 / sprite->getFrameSize().height);
    trans.translate(pos);
    sprite->draw(batch, trans);
    
    // Might need to change; this was specifically for the default health bar
    Affine2 trans_bar;
    trans_bar.scale(0.1);
    Vec2 pos_bar = Vec2(pos.x + 5, pos.y + 35);
    trans_bar.translate(pos_bar);
    
    _healthBar->setProgress(getHealth()/(float)_maxHealth);
    _healthBar->render(batch, trans_bar, Color4::RED);
}

void BombEnemy::update(float dt, OverWorld& overWorld){
    if (_attackCooldown < 60){
        _attackCooldown++;
    }
    
    cugl::Vec2 target_pos = getTargetPositionFromIndex(overWorld);
    cugl::Vec2 direction = target_pos- position;
    // Animate
    position += direction.normalize() * 0.03;
    cugl::Size size = overWorld.getTotalSize();
    
    _walkingAnimations.update(direction.getAngle());
    
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
void BombEnemy::executeDeath(OverWorld& overWorld){
    
    std::shared_ptr<Dog> curDog = overWorld.getDog();
    cugl::Vec2 dog_pos = curDog->getPosition();
    
    float distance = (dog_pos - getPos()).length();
    if (distance > EXPLOSION_RADIUS){
        return;
    }
    curDog->setHealth(curDog->getHealth() - getExplosionDamage());
    CULog("Dog got exploded on\n");
}

