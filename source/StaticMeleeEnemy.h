//
//  StaticMeleeEnemy.hpp
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//
#pragma once
#ifndef StaticMeleeEnemy_hpp
#define StaticMeleeEnemy_hpp

#include "MeleeEnemy.h"

class StaticMeleeEnemy: public MeleeEnemy{
    
public:
    StaticMeleeEnemy(cugl::Vec2 m_pos, int m_health, float m_radius, int m_targetIndex);
    ~StaticMeleeEnemy(){}
    
    virtual void update(float dt, OverWorld& overWorld) override;
    
    
private:
    cugl::Vec2 original_pos;
};

#endif /* StaticMeleeEnemy_hpp */
