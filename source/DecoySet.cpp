//
//  DecoySet.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/22/24.
//

#include "DecoySet.h"

void DecoySet::init(){
    _currentDecoys.clear();
    _pendingDecoys.clear();
    _removedDecoys.clear();
}

void DecoySet::update(float timestep){
    auto itD = _currentDecoys.begin();
    while (itD != _currentDecoys.end()){
        std::shared_ptr<Decoy> curDecoy = *itD;
        curDecoy->update(timestep);
        if (curDecoy->destroyed()){
            _removedDecoys.emplace_back(curDecoy);
            itD = _currentDecoys.erase(itD);
        }else {
            ++itD;
        }
    }
}

void DecoySet::draw(const std::shared_ptr<cugl::SpriteBatch>& batch){
    for (auto& decoy : _currentDecoys){
        cugl::Vec2 pos = decoy->getPos();
        cugl::Vec2 origin(0, 0);
        cugl::Affine2 trans;
        trans.translate(pos);
        batch->draw(_texture,origin,trans);
    }
}
void DecoySet::setTexture(const std::shared_ptr<cugl::Texture> &value){
    _texture = value;
}

void DecoySet::addNewDecoy(cugl::Vec2 pos){
    _pendingDecoys.emplace_back(std::make_shared<Decoy>(pos));
}
void DecoySet::postUpdate(){
    for (auto& decoy : _pendingDecoys){
        _currentDecoys.emplace_back(decoy);
    }
    _pendingDecoys.clear();
    _removedDecoys.clear();
}
