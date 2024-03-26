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
        cugl::Rect boundaryRect;
        // NULL POINTER ALERT!
        std::shared_ptr<cugl::physics2::BoxObstacle> boxObstacle;
        Terrain type;
        TileInfo(cugl::Size size, Terrain type, std::shared_ptr<cugl::Texture> texture);
    private:
        
        
    };
private:
    // Matrix with information about the overworld
    std::shared_ptr<cugl::Texture> tile;
    cugl::Vec2 start;
    
public:
    std::vector<std::vector<TileInfo>> overworld;
    World () {};
    World (cugl::Vec2 bottomleft, std::vector<std::vector<int>> &map, std::vector<std::vector<int>> &passable, std::shared_ptr<cugl::Texture> tileset, std::shared_ptr<cugl::physics2::ObstacleWorld> physicsworld);
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
    std::shared_ptr<cugl::Texture> getBox(int position);

};

#endif /* World_hpp */
