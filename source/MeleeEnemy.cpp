//
//  MeleeEnemy.cpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#include "MeleeEnemy.hpp"
MeleeEnemy::MeleeEnemy(cugl::Vec2 m_pos, int m_health, float m_radius, int m_targetIndex)
: AbstractEnemy(m_pos, m_health, m_radius, m_targetIndex)
, _contactDamage(5)
{
    
}

void MeleeEnemy::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size,  std::shared_ptr<cugl::Font> font){
    Vec2 pos = getPos();
    
    float maxHealth = 3;
    Affine2 trans;
    trans.translate(pos);
//    trans_bar.translate(pos_bar);

    const std::shared_ptr<cugl::SpriteSheet>& sprite = getSprite();
    sprite->draw(batch, trans); // draw enemy animation
    
    Affine2 trans_bar;
    trans_bar.scale(0.1);
    Vec2 pos_bar = Vec2(pos.x + 5, pos.y + 35);
    trans_bar.translate(pos_bar);
    
    _healthBar->setProgress(getHealth()/maxHealth);
    _healthBar->render(batch, trans_bar, Color4::RED);
}

void MeleeEnemy::update(float dt, OverWorld& overWorld){
    if (_attackCooldown < 60){
        _attackCooldown++;
    }
    
    cugl::Vec2 target_pos = getTargetPositionFromIndex(overWorld);
    cugl::Vec2 direction = target_pos- position;
    // Animate
    position += direction.normalize();
    cugl::Size size = overWorld.getTotalSize();
    
    // should remove 90 when we have full 8 directions
    _walkingAnimations.update(direction.getAngle() - 90);
    
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
