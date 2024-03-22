//
//  MeleeEnemy.hpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#ifndef MeleeEnemy_hpp
#define MeleeEnemy_hpp

#include <cugl/cugl.h>
#include "AbstractEnemy.h"


class MeleeEnemy : public AbstractEnemy {
    
public:
    MeleeEnemy(cugl::Vec2 m_pos, int m_health, float m_radius);
    virtual void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size,  std::shared_ptr<cugl::Font> font) override;
    
    virtual void update(float dt, const OverWorld& overWorld) override;
    
    virtual int getDamage() override{
        if (canAttack()){
            _attackCooldown = 0;
            return _contactDamage;
        }
        return 0;
    }
    bool canAttack() const {
        return _attackCooldown == 60;
    }
    virtual int getAbsorbValue() const override{
        CULog("TODO ABSORB MELEE\n");
        return 1;
    }
    virtual ~MeleeEnemy() {
        
    }
private:
    
    int _contactDamage;
    int _attackCooldown;
};
#endif /* MeleeEnemy_hpp */
