//
//  BaseSet.cpp
//  Ship
//
//  Created by Andrew Cheng on 2/26/24.
//
#include "BaseSet.h"

BaseSet::BaseSet(){
    CULog("HERE\n");
}
BaseSet::~BaseSet(){
    
}

void BaseSet::update(AsteroidSet &aset){
    for(auto& base : _bases) {
        base->update();
    }

}

bool BaseSet::init(std::shared_ptr<cugl::JsonValue> data) {
    if (data) {
        _bases.clear();
//        CULog("INIT\n");
        if (data->get("start")){
            auto baseValues = data->get("start")->children();
            for (auto it = baseValues.begin(); it != baseValues.end();it++){
                std::shared_ptr<cugl::JsonValue> entry = (*it);
                cugl::Vec2 pos;
                pos.x = entry->get(0)->get(0)->asFloat(0);
                pos.y = entry->get(0)->get(1)->asFloat(0);
                int health = entry->get(1)->asInt(0);
                _bases.insert(std::make_shared<Base>(health,pos));
            }
        }
        return true;
    }
    return false;
}



void BaseSet::setTexture(const std::shared_ptr<cugl::Texture>& value){
    _texture = value;
//    for(auto& spawner : _spawners) {
//        spawner->setTexture(value);
//        std::cout << spawner->getTexture()<< std::endl;
//    }
}

void BaseSet::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    for(auto& base : _bases) {
        cugl::Vec2 pos = base->getPos();
        cugl::Vec2 origin(0, 0);
        cugl::Affine2 trans;
        float scale = 2;
        trans.scale(scale);
        trans.translate(pos);
        batch->draw(_texture, origin, trans);
        
        
//        std::cout << spawner->getTexture()<< std::endl;
//        spawner->draw(batch, size);
    }
    
}
