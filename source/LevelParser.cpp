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
    _tileset = _level->get("tileset");
    _levelHeight = _level->getInt("height");
    _levelWidth = _level->getInt("width");
    _tileHeight = _level->getInt("tileheight");
    _tileWidth = _level->getInt("tilewidth");
}

void LevelParser::processLayers(){
    if (_level){
        for(auto& layer : _level->get("layers")->children()) {
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
                float playerX = layer->get("name")->get("objects")->getFloat("x");
                float playerY = layer->get("name")->get("objects")->getFloat("y");
                _playerPos = cugl::Vec2(playerX, playerY);
            }
            else if (layer->getString("name")=="SpawnerLocs"){
                for(auto& obj : layer->get("name")->get("objects")->children()) {
                    float spawnerX = obj->getFloat("x");
                    float spawnerY = obj->getFloat("y");
                    cugl::Vec2 spawnerPos = cugl::Vec2(spawnerX, spawnerY);
                    _spawnersPos.push_back(spawnerPos);
                }
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
    matrix.resize(column);
    for (auto it = array.begin(); it != array.end(); it++)
    {
        matrix.at(x).at(y) = (*it)->asInt();
        y += 1;
        if(y == column){
            y = 0;
            x += 1;
            matrix[x].resize(column);
        }
    }
}

