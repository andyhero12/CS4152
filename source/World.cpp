//
//  World.cpp
//  Heavan
//
//  Created by Henry Gao on 3/24/24.
//

#include "World.h"
using namespace cugl;

World::World (cugl::Vec2 bottomleft, std::vector<std::vector<int>> &map, std::shared_ptr<cugl::Texture> passable,     std::shared_ptr<cugl::Texture> impassable):start(bottomleft){
    cugl::Size size(40,40);
    for(int i = 0; i < map.size(); i++){
        for (int j = 0; j < map[0].size(); j++){
            if(map[i][j] == 0){
                overworld[i].emplace_back(size, Terrain::PASSABLE, passable);
            }
            else{
                overworld[i].emplace_back(size, Terrain::IMPASSIBLE, impassable);
            }
        }
    }
}


World::TileInfo::TileInfo(cugl::Size &size, Terrain type, std::shared_ptr<cugl::Texture> texture):type(type), size(size), texture(texture){}

void World::draw(const std::shared_ptr<cugl::SpriteBatch>& batch){
    for (int i = 0; i <= overworld.size(); i++) {
           for (int j = 0; j <= overworld[0].size(); j++) {
               Color4 tint;
            tint = cugl::Color4("white");
               TileInfo t = overworld[i][j];
               batch->draw(t.texture, tint, Rect(Vec2((t.size.width - 2)*i, (t.size.height-2)*j), t.size));
           }
       }
};


