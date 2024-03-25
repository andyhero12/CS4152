//
//  World.hpp
//  Heavan
//
//  Created by Henry Gao on 3/24/24.
//

#ifndef World_hpp
#define World_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include <unordered_set>
#include <vector>
//

class World{
public:
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
private:
    // Matrix with information about the overworld
    std::vector<std::vector<TileInfo>> overworld;
    std::shared_ptr<cugl::Texture> passable;
    std::shared_ptr<cugl::Texture> impassable;
    int length = 40;
    int width = 40;
    cugl::Vec2 start;
public:
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
    void setPassableTexture(const std::shared_ptr<cugl::Texture>& value);
    void setImpassableTexture(const std::shared_ptr<cugl::Texture>& value);
//    void boundMovement(cugl::Size size, cugl::Vec2 pos);
    
    
};

#endif /* World_hpp */
