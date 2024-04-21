//
//  AbstractSpawner.h
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//

#ifndef AbstractSpawner_h
#define AbstractSpawner_h
#include <cugl/cugl.h>
#include "MonsterController.h"
#include "OverWorld.h"

class AbstractSpawner{
protected:
    std::shared_ptr<cugl::scene2::PolygonNode> polyNode;
    float _regularDelay;
    float _accumulatedDelay;
    int _health;
    float _delay;
    float _timeElapsed;
    cugl::Vec2 _position;
    
public:

    std::shared_ptr<cugl::scene2::SceneNode> getSpawnerNode(){
        return polyNode;
    }
    void setSpawnerNode(std::shared_ptr<cugl::scene2::PolygonNode>  inc){
        polyNode = inc;
    }
    AbstractSpawner(float regularDelay, cugl::Vec2 pos, int health, float delay)
    : _regularDelay(regularDelay)
    , _position(pos)
    , _health(health)
    , _timeElapsed(0.0)
    , _accumulatedDelay(-delay + regularDelay)
    , _delay{delay}
    {
    }
    
    float getTimeElapsed() const{
        return _timeElapsed;
    }

    void updateTime(float dt){
        _accumulatedDelay += dt;
        _timeElapsed += dt;
    }

    bool canSpawn() const {
        return (_accumulatedDelay > _regularDelay);
    }
    void reloadSpawner() {
        _accumulatedDelay = 0;
    }
    void setSceneNode(std::shared_ptr<cugl::Texture> _texture){
        polyNode = cugl::scene2::PolygonNode::allocWithTexture(_texture);
        polyNode->setContentSize(Vec2(1, 1));
        polyNode->setScale(cugl::Size(1,1)/48);
    }
    const float getAccumulatedDelay() const {
        return _accumulatedDelay;
    }
    cugl::Vec2 getPos() const {
        return _position; }
    void subHealth(const int val) {
        _health -= val;
        cout << _health;
    }
    bool dead(){
        return _health <= 0;
    }
    virtual void update(MonsterController& monsterController, OverWorld& overWorld, float timestep, float difficulty) = 0;
    
};
#endif /* AbstractSpawner_h */
