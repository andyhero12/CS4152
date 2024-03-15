//
//  AbstractEnemy.h
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#ifndef AbstractEnemy_h
#define AbstractEnemy_h
#include <cugl/cugl.h>
#include "../OverWorld.h"
class AbstractEnemy {
public:
    
    virtual void draw() = 0;
    
    virtual void update(float dt, const OverWorld& overWorld) = 0;
    
    virtual void getDamage() = 0;
    // Virtual destructor
    virtual ~AbstractShape() {}
    
    
    cugl::Vec2 getPos() const {
        return position;
    }
    int getHealth() const {
        return _health;
    }
private:
    
    int _health;
    cugl::Vec2 position;
};
#endif /* AbstractEnemy_h */
