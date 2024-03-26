//
//  Spawner.cpp
//
//  Created by Henry Gao on 2/23/24.
//

#include "Spawner.h"
using namespace cugl;
Spawner::Spawner(int rate, cugl::Vec2 pos, int health )
: _spawnRate(rate)
, _position(pos)
, _health(health)
, _scale(1.0)
, _timeElapsed(0.0)
{
    
}

void Spawner::update(float timestep){
    if (_respawnCnt <= _spawnRate) {
        _respawnCnt++;
    }
    _timeElapsed += timestep;
}

void Spawner::subHealth(const int val){
    _health = _health - val;
}

void Spawner::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    if (_texture){
        cugl::Vec2 pos = getPos();
        //pos.add(-_texture->getWidth() / 2, -_texture->getHeight() / 2);
        cugl::Vec2 origin(0, 0);
        cugl::Affine2 trans;
        float scale = 1;
        trans.scale(scale / _texture->getHeight());
        trans.translate(pos);
        batch->draw(_texture, origin, trans);
    }
}
