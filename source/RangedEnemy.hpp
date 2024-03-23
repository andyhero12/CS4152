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
    virtual void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size,  std::shared_ptr<cugl::Font> font) override;
    
    virtual void update(float dt, OverWorld& overWorld) override;
    
    virtual int getDamage() override{
        return rangedDamage;
    }
    virtual int getAbsorbValue() const override{
        CULog("TODO ABSORB RANGE\n");
        return 1;
    }
    virtual ~RangedEnemy() {
        
    }
    
private:
    
    int rangedDamage;
};
#endif /* RangedEnemy_hpp */
