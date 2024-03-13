//
//  Decoy.cpp
//  ship-mac
//
//  Created by Lisa Asriev on 3/7/24.
//

#include "Decoy.hpp"
#define DECOY_DURATION 3
Decoy::Decoy(cugl::Vec2 pos)
: _position{pos}
, _health{15}
, _time_duration{DECOY_DURATION}
{
    
}

void Decoy::update(float timestep){
    _time_duration -= timestep;
    _time_duration = fmax(_time_duration, 0.0);
}
