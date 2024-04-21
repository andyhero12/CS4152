//
//  Base.cpp
//
//  Created by Andrew Cheng on 2/26/24.
//

#include <stdio.h>
#include "Base.h"
Base::Base(int health, cugl::Vec2 pos)
:_health{health}
,_position{pos}
,_healVal{5}
{
}

void Base::update(){
}

void Base::reduceHealth(int val){
    _health = _health - val;
}

