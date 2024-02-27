//
//  Base.h
//  Ship
//
//  Created by Andrew Cheng on 2/26/24.
//
#pragma once
#ifndef Base_h
#define Base_h

#include <stdio.h>
#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>

class Base{
    
private:
    
    std::shared_ptr<cugl::Texture> _texture;
    
    int _health;
    cugl::Vec2 _position;
    
public:
    
    Base(int health, cugl::Vec2 pos);
    void update();
    void subHealth(const int val);
    
    const std::shared_ptr<cugl::Texture>& getTexture() const {
        return _texture;
    }
    
    const cugl::Vec2& getPos() const {
        return _position;
    }
};

#endif /* Base_h */
