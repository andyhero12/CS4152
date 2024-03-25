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
        cugl::Size size;
        std::shared_ptr<cugl::Texture> texture;
        TileInfo(cugl::Size size, Terrain type, std::shared_ptr<cugl::Texture> texture);
    private:
        Terrain type;
    };
private:
    // Matrix with information about the overworld
    std::vector<std::vector<TileInfo>> overworld;
    std::shared_ptr<cugl::Texture> tile;
    cugl::Vec2 start;
    
public:
    World () {};
    World (cugl::Vec2 bottomleft, std::vector<std::vector<int>> &map, std::vector<std::vector<int>> &passable, std::shared_ptr<cugl::Texture> tileset);
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
    std::shared_ptr<cugl::Texture> getBox(int position);

};

#endif /* World_hpp */
