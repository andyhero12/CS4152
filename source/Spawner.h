//
//  Spawner.hpp
//  Ship
//
//  Created by Henry Gao on 2/23/24.
//
#pragma once
#ifndef Spawner_hpp
#define Spawner_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>

class Spawner{
    
private:
    // Will include a spawner texture and a monster texture
    
    std::shared_ptr<cugl::Texture> _texture;
    int _spawnRate;
    int _respawnCnt;
    int _health;
    double _scale;
    double _timeElapsed;
    
    cugl::Vec2 _position;
    
public:
    
    Spawner(int rate, cugl::Vec2 pos, int health);
    void update(float timestep);
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    
    const std::shared_ptr<cugl::Texture>& getTexture() const {
        return _texture;
    }
    void setTexture(const std::shared_ptr<cugl::Texture>& value){
        _texture = value;
    }
    
    double getTimeElapsed() const{
        return _timeElapsed;
    }
    bool canSpawn() const {
        return (_respawnCnt > _spawnRate);
    }
    void reloadSpawner() {
        _respawnCnt = 0;
    }
    const int getCnt() const {
        return _respawnCnt;
    }
    cugl::Vec2 getPos() const {
        return _position;
    }
    void subHealth(const int val);
    bool dead(){return _health <= 0;}
};

#endif /* Spawner_hpp */

