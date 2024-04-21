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
    cugl::Vec2 start;
    
public:
    std::vector<std::vector<std::shared_ptr<TileInfo>>> overworld;
    World () {};
    World (cugl::Vec2 bottomleft, std::vector<std::vector<int>> &map, std::vector<std::vector<int>> &passable, std::shared_ptr<cugl::Texture> tileset);
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
    std::shared_ptr<cugl::Texture> getBox(int position);

};

#endif /* World_hpp */
