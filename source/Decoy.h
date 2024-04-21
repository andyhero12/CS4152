//
//  Decoy.hpp
//  ship-mac
//
//  Created by Lisa Asriev on 3/7/24.
//

#ifndef Decoy_h
#define Decoy_h

#include "NLDog.h"
#include <stdio.h>
#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>


class Decoy{
    
private:
    
    std::shared_ptr<cugl::Texture> _texture;
    std::shared_ptr<cugl::scene2::SceneNode> drawNode;
    int _health;
    float _time_duration;
    cugl::Vec2 _position;
    
public:
    void setSceneNode(std::shared_ptr<cugl::scene2::SceneNode> incomingNode){
        drawNode = incomingNode;
    }
    std::shared_ptr<cugl::scene2::SceneNode> getSceneNode() const{
        return drawNode;
    }
    Decoy(cugl::Vec2 pos);
    
    void update(float timestep);
    bool destroyed(){
        return _health <= 0 || _time_duration <= 0;
    }
    const std::shared_ptr<cugl::Texture>& getTexture() const {
        return _texture;
    }
    
    const cugl::Vec2& getPos() const {
        return _position;
    }
    const int getHealth() const{
        return _health;
    }
    const float getTime() const{
        return _time_duration;
    }
    void subHealth(int amt) {
        _health -= amt;
        _health = fmax(_health, 0);
    }

};



#endif /* Decoy_hpp */
