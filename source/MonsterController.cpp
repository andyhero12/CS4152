//
//  MonsterController.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/21/24.
//
#include "MonsterController.h"



bool MonsterController::init(std::shared_ptr<cugl::JsonValue> data, const OverWorld& overWorld){
    if (data){
        _current.clear();
        _pending.clear();
        if (data->get("start")){
            auto initEnemies = data->get("start")->children();
            for (auto it = initEnemies.begin(); it != initEnemies.end(); it++){
                std::shared_ptr<JsonValue> entry = (*it);
                Vec2 pos;
                pos.x = entry->get(0)->get(0)->asFloat(0);
                pos.y = entry->get(0)->get(1)->asFloat(0);
                spawnBasicEnemy(pos, overWorld);
            }
        }
    }
    return true;
}
void MonsterController::postUpdate(cugl::Size size, float timestep){
    for (std::shared_ptr<AbstractEnemy> curEnemy: _pending){
        _current.insert(curEnemy);
    }
    _pending.clear();
}

void MonsterController::update(cugl::Size size, float timestep, const OverWorld& overWorld){
    
    for (std::shared_ptr<AbstractEnemy> curEnemy: _current){
        curEnemy->update(timestep, overWorld);
    }
    
}
void MonsterController::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size,  std::shared_ptr<cugl::Font> font){
    for (std::shared_ptr<AbstractEnemy> curEnemy: _current){
        curEnemy->draw( batch, size, font);
    }
}

void MonsterController::setMeleeAnimationData(std::shared_ptr<cugl::JsonValue> data,
                           const std::shared_ptr<cugl::AssetManager> _assets){
    int _framecols = data->getFloat("sprite cols", 0);
    int _framesize = data->getFloat("sprite size", 0);
    std::vector<std::shared_ptr<cugl::Texture>> textures;
    textures.push_back(_assets->get<Texture>("monkey0"));
    textures.push_back(_assets->get<Texture>("monkey1"));
    textures.push_back(_assets->get<Texture>("monkey2"));
    textures.push_back(_assets->get<Texture>("monkey3"));
    textures.push_back(_assets->get<Texture>("monkey4"));
    textures.push_back(_assets->get<Texture>("monkey5"));
    textures.push_back(_assets->get<Texture>("monkey6"));
    textures.push_back(_assets->get<Texture>("monkey7"));
    meleeAnimationData._texture = textures;
    meleeAnimationData._framesize = _framesize;
    meleeAnimationData._framecols = _framecols;
}

void MonsterController::spawnBasicEnemy(cugl::Vec2 pos, const OverWorld& overWorld){
    
    std::vector<std::shared_ptr<cugl::Texture>> _texture = meleeAnimationData._texture;
    int _framesize = meleeAnimationData._framesize;
    int _framecols = meleeAnimationData._framecols;
    if (_texture.size() > 0)
    {
        int rows = _framesize / _framecols;
        if (_framesize % _framecols != 0)
        {
            rows++;
        }
        float _radius = std::max(_framecols, rows) / 2;
        std::shared_ptr<MeleeEnemy> basic = std::make_shared<MeleeEnemy>(pos, 3, _radius);
        basic->setSprite(_texture, rows,_framecols, _framesize, Vec2(0, 0));
        _pending.emplace(basic);
    }
    
}
