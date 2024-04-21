//
//  OverWorld.hpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//
#pragma once
#ifndef OverWorld_hpp
#define OverWorld_hpp
#include <cugl/cugl.h>
#include <unordered_set>
#include "Base.h"
#include "NLDog.h"
#include "Devil.h"
#include "Animation.h"
#include "BaseSet.h"
#include "NLInput.h"
#include "AttackPolygons.h"
#include "DecoySet.h"
#include "NLLevelModel.h"
#include "NLDecoyEvent.h"
#include "NLRecallEvent.h"
#include "NLBiteEvent.h"
#include "NLExplodeEvent.h"
#include "NLShootEvent.h"
#include "World.h"
#include "NLDashEvent.h"
#include "NLSizeEvent.h"

class OverWorld
{
private:
    std::shared_ptr<LevelModel> _level;
    std::shared_ptr<NetEventController> _network;
    cugl::Size _activeSize;
    std::shared_ptr<Dog> _dog;
    std::shared_ptr<Dog> _dogClient;
    std::shared_ptr<Devil> _devil;
    std::shared_ptr<DecoySet> _decoys;
    std::shared_ptr<BaseSet> _bases;
    std::shared_ptr<cugl::JsonValue> _constants;
    std::shared_ptr<cugl::AssetManager> _assets;
    AttackPolygons _attackPolygonSet;
    AttackPolygons _clientAttackPolygonSet;
    std::shared_ptr<World> _world;

    void drawDecoy(const std::shared_ptr<cugl::SpriteBatch> &batch);

public:
    bool _isHost;
    OverWorld()
    {
    }
    ~OverWorld()
    {
    }

    void reset();
    bool init(const std::shared_ptr<cugl::AssetManager> &assets, const std::shared_ptr<LevelModel> &_level, cugl::Size activeSize, std::shared_ptr<cugl::physics2::net::NetEventController> network, bool isHost, std::shared_ptr<World> world);
    bool initDogModel();
    bool initDevil();
    bool initBases();
    bool initDecoys();
    bool initPolygons();
    bool initWorld();

    bool setRootNode(const std::shared_ptr<scene2::SceneNode> &_worldNode, const std::shared_ptr<scene2::SceneNode> &_debugNode, std::shared_ptr<cugl::physics2::net::NetWorld> _world);
    // will add Obstacle nodes too
    void dogUpdate(InputController &_input, cugl::Size totalSize);
    void devilUpdate(InputController &_input, cugl::Size totalSize);
    void update(InputController &input, cugl::Size totalSize, float timestep);
    void postUpdate();
    void ownedDogUpdate(InputController& _input, cugl::Size, std::shared_ptr<Dog> _curDog);
    void processShootEvent(const std::shared_ptr<ShootEvent>& shootEvent);
    void processSizeEvent(const std::shared_ptr<SizeEvent>& sizeEvent);
    void processBiteEvent(const std::shared_ptr<BiteEvent>& biteEvent);
    void processRecallEvent(const std::shared_ptr<RecallEvent>& recallEvent);
    void processExplodeEvent(const std::shared_ptr<ExplodeEvent>& explodeEvent);
    void processDashEvent(const std::shared_ptr<DashEvent>& dashEvent);
    void recallDogToClosetBase(std::shared_ptr<Dog> _curDog);
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch,cugl::Size totalSize);
    std::shared_ptr<Dog> getDog() const {
        return _dog;
    }
    std::shared_ptr<Dog> getClientDog() const
    {
        return _dogClient;
    }
    std::shared_ptr<Devil> getDevil() const
    {
        return _devil;
    }
    std::shared_ptr<DecoySet> getDecoys() const
    {
        return _decoys;
    }
    std::shared_ptr<BaseSet> getBaseSet()
    {
        return _bases;
    }
    std::shared_ptr<LevelModel> getLevelModel()
    {
        return _level;
    }
    std::shared_ptr<World> getWorld()
    {
        return _world;
    }

    int getTotalTargets() const
    {
        return 1 + (int)_bases->_bases.size() + (int)_decoys->getCurrentDecoys().size();
    }
    AttackPolygons &getAttackPolygons()
    {
        return _attackPolygonSet;
    }
    AttackPolygons &getAttackPolygonsClient()
    {
        return _clientAttackPolygonSet;
    }
    void setDog(std::shared_ptr<Dog> m_dog)
    {
        _dog = m_dog;
    }
    void setDevil(std::shared_ptr<Devil> m_devil)
    {
        _devil = m_devil;
    }
};
#endif /* OverWorld_hpp */
