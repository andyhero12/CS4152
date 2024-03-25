//
//  LevelParser.cpp
//  Heavan
//
//  Created by Lisa Asriev on 3/24/24.
//

#include "LevelParser.h"

using namespace cugl;

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
                processTiles(layer);
            }
            else if (layer->getString("name")=="Boundaries"){
                processBounds(layer);
            }
            else if (layer->getString("name")=="Decorative"){
                processDecors(layer);
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

void LevelParser::processTiles(std::shared_ptr<cugl::JsonValue> levelJson){
    
}

void LevelParser::processBounds(std::shared_ptr<cugl::JsonValue> levelJson){
    
}

void LevelParser::processDecors(std::shared_ptr<cugl::JsonValue> levelJson){
    
}

