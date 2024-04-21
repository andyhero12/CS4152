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
#include "NLDog.h"
#include "SpriteAnimationNode.h"


using namespace cugl;
enum class Action : unsigned int {
    BITE = 0,
    SHOOT = 1,
    EXPLODE = 2
};

class ActionPolygon{
    
private:
    std::shared_ptr<SpriteAnimationNode> spriteActionNode;
    std::shared_ptr<cugl::scene2::PolygonNode> polyActionNode;
public:
    Action polygonAction;
    Poly2 internalPolygon;
    Vec2 _center;
    int _age;
    int _maxage;
    float _scale;
    float _ang;
    bool _polygon;
    bool _expired;
    int _freq;
    
    ActionPolygon(Action curAction, Poly2& mintPoly, int mx, float scale, float ang, Vec2 center);
    ActionPolygon(std::shared_ptr<SpriteAnimationNode> actionSprite, Action curAction, Poly2& mintPoly, int mx, float scale, float ang, Vec2 center);
    
    float getScale() const { return _scale; }
    
    int getAge() const { return _age; }
    Action getAction() const {return polygonAction;}
    const Poly2& getPolygon() const {return internalPolygon;}
    
    std::shared_ptr<cugl::scene2::SceneNode> getPolyNode(){ return polyActionNode; }
    bool expired() const { return _expired;}
    void update();
    std::shared_ptr<cugl::scene2::SceneNode> getActionNode();
    std::shared_ptr<SpriteAnimationNode> getSpriteNode(){
        return spriteActionNode;
    }
    bool dealDamage();
    float getAngle() const { return _ang;}
    Vec2 getCenter() const { return _center;}
};

class AttackPolygons{
    
private:
    std::shared_ptr<cugl::scene2::SceneNode> frontAttackPolygonNode;
    std::shared_ptr<cugl::scene2::SceneNode> backAttackPolygonNode;
    std::shared_ptr<cugl::Texture> biteRightTexture;
    std::shared_ptr<cugl::Texture> biteLeftTexture;
    std::shared_ptr<cugl::Texture> biteFrontTexture;
    std::shared_ptr<cugl::Texture> biteBackTexture;
    float zorder;
public:
    // Have AttackPolygon Set hold all possible textures for bite, shoot, explode
    int max_age;
    std::unordered_set<std::shared_ptr<ActionPolygon>> currentAttacks;
    AttackPolygons();
    bool init();
    bool setTexture(const std::shared_ptr<cugl::Texture> &biteL, const std::shared_ptr<cugl::Texture> &biteR, const std::shared_ptr<cugl::Texture> &biteF, const std::shared_ptr<cugl::Texture> &biteB);
    bool isEmpty() const { return currentAttacks.empty(); }
    void update();
    void addShoot(Vec2 center, float angle, float shootRadius);
    void addExplode(cugl::Vec2 center, float explosionRadius);
    void addBite(cugl::Vec2 center, float angle, float explosionRad, float scale);
    std::shared_ptr<cugl::scene2::SceneNode> getFrontAttackPolygonNode(){
        return frontAttackPolygonNode;
    }
    std::shared_ptr<cugl::scene2::SceneNode> getBackAttackPolygonNode(){
        return backAttackPolygonNode;
    }
    int getZOrder() const { return zorder; }

};
#endif /* AttackPolygons_hpp */
