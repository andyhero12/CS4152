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
    std::string hpMsg = strtool::format(std::to_string(getHealth()));
    std::shared_ptr<cugl::TextLayout> hptext = TextLayout::allocWithText(hpMsg, font);
    hptext->layout();
    Affine2 trans;
    trans.translate(pos);
    const std::shared_ptr<cugl::SpriteSheet>& sprite = getSprite();
    sprite->draw(batch, trans);
    batch->drawText(hptext, trans);
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
