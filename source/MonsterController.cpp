//
//  MonsterController.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/21/24.
//
#include "MonsterController.h"


int generateRandomInclusiveHighLow(int low, int high)
{
    // Static used for the seed to ensure it's only seeded once
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high); // Range is 1 to 3, inclusive
    return dis(gen);
}

bool MonsterController::init(std::shared_ptr<cugl::JsonValue> data, OverWorld& overWorld){
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

void MonsterController::retargetToDecoy( OverWorld& overWorld){
    int totalTargets = overWorld.getTotalTargets(); // really brittle be careful
    for (std::shared_ptr<AbstractEnemy> enemy : getEnemies()){
        enemy->setTargetIndex(totalTargets); // will add the pending decoy to real next iteration
    }
}
void MonsterController::retargetCloset( OverWorld& overWorld){
    cugl::Vec2 dogPos = overWorld.getDog()->getPosition();
    int baseSize = (int) overWorld.getBaseSet()->_bases.size();
    int decoySize = (int) overWorld.getDecoys()->getCurrentDecoys().size();
    for (std::shared_ptr<AbstractEnemy> enemy : getEnemies()){
        Vec2 enemyPos = enemy->getPos();
        int index = 0;
        float dist = (enemyPos - dogPos).length();
        int sizeBases = baseSize;
        for (int i = 0 ;i < sizeBases;i++){
            Vec2 basePos = overWorld.getBaseSet()->_bases[i]->getPos();
            float curDist = (enemyPos -  basePos).length();
            if (curDist < dist){
                dist = curDist;
                index = i + 1;
            }
        }
        int sizeDecoys = decoySize;
        for (int i = 0 ;i < sizeDecoys; i++){
            Vec2 decoyPos = overWorld.getDecoys()->getCurrentDecoys()[i]->getPos();
            float curDist = (enemyPos -  decoyPos).length();
            if (curDist < dist){
                dist = curDist;
                index = i + 1 + sizeBases;
            }
        }
        enemy->setTargetIndex(index);
    }
}
void MonsterController::update(cugl::Size size, float timestep, OverWorld& overWorld){
    std::shared_ptr<DecoySet> decoySet = overWorld.getDecoys();
    if (decoySet->addedNewDecoy()){
        retargetToDecoy(overWorld);
        return;
    }
    if (decoySet->removedDecoy()){
        retargetCloset(overWorld);
        return;
    }
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

void MonsterController::spawnBasicEnemy(cugl::Vec2 pos, OverWorld& overWorld){
    
    std::vector<std::shared_ptr<cugl::Texture>> _texture = meleeAnimationData._texture;
    int _framesize = meleeAnimationData._framesize;
    int _framecols = meleeAnimationData._framecols;
    int numTargets =  overWorld.getTotalTargets();
    int chosenTarget = generateRandomInclusiveHighLow(0, numTargets-1);
    if (_texture.size() > 0)
    {
        int rows = _framesize / _framecols;
        if (_framesize % _framecols != 0)
        {
            rows++;
        }
        float _radius = std::max(_framecols, rows) / 2;
        std::shared_ptr<MeleeEnemy> basic = std::make_shared<MeleeEnemy>(pos, 3, _radius, chosenTarget);
        basic->setSprite(_texture, rows,_framecols, _framesize, Vec2(0, 0));
        _pending.emplace(basic);
    }
    
}