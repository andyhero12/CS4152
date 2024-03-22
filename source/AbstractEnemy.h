//
//  AbstractEnemy.h
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#ifndef AbstractEnemy_h
#define AbstractEnemy_h
#include <cugl/cugl.h>
#include "OverWorld.hpp"
#include "Animation.h"

class AbstractEnemy {
public:
    
    virtual void draw() = 0;
    
    virtual void update(float dt, const OverWorld& overWorld) = 0;
    
    virtual int getDamage() = 0;
    
    // Virtual destructor
    virtual ~AbstractEnemy() {}
    
    
    cugl::Vec2 getPos() const {
        return position;
    }
    int getHealth() const {
        return _health;
    }
    
    void setAnimation(){
        
    }
private:
    
    int _health;
    cugl::Vec2 position;
    Animation _animations;
};
#endif /* AbstractEnemy_h */
