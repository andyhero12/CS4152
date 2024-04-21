//
//  Devil.h
//  Heavan
//
//  Created by Andrew Cheng on 3/20/24.
//

#pragma once
#ifndef Devil_h
#define Devil_h

#include "NLDog.h"
#include "Animation.h"
#include <cugl/cugl.h>

class Devil{
    
private:
    std::shared_ptr<Dog> dog;
    cugl::Vec2 _pos;
    /** The number of columns in the sprite sheet */
    int _framecols;
    /** The number of frames in the sprite sheet */
    int _framesize;
    int _frameflat;
    /** Radius of the ship in pixels (derived from sprite sheet) */
    float _radius;
    
    int prevDirection;
    
    Animation runAnimation;
    
public:
    
    void setRunTexture(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    void move(cugl::Size size);
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size bounds);
    void setPosition(cugl::Vec2 value) { _pos = value; }
    
    /**
     * Creates a ship wiht the given position and data.
     *
     * The JsonValue should be a reference of all of the constants
     * that necessary to set the "hidden physical properties".
     *
     * @param pos   The ship position
     * @param data  The data defining the physics constants
     */
    Devil(std::shared_ptr<Dog> inc_dog, const cugl::Vec2& pos, std::shared_ptr<cugl::JsonValue> data);
    
    /**
     * Disposes the ship, releasing all resources.
     */
    ~Devil() {}
};
#endif /* Devil_h */
