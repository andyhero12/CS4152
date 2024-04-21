//
//  JSLevelModel.hpp
//  JSONDemo
//
//  This module provides a level that can be loaded from a JSON file.  It reads the JSON
//  file and creates the objects in the level.  Objects are one of four categories: the
//  Dog, the exit, the walls, and the crates.
//
//  This class is a subclass of Asset so that we can use a GenericLoader to manage the
//  the loading.  Even though this class defines a load method, there is the question of
//  when the method is called. The GenericLoader allows this object to be loaded in a
//  separate thread along with all of the other assets.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White and Anthony Perello
//  Version: 3/12/17
//
#ifndef __NL_LEVEL_Parser_H__
#define __NL_LEVEL_Parser_H__
#include <cugl/cugl.h>
#include <cugl/physics2/CUObstacleWorld.h>
#include <vector>
#include <cugl/assets/CUAsset.h>
#include <cugl/io/CUJsonReader.h>
#include "NLDog.h"
#include "NLLevelConstants.h"
using namespace cugl;


class TileSet{
public:
    int firstGid;
    std::string source;
    std::shared_ptr<cugl::Texture> textureTile;
    std::shared_ptr<cugl::JsonValue> tileJson;
    
    TileSet(int m_gid, std::string& m_source, std::shared_ptr<cugl::AssetManager> _assets){
        firstGid = m_gid;
        source = m_source;
        CULog("source %s", source.data());
        textureTile = _assets->get<cugl::Texture>(source);
        if (textureTile){
            CULog("tile Found %s", source.data());
        }else{
            CULog("tile NOT FOUND %s", source.data());
        }
        tileJson = _assets->get<JsonValue>(source);
        if (tileJson){
            CULog("Json Found %s", source.data());
        }else{
            CULog("Json NOT FOUND %s", source.data());
        }
    }
};
#pragma mark -
#pragma mark Level Model
/**
* Class that represents a dynamically loaded level in the game
*
* This class is a subclass of Asset so that we can use it with a GenericLoader. As with
* all assets, this class SHOULD NOT make any references to AssetManager in the load/unload
* methods. Assets should be treated as if they load in parallel, not in sequence.  Therefore,
* it is unsafe to assume that one asset loads before another.  If this level needs to connect
* to other assets (sound, images, etc.) this should take place after asset loading, such as
* during scene graph initialization.
*/
class LevelModel : public Asset {
public:
    struct Spawner{
        float spawnerX;
        float spawnerY;
        int hp;
        float initDelay;
        float regularDelay;
        std::string primaryEnemy;
        std::string secondaryEnemy;
        std::string tertiaryEnemy;
    };
    
private:
    std::shared_ptr<cugl::AssetManager> _assets;
    
    std::shared_ptr<cugl::JsonValue> _tileset;
    
    std::shared_ptr<cugl::Texture> _defaultTexture;
    
    int _numLowerDecorLayers;
    int _numUpperDecorLayers;
    
    float _levelHeight;
    
    float _levelWidth;
    
    float _tileHeight;
    
    float _tileWidth;
    
    cugl::Vec2 _playerPos;
    
    std::vector<cugl::Vec3> _basesPos;
    std::vector<cugl::Vec3> _preSpawnLocs;
    std::vector<Spawner> _spawnersPos;
    
    std::vector<std::vector<int>> _tiles;
    
    std::vector<std::vector<int>> _walls;
    
    std::vector<std::vector<std::vector<int>>> _lowerDecorLayers;
    std::vector<std::vector<std::vector<int>>> _upperDecorLayers;
    // ordered for lower bound
    std::map<int,std::string> tileSetMapping;
    std::map<int,TileSet> tilesMappingWithTextures;
protected:
    
#pragma mark Internal Helper
    
    void loadLayer(const std::shared_ptr<JsonValue>& json);
    
public:
    
    const std::map<int,std::string>& getTileSetMapping(){
        return tileSetMapping;
    }
    const std::map<int,TileSet>& getTileSetWithTextures(){
        return tilesMappingWithTextures;
    }
    void setTileSetAssets(std::shared_ptr<cugl::AssetManager> assets){
        for (auto& kv : tileSetMapping){
            tilesMappingWithTextures.insert({kv.first,TileSet(kv.first, kv.second,assets)});
        }
    }
    int getLevelHeight(){return _levelHeight;};
    
    int getLevelWidth(){return _levelWidth;};
    
    int getTileHeight(){return _tileHeight;};
    
    int getTileWidth(){return _tileWidth;};
    int getLowerDecorLayers(){ return _numLowerDecorLayers;}
    int getUpperDecorLayers(){ return _numUpperDecorLayers;}
    
    cugl::Vec2 getPlayerPos(){return _playerPos;};
    
    const std::vector<Spawner>& getSpawnersPos(){
        return _spawnersPos;
    };
    const std::vector<cugl::Vec3>& getBasesPos(){
        return _basesPos;
    };
    const std::vector<cugl::Vec3>& preSpawnLocs(){
        return _preSpawnLocs;
    };
    
    const std::vector<std::vector<int>>& getTiles()    { return _tiles;};
    
    const std::vector<std::vector<int>>& getBoundaries(){return _walls;};
    
    const std::vector<std::vector<std::vector<int>>>& getLowerDecorations(){
        return _lowerDecorLayers;
    };
    const std::vector<std::vector<std::vector<int>>>& getUpperDecorations(){
        return _upperDecorLayers;
    };
#pragma mark Static Constructors
    /**
     * Creates a new game level with no source file.
     *
     * The source file can be set at any time via the setFile() method. This method
     * does NOT load the asset.  You must call the load() method to do that.
     *
     * @return  an autoreleased level file
     */
    static std::shared_ptr<LevelModel> alloc() {
        std::shared_ptr<LevelModel> result = std::make_shared<LevelModel>();
        return (result->init("") ? result : nullptr);
    }

    /**
     * Creates a new game level with the given source file.
     *
     * This method does NOT load the level. You must call the load() method to do that.
     * This method returns false if file does not exist.
     *
     * @return  an autoreleased level file
     */
    static std::shared_ptr<LevelModel> alloc(std::string file) {
        std::shared_ptr<LevelModel> result = std::make_shared<LevelModel>();
        return (result->init(file) ? result : nullptr);
    }
    /**
     * Sets the loaded assets for this game level
     *
     * @param assets the loaded assets for this game level
     */
    void setAssets(const std::shared_ptr<AssetManager>& assets) { _assets = assets;  }


#pragma mark -
#pragma mark Asset Loading
    bool loadBaseLocations(const std::shared_ptr<JsonValue>& json);

    bool loadPreSpawnedClusters(const std::shared_ptr<JsonValue>& json);
    
    bool loadTiles(const std::shared_ptr<JsonValue>& json);
    
    bool loadBoundaries(const std::shared_ptr<JsonValue>& json);
    
    bool loadDecorations(const std::shared_ptr<JsonValue>& json);
    
    bool loadNumDecor(const std::shared_ptr<JsonValue>& json);
    bool loadLowerDecorLayer(const std::shared_ptr<JsonValue>& json, int index);
    bool loadUpperDecorLayer(const std::shared_ptr<JsonValue>& json, int index);
    bool loadPlayer(const std::shared_ptr<JsonValue>& json);
    bool loadSpanwerLocations(const std::shared_ptr<JsonValue>& json);
    /**
     * Loads this game level from the source file
     *
     * This load method should NEVER access the AssetManager.  Assets are loaded in
     * parallel, not in sequence.  If an asset (like a game level) has references to
     * other assets, then these should be connected later, during scene initialization.
     *
     * @param file the name of the source file to load from
     *
     * @return true if successfully loaded the asset from a file
     */
    virtual bool preload(const std::string& file);


    /**
     * Loads this game level from a JsonValue containing all data from a source Json file.
     *
     * This load method should NEVER access the AssetManager.  Assets are loaded in
     * parallel, not in sequence.  If an asset (like a game level) has references to
     * other assets, then these should be connected later, during scene initialization.
     *
     * @param json the json loaded from the source file to use when loading this game level
     *
     * @return true if successfully loaded the asset from the input JsonValue
     */
    virtual bool preload(const std::shared_ptr<cugl::JsonValue>& json) override;

    /**
     * Unloads this game level, releasing all sources
     *
     * This load method should NEVER access the AssetManager.  Assets are loaded and
     * unloaded in parallel, not in sequence.  If an asset (like a game level) has
     * references to other assets, then these should be disconnected earlier.
     */
    void unload();


    //#pragma mark -
    //#pragma mark Initializers
    /**
     * Creates a new, empty level.
     */
    LevelModel(void);

    /**
     * Destroys this level, releasing all resources.
     */
    virtual ~LevelModel(void);
};

#endif /* defined(__NL_LEVEL_Parser__) */
