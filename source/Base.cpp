//
//  Base.cpp
//  Ship
//
//  Created by Andrew Cheng on 2/26/24.
//

#include <stdio.h>
#include "Base.h"
Base::Base(int health, cugl::Vec2 pos)
:_health{health}
,_position{pos}
{
    
}

void Base::update(){
    
}

void Base::reduceHealth(int val){
    _health = _health - val;
}
