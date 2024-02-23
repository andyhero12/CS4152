//
//  Spawner.cpp
//  Ship
//
//  Created by Henry Gao on 2/23/24.
//

#include "Spawner.h"

Spawner::Spawner(int rate, cugl::Vec2 pos)
: _spawnRate(rate)
, _position(pos)
{
    
}

void Spawner::update(){
    if (_respawnCnt <= _spawnRate) {
        _respawnCnt++;
    }
}

