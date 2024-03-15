//
//  MeleeEnemy.hpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#ifndef MeleeEnemy_hpp
#define MeleeEnemy_hpp

#include <stdio.h>

#endif /* MeleeEnemy_hpp */


class MeleeEnemy : public AbstractEnemy {
public:
    virtual void draw() override;
    
    virtual void update(float dt, const OverWorld& overWorld) override;
    
    virtual void getDamage() override{
        return contactDamage;
    }
    // Virtual destructor
    virtual ~AbstractShape() {}
    
    
    cugl::Vec2 getPos() const {
        return position;
    }
    int getHealth() const {
        return _health;
    }
    
private:
    
    int contactDamage;
};
