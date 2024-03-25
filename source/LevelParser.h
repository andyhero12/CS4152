//
//  LevelParser.hpp
//  Heavan
//
//  Created by Lisa Asriev on 3/24/24.
//

#ifndef LevelParser_h
#define LevelParser_h

#include <stdio.h>
#include <cugl/cugl.h>

class LevelParser{
public:
    LevelParser(std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::JsonValue> levelJson);
    
    LevelParser();
    
    ~LevelParser();
    
    void processLayers();
    
    void processMatrix(std::shared_ptr<cugl::JsonValue> levelJson, std::vector<std::vector<int>>& matrix);
    
    int getLevelHeight(){return _levelHeight;};
    
    int getLevelWidth(){return _levelWidth;}; 
    
    int getTileHeight(){return _tileHeight;};
    
    int getTileWidth(){return _tileWidth;};
    
    cugl::Vec2 getPlayerPos(){return _playerPos;};
    
    std::vector<cugl::Vec2> getSpawnersPos(){return _spawnersPos;};
    
    std::vector<std::vector<int>> getTile(){return _tile;};
    
    std::vector<std::vector<int>> getBoundaries(){return _walls;};
    
    std::vector<std::vector<int>> getDecorations(){return _decors;};

private:
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::JsonValue> _tileset;
    
    std::shared_ptr<cugl::JsonValue> _level;
    
    std::shared_ptr<cugl::Texture> _defaultTexture;
    
    int _levelHeight;
    
    int _levelWidth;
    
    int _tileHeight;
    
    int _tileWidth;
    
    cugl::Vec2 _playerPos;
    
    std::vector<cugl::Vec2> _spawnersPos;
    
    std::vector<std::vector<int>> _tile;
    
    std::vector<std::vector<int>> _walls;
    
    std::vector<std::vector<int>> _decors;
};



#endif /* LevelParser_h */
