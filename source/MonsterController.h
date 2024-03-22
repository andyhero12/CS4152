//
//  MonsterController.h
//  Heavan
//
//  Created by Andrew Cheng on 3/21/24.
//

#ifndef MonsterController_h
#define MonsterController_h

#include "AbstractEnemy.h"
#include "MeleeEnemy.hpp"
#include "OverWorld.hpp"
#include <unordered_set>
#include <vector>
struct AnimationDataStruct{
    std::vector<std::shared_ptr<cugl::Texture>> _texture;
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
public:
        
    MonsterController(){
        
    }
    ~MonsterController(){
        
    }
    bool init(std::shared_ptr<cugl::JsonValue> data, const OverWorld& overWorld);
    
    bool isEmpty(){
        return _current.size() == 0 && _pending.size() == 0;
    }
    void update(cugl::Size size, float timestep, const OverWorld& overWorld);
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size,  std::shared_ptr<cugl::Font> font);
    
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& getEnemies(){
        return _current;
    }
    
    void spawnBasicEnemy(cugl::Vec2 pos, const OverWorld& overWorld);
    
    void postUpdate(cugl::Size size, float timestep);
    
    void setMeleeAnimationData(std::shared_ptr<cugl::JsonValue> data,
                               const std::shared_ptr<cugl::AssetManager> _assets);
};

#endif /* MonsterController_h */
