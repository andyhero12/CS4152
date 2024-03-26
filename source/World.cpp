//
//  World.cpp
//  Heavan
//
//  Created by Henry Gao on 3/24/24.
//

#include "World.h"
using namespace cugl;

cugl::Size size(1,1);
#define DRAW_WIREFRAME true

World::World (cugl::Vec2 bottomleft, std::vector<std::vector<int>> &map, std::vector<std::vector<int>> &passable, std::shared_ptr<cugl::Texture> tileset):start(bottomleft), tile(tileset){
    overworld.resize(map.size());
    for(int i = 0; i < map.size(); i++){
        for (int j = 0; j < map[0].size(); j++){
            if(passable.at(i).at(j) == 0 && i != 26){
                overworld[i].emplace_back(size, Terrain::PASSABLE, getBox(map.at(i).at(j)));
            }
            else{
                overworld[i].emplace_back(size, Terrain::IMPASSIBLE, getBox(map.at(i).at(j)));
            }
        }
    }
    int originalRows = (int) overworld.size();
    int originalCols = (int) overworld.at(0).size();
    int printIndexJ = 0;
    for (int j =0  ;j< originalCols; j++){
        int printIndexI = 0;
        for (int i = originalRows -1; i > -1; i--){
            TileInfo& t = overworld.at(i).at(j);
            t.boundaryRect = Rect(Vec2((t.size.width )*printIndexJ, (t.size.height)*printIndexI), t.size);
            t.boxObstacle = t.type == Terrain::IMPASSIBLE ? physics2::BoxObstacle::alloc(t.boundaryRect.origin, t.boundaryRect.size) : nullptr;
            if (t.boxObstacle != nullptr) {
                t.boxObstacle->setBodyType(b2_staticBody);
                t.boxObstacle->setDensity(1);
                t.boxObstacle->setLinearDamping(0.1f);
            }
            printIndexI++;
        }
        printIndexJ++;
    }
}

World::TileInfo::TileInfo(cugl::Size size, Terrain type, std::shared_ptr<cugl::Texture> texture)
: size(size), 
type(type),
texture(texture) {
}

void World::draw(const std::shared_ptr<cugl::SpriteBatch>& batch){
    int originalRows = (int) overworld.size();
    int originalCols = (int) overworld.at(0).size();
    int printIndexJ = 0;
    for (int j =0  ;j< originalCols; j++){
        int printIndexI = 0;
        for (int i = originalRows -1; i > -1; i--){
            Color4 tint = cugl::Color4("white");
            TileInfo& t = overworld.at(i).at(j);
            batch->draw(t.texture, tint, t.boundaryRect);
            if (t.boxObstacle != nullptr && DRAW_WIREFRAME) {
                //std::cout << "hello " + std::to_string(t.boxObstacle->getDimension().width) + " "
                //    + std::to_string(t.boxObstacle->getDimension().height) + "\n";
                Color4 wireframeColor = Color4("green");
                batch->setColor(wireframeColor);
                batch->setTexture(nullptr);
                batch->outline(Rect(t.boxObstacle->getPosition(), t.boxObstacle->getDimension()));
            }
            printIndexI++;
        }
        printIndexJ++;
    }
    
}
std::shared_ptr<cugl::Texture> World::getBox(int position){
    int boxSize = 32;  // Each box is 32x32 pixels
    float textureWidth = tile->getWidth();
    float textureHeight = tile->getHeight();
    position = position - 1;
    int boxesPerRow = textureWidth / boxSize;
    int row = position / boxesPerRow;
    int column = position % boxesPerRow;

    // Calculate the pixel coordinates for the kth box
    int minS_px = (column) * boxSize;
    int maxS_px = minS_px + boxSize;
    int minT_px = row * boxSize;
    int maxT_px = minT_px + boxSize;

    GLfloat minS = (minS_px) / textureWidth;
    GLfloat maxS = (maxS_px) / textureWidth;
    GLfloat minT = (minT_px) / textureHeight;
    GLfloat maxT = (maxT_px) / textureHeight;

    // Get the subTexture
    return tile->getSubTexture(minS, maxS, minT, maxT);
}
    


