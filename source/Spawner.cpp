//
//  Spawner.cpp
//  Dog
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
//
//void Spawner::setTexture(const std::shared_ptr<cugl::Texture>& value){
//    if (value){
//        std::cout << _texture << std::endl;
//        _texture = value;
//        std::cout << _texture << std::endl;
//        std::cout << value << std::endl;
//        CULog("Assigned texture");
//    }else {
//        _texture = nullptr;
//    }
//    if(_texture){
//        CULog(" texture exist");
//    }
//}
//
//void Spawner::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
//    
//    CULog("Calling draw");
//    std::cout << _texture << std::endl;
//    if(_texture){
//        CULog(" texture exist");
//       Affine2 trans;
//       trans.translate(_position);
//   
//       Vec2 v = Vec2(0,0) ;
//       batch->draw(_texture, v, trans);
//        
//    }
//    
//}
