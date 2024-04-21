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
#include "SpawnerEnemy.h"
#include "AbsorbEnemy.h"
#include "BombEnemy.h"
#include "OverWorld.h"
#include <unordered_set>
#include <vector>
#include <random>
struct AnimationDataStruct{
    std::vector<std::shared_ptr<cugl::Texture>> _textures;
    std::vector<std::shared_ptr<cugl::Texture>> _attackTextures;
    int _framesize;
    int _framecols;
    int _freqAnimations;
};
class MonsterController{
private:
    
    std::shared_ptr<StaticMeleeFactory> _staticMeleeFactory;
    Uint32 _staticMeleeFactID;
    
    std::shared_ptr<MeleeFactory> _meleeFactory;
    Uint32 _meleeFactID;
    
    std::shared_ptr<BombFactory> _bombEnemyFactory;
    Uint32 _bombEnemyFactID;
    
    std::shared_ptr<SpawnerEnemyFactory> _spawnerEnemyFactory;
    Uint32 _spawnerEnemyFactID;
    
    std::shared_ptr<AbsorbFactory> _absorbEnemyFactory;
    Uint32 _absorbEnemyFactID;
    
    std::unordered_set<std::shared_ptr<AbstractEnemy>> _current;
    std::unordered_set<std::shared_ptr<AbstractEnemy>> _pending;
    std::unordered_set<std::shared_ptr<AbsorbEnemy>> _absorbEnem;
    std::shared_ptr<NetEventController> _network;
    std::shared_ptr<cugl::scene2::SceneNode> _debugNode;
    // Need a Wrapper class that contains each and every Sprite
    // Each one needs its own sprite
    
    // Each Monster Type needs one of these
    AnimationDataStruct meleeAnimationData;
    AnimationDataStruct bombAnimationData;
    AnimationDataStruct absorbAnimationData;
    std::shared_ptr<cugl::scene2::ProgressBar>  _healthBar;
    
    std::shared_ptr<cugl::scene2::SceneNode> monsterControllerSceneNode;
public:
    
    void removeEnemy(std::shared_ptr<AbstractEnemy> enemy);
    void setNetwork(std::shared_ptr<NetEventController> network){
        _network = network;
    }
    std::shared_ptr<NetEventController> getNetwork(){
        return _network;
    }
    MonsterController(){
        
    }
    ~MonsterController(){
        
    }
    bool init(OverWorld& overWorld,
              std::shared_ptr<cugl::scene2::SceneNode> _debugNode);
    
    bool isEmpty(){
        return _current.size() == 0 && _pending.size() == 0;
    }
    void retargetToDecoy( OverWorld& overWorld);
    void retargetCloset( OverWorld& overWorld);
    void update( float timestep, OverWorld& overWorld);
    
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& getEnemies(){
        return _current;
    }
    std::unordered_set<std::shared_ptr<AbsorbEnemy>>& getAbsorbEnemies(){
        return _absorbEnem;
    }
    void spawnBasicEnemy(cugl::Vec2 pos, OverWorld& overWorld, float power);
    void spawnStaticBasicEnemy(cugl::Vec2 pos, OverWorld& overWorld, float power);
    void spawnBombEnemy(cugl::Vec2 pos, OverWorld& overWorld, float power);
    
    void spawnSpawnerEnemy(cugl::Vec2 pos, OverWorld& overWorld, float power);
    
    void spawnAbsorbEnemy(cugl::Vec2 pos, OverWorld& overWorld, float power);
    
    void postUpdate();
    
    void setAbsorbAnimationData(std::shared_ptr<cugl::JsonValue> data,
                                std::shared_ptr<cugl::AssetManager> _assets);
    void setMeleeAnimationData(std::shared_ptr<cugl::JsonValue> data,
                            std::shared_ptr<cugl::AssetManager> _assets);
    void setBombAnimationData(std::shared_ptr<cugl::JsonValue> data,
                            std::shared_ptr<cugl::AssetManager> _assets);
    
    void setSpawnerAnimationData(std::shared_ptr<cugl::JsonValue> data,
                                 std::shared_ptr<cugl::AssetManager> _assets);
    void setHealthBar(std::shared_ptr<cugl::scene2::ProgressBar> bar);
    
    void powerSize(float power, Size& size);

    float powerHealth(float power, int hp);
};

#endif /* MonsterController_h */
