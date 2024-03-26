//
//  MonsterController.h
//  Heavan
//
//  Created by Andrew Cheng on 3/21/24.
//

#ifndef MonsterController_h
#define MonsterController_h

#include "AbstractEnemy.h"
#include "MeleeEnemy.h"
#include "StaticMeleeEnemy.h"
#include "BombEnemy.h"
#include "OverWorld.h"
#include <unordered_set>
#include <vector>
#include <random>
struct AnimationDataStruct{
    std::vector<std::shared_ptr<cugl::SpriteSheet>> _sprite;
    int _framesize;
    int _framecols;
};
class MonsterController{
private:
    std::unordered_set<std::shared_ptr<AbstractEnemy>> _current;
    std::unordered_set<std::shared_ptr<AbstractEnemy>> _pending;
    // Need a Wrapper class that contains each and every Sprite
    // Each one needs its own sprite
    
    // Each Monster Type needs one of these
    AnimationDataStruct meleeAnimationData;
    AnimationDataStruct bombAnimationData;
    std::shared_ptr<cugl::scene2::ProgressBar>  _healthBar;
public:
        
    MonsterController(){
        
    }
    ~MonsterController(){
        
    }
    bool init(std::shared_ptr<cugl::JsonValue> data, OverWorld& overWorld);
    
    bool isEmpty(){
        return _current.size() == 0 && _pending.size() == 0;
    }
    void retargetToDecoy( OverWorld& overWorld);
    void retargetCloset( OverWorld& overWorld);
    void update(cugl::Size size, float timestep, OverWorld& overWorld);
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size,  std::shared_ptr<cugl::Font> font);
    
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& getEnemies(){
        return _current;
    }
    
    void spawnBasicEnemy(cugl::Vec2 pos, OverWorld& overWorld);
    void spawnStaticBasicEnemy(cugl::Vec2 pos, OverWorld& overWorld);
    void spawnBombEnemy(cugl::Vec2 pos, OverWorld& overWorld);
    void postUpdate(cugl::Size size, float timestep);
    
    void setMeleeAnimationData(std::shared_ptr<cugl::JsonValue> data,
                               const std::shared_ptr<cugl::AssetManager> _assets);
    void setBombAnimationData(std::shared_ptr<cugl::JsonValue> data,
                               const std::shared_ptr<cugl::AssetManager> _assets);
    
    void setHealthBar(std::shared_ptr<cugl::scene2::ProgressBar> bar);
    
};

#endif /* MonsterController_h */
