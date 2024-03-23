//
//  AbstractEnemy.h
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#ifndef AbstractEnemy_h
#define AbstractEnemy_h
#include <cugl/cugl.h>
#include "OverWorld.hpp"
#include "Animation.h"

class AbstractEnemy {
public:
    
    
    AbstractEnemy(cugl::Vec2 m_pos, int m_health, float m_radius, int m_targetIndex)
    : position{m_pos}
    , _health{m_health}
    , _radius{m_radius}
    , targetIndex{m_targetIndex}
    {
        
    }
    virtual void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size,  std::shared_ptr<cugl::Font> font) = 0;
    
    virtual void update(float dt, OverWorld& overWorld) = 0;
    
    virtual int getDamage() = 0;
    virtual bool canAttack() const = 0;
    virtual void resetAttack() = 0;
    virtual int getAbsorbValue() const = 0;
    // Virtual destructor
    virtual ~AbstractEnemy() {}
    
    cugl::Vec2 getPos() const {
        return position;
    }
    void setPos(cugl::Vec2 m_pos){
        position = m_pos;
    }
    int getTargetIndex() const{
        return targetIndex;
    }
    void setTargetIndex(int index){
        targetIndex = index;
    }
    int getHealth() const {
        return _health;
    }
    void setHealth(int m_health) {
        _health = m_health;
    }
    int getRadius() const{
        return _radius;
    }
    
    cugl::Vec2 getTargetPositionFromIndex(OverWorld& overWorld){
        const std::shared_ptr<Dog>& curDog = overWorld.getDog();
        const std::shared_ptr<BaseSet> baseSet = overWorld.getBaseSet();
        const std::shared_ptr<DecoySet> decoySet = overWorld.getDecoys();
        cugl::Vec2 target_pos;
        int _targetIndex  = getTargetIndex();
        if (_targetIndex == 0){
            target_pos = curDog->getPosition();
        }else if (_targetIndex <= baseSet->_bases.size()){
            target_pos = baseSet->_bases[_targetIndex-1]->getPos();
        }else{
            target_pos = decoySet->getCurrentDecoys()[_targetIndex-1-baseSet->_bases.size()]->getPos();
        }
        return target_pos;
    }
    void setSprite(const std::vector<std::shared_ptr<cugl::Texture>>& value, int rows, int _framecols, int _framesize, cugl::Vec2 origin ){

        std::vector<std::shared_ptr<cugl::SpriteSheet>> anims;
        for(auto& text : value) {

            std::shared_ptr<cugl::SpriteSheet> _sprite = SpriteSheet::alloc(text, rows, _framecols, _framesize);
            anims.push_back(_sprite);
        }
        _animations.setOrigin(origin);
        _animations = Animation(anims, 10, 0);
    }
    
    const std::shared_ptr<cugl::SpriteSheet>& getSprite() const {
        return _animations.getSprite();
    }
    
protected:
    int _health;
    int targetIndex;
    cugl::Vec2 position;
    Animation _animations;
    float _radius;
};
#endif /* AbstractEnemy_h */
