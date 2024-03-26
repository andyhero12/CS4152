//
//  AttackPolygons.hpp
//  Dog
//
//  Created by Andrew Cheng on 3/12/24.
//
#pragma once
#ifndef AttackPolygons_hpp
#define AttackPolygons_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <unordered_set>
#include "Dog.h"
using namespace cugl;
enum class Action : unsigned int {
    BITE = 0,
    SHOOT = 1,
    EXPLODE = 2
};

class ActionPolygon{
public:
    Action polygonAction;
    Poly2 internalPolygon;
    int _age;
    int _maxage;
    float _scale;
    
    ActionPolygon(Action curAction, Poly2& mintPoly, int mx);
    
    float getScale() const { return _scale; }
    
    int getAge() const { return _age; }
    Action getAction() const {return polygonAction;}
    const Poly2& getPolygon() const {return internalPolygon;}
    bool expired() const { return _age >= _maxage;}
    
    void update(cugl::Size size);
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    
private:
    void drawShoot(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    void drawExplode(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    void drawBite(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
};

class AttackPolygons{
public:
    // Have AttackPolygon Set hold all possible textures for bite, shoot, explode
    int max_age;
    std::unordered_set<std::shared_ptr<ActionPolygon>> currentAttacks;
    AttackPolygons();
    bool init();
    bool isEmpty() const { return currentAttacks.empty(); }
    void update(cugl::Size size);
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
    void addShoot(const std::shared_ptr<Dog>& ship);
    void addExplode(const std::shared_ptr<Dog>& ship);
    void addBite(const std::shared_ptr<Dog>& ship);
private:
    
};
#endif /* AttackPolygons_hpp */
