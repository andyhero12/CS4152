//
//  BombEnemy.hpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#ifndef BombEnemy_hpp
#define BombEnemy_hpp

#include "AbstractEnemy.h"
class BombEnemy : public AbstractEnemy {
public:
    virtual void draw() override;
    
    virtual void update(float dt, const OverWorld& overWorld) override;
    
    virtual int getDamage() override{
        return contactDamage;
    }
    virtual ~BombEnemy() {
        
    }
    
private:
    
    int contactDamage;
};
#endif /* BombEnemy_hpp */
