//
//  BaseSet.h
//
//  Created by Andrew Cheng on 2/26/24.
//
#pragma once
#ifndef BaseSet_h
#define BaseSet_h

#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>
#include "Base.h"


class BaseSet{
public:
//    /* Base Textures */
    std::shared_ptr<cugl::Texture> _texture;
//
//
//    /* Set of bases */
    std::vector<std::shared_ptr<Base>> _bases;
//
    BaseSet();
    
    ~BaseSet();

    void setTexture(const std::shared_ptr<cugl::Texture>& value);
    
    void update();
    
    bool init(std::shared_ptr<cugl::JsonValue> data);
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    
    bool baseLost();
    
    int getFirstHealth();
private:
    

};

#endif /* BaseSet_h */
