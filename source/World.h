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
#include "NLLevelModel.h"
//
// Might want to be more specific with tile types
enum Terrain {
    PASSABLE,
    IMPASSIBLE
};

// Information for a tile, add to this later since idk what this will include
class TileInfo : public cugl::physics2::BoxObstacle{
public:
#pragma mark Static Constructors
    
    /**
     * Creates a new Dog at the given position.
     *
     * The Dog is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param size  The size of the dude in world units
     * @param scale The drawing scale (world to screen)
     *
     * @return  A newly allocated DudeModel at the given position with the given scale
     */
    static std::shared_ptr<TileInfo> alloc(const cugl::Vec2& pos, const cugl::Size& size,Terrain m_type, std::shared_ptr<cugl::Texture> m_texture) {
        std::shared_ptr<TileInfo> result = std::make_shared<TileInfo>();
        return (result->init(pos, size, m_type, m_texture) ? result : nullptr);
    }
#pragma mark Constructors
    
    virtual bool init(const cugl::Vec2& pos, const cugl::Size& size,Terrain m_type, std::shared_ptr<cugl::Texture> m_texture);
    std::shared_ptr<cugl::Texture> texture;
    Terrain type;
    TileInfo(){
        
    }
    ~TileInfo(){
//        std::cout <<"TODO DISPOSE\n";
    }
private:
};

class World{
public:
        
private:
    // Matrix with information about the overworld
    std::shared_ptr<cugl::Texture> tile;
    std::shared_ptr<cugl::AssetManager> _assets;
    cugl::Vec2 start;
    
public:
    
    std::vector<std::vector<std::shared_ptr<TileInfo>>> tileWorld;
    std::vector<std::vector<std::shared_ptr<TileInfo>>> boundaryWorld;
    std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> lowerDecorWorld;
    std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> upperDecorWorld;
    World () {};
    ~World(){
        CULog("Destructing World");
    }
    World(std::shared_ptr<LevelModel> _level,std::shared_ptr<cugl::AssetManager> assets);

    std::shared_ptr<cugl::Texture> getBoxFromTileSet(int position, const std::map<int,TileSet>& tileSets);
    
    const std::vector<std::vector<std::shared_ptr<TileInfo>>>& getTileWorld(){
        return tileWorld;
    }
    
    const std::vector<std::vector<std::shared_ptr<TileInfo>>>& getBoundaryWorld(){
        return boundaryWorld;
    }
    
    const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>>& getLowerDecorWorld(){
        return lowerDecorWorld;
    }
    const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>>& getUpperDecorWorld(){
        return upperDecorWorld;
    }
    
    // Get whether a tile is passible or not
    const bool isPassable(int x, int y);
    
    // Get the number of rows of tiles in the world
    int getRows(){
        return(int) boundaryWorld.size();
    }
    
    // Get the number of columns of tiles in the world
    int getCols(){
        return (int) boundaryWorld[0].size();
    }
};

#endif /* World_hpp */
