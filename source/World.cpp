//
//  World.cpp
//  Heavan
//
//  Created by Henry Gao on 3/24/24.
//

#include "World.h"
using namespace cugl;

World::World (cugl::Vec2 bottomleft, std::vector<std::vector<int>> &map, std::vector<std::vector<int>> &passable, std::shared_ptr<cugl::Texture> tileset):start(bottomleft), tile(tileset){
    cugl::Size size(40,40);
    overworld.resize(map.size());
    for(int i = 0; i < map.size(); i++){
        for (int j = 0; j < map[0].size(); j++){
            if(passable.at(i).at(j) == 0){
                overworld[i].emplace_back(size, Terrain::PASSABLE, getBox(map.at(i).at(j)));
            }
            else{
                overworld[i].emplace_back(size, Terrain::IMPASSIBLE, getBox(map.at(i).at(j)));
            }
        }
    }
}


#include <iostream>

World::TileInfo::TileInfo(cugl::Size size, Terrain type, std::shared_ptr<cugl::Texture> texture)
: size(size), type(type), texture(texture) {
    // Debug print to verify texture assignment (optional)
//    std::cout << "Texture assigned: " << texture << std::endl;
}


template<typename T>
std::vector<std::vector<T>> rotate90(const std::vector<std::vector<T>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) return {};

    int originalRows = matrix.size();
    int originalCols = matrix[0].size();

    // Create an outer vector with the correct size, but don't initialize the inner vectors yet
    std::vector<std::vector<T>> rotatedMatrix(originalCols);

    for (int j = 0; j < originalCols; ++j) {
        // Create a new inner vector for each column of the original matrix
        std::vector<T> newRow;

        for (int i = originalRows - 1; i >= 0; --i) {
            // Push elements into the new row vector in the correct order
            newRow.push_back(matrix[i][j]);
        }

        // Assign the newly created row to the rotated matrix
        rotatedMatrix[j] = newRow;
    }

    return rotatedMatrix;
}

void World::draw(const std::shared_ptr<cugl::SpriteBatch>& batch){
    
    std::vector<std::vector<World::TileInfo>> overworld90 = rotate90(overworld);
    
    for (int i = 0; i < overworld90.size(); i++) {
           for (int j = 0; j < overworld90[0].size(); j++) {
               Color4 tint = cugl::Color4("white");
               TileInfo t = overworld90.at(i).at(j);
               batch->draw(t.texture, tint, Rect(Vec2((t.size.width )*i, (t.size.height)*j), t.size));
           }
       }
};


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


