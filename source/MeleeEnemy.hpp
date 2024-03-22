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
    virtual void draw() override;
    
    virtual void update(float dt, const OverWorld& overWorld) override;
    
    virtual int getDamage() override{
        return contactDamage;
    }
//    virtual bool canAttack() override{
//        return
//    }
    virtual ~MeleeEnemy() {
        
    }
    
private:
    
    int contactDamage;
//    int
};
#endif /* MeleeEnemy_hpp */
