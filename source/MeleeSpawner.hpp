//
//  MeleeSpawner.hpp
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//

#ifndef MeleeSpawner_hpp
#define MeleeSpawner_hpp

#include "AbstractSpawner.h"
class MeleeSpawner: public AbstractSpawner{
    
public:
    MeleeSpawner(int rate, cugl::Vec2 pos, int health, int delay);
    virtual void update(MonsterController& monsterController, OverWorld& overWorld, float timestep) override;
    virtual void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size) override;
};

#endif /* MeleeSpawner_hpp */
