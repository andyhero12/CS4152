//
//  MeleeSpawner.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//

#include "MeleeSpawner.h"




MeleeSpawner::MeleeSpawner(int rate, cugl::Vec2 pos, int health, int delay)
:AbstractSpawner(rate, pos, health, delay)
{
    
}


void MeleeSpawner::update(MonsterController& monsterController, OverWorld& overWorld, float timestep){
    if (_respawnCnt <= _spawnRate) {
        _respawnCnt++;
    }
    _timeElapsed += timestep;
    if (canSpawn()){
        reloadSpawner();
        monsterController.spawnBasicEnemy(getPos(),overWorld);
    }
}

void MeleeSpawner::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){ 
    cugl::Vec2 pos = getPos();
    cugl::Vec2 origin(0, 0);
    cugl::Affine2 trans;
    float scale = 1;
    trans.scale(scale / _texture->getHeight());
    trans.translate(pos);
    batch->draw(_texture, origin, trans);
}
