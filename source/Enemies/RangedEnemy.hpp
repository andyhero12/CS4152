//
//  RangedEnemy.hpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#ifndef RangedEnemy_hpp
#define RangedEnemy_hpp

#include "AbstractEnemy.h"
class RangedEnemy : public AbstractEnemy {
public:
    virtual void draw() override;
    
    virtual void update(float dt, const OverWorld& overWorld) override;
    
    virtual int getDamage() override{
        return rangedDamage;
    }
    virtual ~RangedEnemy() {
        
    }
    
private:
    
    int rangedDamage;
};
#endif /* RangedEnemy_hpp */
