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
    
    virtual ~BombEnemy() {
        
    }
    virtual void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size,  std::shared_ptr<cugl::Font> font) override;
    
    virtual void update(float dt, OverWorld& overWorld) override;
    
    virtual int getDamage() override{
        return contactDamage;
    }
    virtual int getAbsorbValue() const override{
        CULog("TODO ABSORB BOMB\n");
        return 1;
    }
    
    
private:
    
    int contactDamage;
};
#endif /* BombEnemy_hpp */
