//
//  LevelParser.cpp
//  Heavan
//
//  Created by Lisa Asriev on 3/24/24.
//

#include "LevelParser.h"

using namespace cugl;

LevelParser::LevelParser(){
}

LevelParser::LevelParser(std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::JsonValue> levelJson){
    _assets = assets;
    _level = levelJson;
//    _tileset = _level->get("tileset");
    _levelHeight = _level->get("height")->asInt();
    _levelWidth = _level->get("width")->asInt();
    _tileHeight = _level->get("tileheight")->asInt();
    _tileWidth = _level->get("tilewidth")->asInt();
}

void LevelParser::processLayers(){
    if (_level){
        auto layerValues = _level->get("layers")->children();
        for(auto it = layerValues.begin(); it != layerValues.end(); it++) {
            std::shared_ptr<cugl::JsonValue> layer = (*it);
            std::cout << layer->toString() << std::endl;
            if (layer->getString("name")=="DrawTiles"){
                processMatrix(layer, _tile);
            }
            else if (layer->getString("name")=="Boundaries"){
                processMatrix(layer, _walls);
            }
            else if (layer->getString("name")=="Decorative"){
                processMatrix(layer, _decors);
            }
            else if (layer->getString("name")=="PlayerSpawn"){
                float playerX = layer->get("objects")->get(0)->get("x")->asFloat();
                float playerY = layer->get("objects")->get(0)->get("y")->asFloat();
                _playerPos = cugl::Vec2(playerX, playerY);
            }
            else if (layer->getString("name")=="SpawnerLocs"){
                float spawnerX1 = layer->get("objects")->get(0)->get("x")->asFloat();
                float spawnerY1 = layer->get("objects")->get(0)->get("x")->asFloat();
                _spawnersPos.push_back(cugl::Vec2(spawnerX1, spawnerY1));
                float spawnerX2 = layer->get("objects")->get(1)->get("x")->asFloat();
                float spawnerY2 = layer->get("objects")->get(1)->get("x")->asFloat();
                _spawnersPos.push_back(cugl::Vec2(spawnerX2, spawnerY2));
//                for(auto it = objValues.begin(); it != objValues.end(); it++){
//                    std::shared_ptr<cugl::JsonValue> layer = (*it);
//                    float spawnerX = obj->getFloat("x");
//                    float spawnerY = obj->getFloat("y");
//                    cugl::Vec2 spawnerPos = cugl::Vec2(spawnerX, spawnerY);
//                    _spawnersPos.push_back(spawnerPos);
//                }
            }
        }
    }
}

void LevelParser::processMatrix(std::shared_ptr<cugl::JsonValue> levelJson, std::vector<std::vector<int>>& matrix){
    int row = levelJson->get("width")->asInt();
    int column = levelJson->get("height")->asInt();
    auto array = levelJson->get("data")->children();
    int x = 0;
    int y = 0;
    matrix.resize(row);
    matrix.at(x).resize(column);
    for (auto it = array.begin(); it != array.end(); it++)
    {
        std::cout << x << " " << y << std::endl;
        matrix.at(x).at(y) = (*it)->asInt();
        y += 1;
        if(y == column){
            y = 0;
            x += 1;
            if(x < row){
                matrix.at(x).resize(column);
            }
        }
    }
}

