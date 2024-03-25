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
    
    ~LevelParser();
    
    void processLayers();
    
    void processTiles(std::shared_ptr<cugl::JsonValue> levelJson);
    
    void processBounds(std::shared_ptr<cugl::JsonValue> levelJson);
    
    void processDecors(std::shared_ptr<cugl::JsonValue> levelJson);
    
    
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
    
    std::vector<std::vector<int>> _walls;
    
    std::vector<std::vector<int>> _decors;
};



#endif /* LevelParser_h */
