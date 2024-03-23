//
//  OverWorld.hpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//
#pragma once
#ifndef OverWorld_hpp
#define OverWorld_hpp
#include <cugl/cugl.h>
#include <unordered_set>
#include "Base.h"
#include "Dog.h"
#include "Devil.h"
#include "Animation.h"
#include "BaseSet.h"
#include "InputController.h"
#include "AttackPolygons.hpp"
#include "DecoySet.hpp"

class OverWorld{
private:
    std::shared_ptr<Dog> _dog;
    std::shared_ptr<Devil> _devil;
    std::shared_ptr<DecoySet> _decoys;
    std::shared_ptr<BaseSet> _bases;
    std::shared_ptr<cugl::JsonValue> _constants;
    std::shared_ptr<cugl::AssetManager> _assets;
    cugl::Size _totalSize;
    AttackPolygons _attackPolygonSet;
    
    void drawDecoy(const std::shared_ptr<cugl::SpriteBatch>& batch);
public:
    
    OverWorld(){
        
    }
    ~OverWorld(){
        
    }
    
    void reset(cugl::Size resetSize);
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size totalSize);
    bool initDog();
    bool initDevil();
    bool initBases();
    bool initDecoys();
    
    void dogUpdate(InputController& _input,cugl::Size totalSize);
    void devilUpdate(InputController& _input,cugl::Size totalSize);
    void update(InputController& input, cugl::Size totalSize, float timestep);
    void postUpdate();
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,cugl::Size totalSize);
    
    // Might want to be more specific with tile types
    enum Terrain {
        PASSABLE,
        IMPASSIBLE
    };
    
    // Information for a tile, add to this later since idk what this will include
    class TileInfo {
    public:
    private:
        Terrain type;
    };
    
    // Matrix with information about the overworld
    std::vector<std::vector<TileInfo>> overworld;
    
    std::shared_ptr<Dog> getDog() const {
        return _dog;
    }
    std::shared_ptr<Devil> getDevil()const {
        return _devil;
    }
    std::shared_ptr<DecoySet> getDecoys() const{
        return _decoys;
    }
    std::shared_ptr<BaseSet> getBaseSet(){
        return _bases;
    }
    int getTotalTargets() const {
        return 1 + (int) _bases->_bases.size() + (int) _decoys->getCurrentDecoys().size();
    }
    AttackPolygons& getAttackPolygons(){
        return _attackPolygonSet;
    }
    void setDog(std::shared_ptr<Dog> m_dog){
        _dog = m_dog;
    }
    void setDevil(std::shared_ptr<Devil> m_devil){
        _devil = m_devil;
    }
    
    cugl::Size getTotalSize() const {
        return _totalSize;
    }
    
    /**
    Get the type of terrain at the tile at (x, y)
     */
    Terrain getTerrain(int x, int y);
    
    /**
    Whether the terrain at (x, y) is passable
     */
    bool isPassable(int x, int y);
    
    /**
     Gets the width of the overworld in tile coordinates
     */
    int getRows();
    
    /**
     Gets the height of the overworld in tile coordinates
     */
    int getCols();
    
    
};
#endif /* OverWorld_hpp */
