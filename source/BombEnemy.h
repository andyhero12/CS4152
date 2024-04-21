//
//  BombEnemy.hpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#ifndef BombEnemy_hpp
#define BombEnemy_hpp

#include "AbstractEnemy.h"

/**
 * The factory class for crate objects.
 *
 * This class is used to support automatically syncing newly added obstacle mid-simulation.
 * Obstacles added throught the ObstacleFactory class from one client will be added to all
 * clients in the simulations.
 */
class BombFactory : public ObstacleFactory
{
public:
    struct AnimationStruct{
        std::vector<std::shared_ptr<cugl::Texture>> _walkTextures;
        std::vector<std::shared_ptr<cugl::Texture>> _attackTextures;
        int _framesize;
        int _framecols;
        int _freqAnimations;
    };
    /** Pointer to the AssetManager for texture access, etc. */
    std::shared_ptr<cugl::JsonValue> _data;
    std::shared_ptr<cugl::AssetManager> _assets;
    AnimationStruct staticEnemyStruct;
    /** Serializer for supporting parameters */
    LWSerializer _serializer;
    /** Deserializer for supporting parameters */
    LWDeserializer _deserializer;

    /**
     * Allocates a new instance of the factory using the given AssetManager.
     */
    static std::shared_ptr<BombFactory> alloc(std::shared_ptr<cugl::JsonValue> data, std::shared_ptr<AssetManager> &assets)
    {
        auto f = std::make_shared<BombFactory>();
        f->init(data, assets);
        return f;
    };

    /**
     * Initializes empty factories using the given AssetManager.
     */
    void init(std::shared_ptr<cugl::JsonValue> data, std::shared_ptr<AssetManager> &assets)
    {
        _data = data;
        _assets = assets;
        int _framecols = data->getFloat("sprite cols", 0);
        int _framesize = data->getFloat("sprite size", 0);
        std::vector<std::shared_ptr<cugl::Texture>> textures;
        textures.push_back(_assets->get<Texture>("bombFrontRightWalk"));
        textures.push_back(_assets->get<Texture>("bombFrontRightWalk"));
        textures.push_back(_assets->get<Texture>("bombFrontRightWalk"));
        textures.push_back(_assets->get<Texture>("bombFrontRightWalk"));
        textures.push_back(_assets->get<Texture>("bombFrontLeftWalk"));
        textures.push_back(_assets->get<Texture>("bombFrontLeftWalk"));
        textures.push_back(_assets->get<Texture>("bombFrontLeftWalk"));
        textures.push_back(_assets->get<Texture>("bombFrontLeftWalk"));
        staticEnemyStruct._walkTextures  = textures;
        staticEnemyStruct._attackTextures  = textures;
        staticEnemyStruct._framesize = _framesize;
        staticEnemyStruct._framecols = _framecols;
        staticEnemyStruct._freqAnimations = 4;
    }

    /**
     * Generate a pair of Obstacle and SceneNode using the given parameters
     */
    std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> createObstacle(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex);

    /**
     * Helper method for converting normal parameters into byte vectors used for syncing.
     */
    std::shared_ptr<std::vector<std::byte>> serializeParams(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex);

    /**
     * Generate a pair of Obstacle and SceneNode using serialized parameters.
     */
    std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> createObstacle(const std::vector<std::byte> &params) override;
};


class BombEnemy : public AbstractEnemy {
public:
    
    static std::shared_ptr<BombEnemy> alloc(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex) {
        std::shared_ptr<BombEnemy> result = std::make_shared<BombEnemy>();
        return (result->init(m_pos, m_size, m_health, m_targetIndex) ? result : nullptr);
    }
    BombEnemy();
    
    bool init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex);
    virtual ~BombEnemy() {
        
    }
    
    virtual void preUpdate(float dt, OverWorld& overWorld) override;
    
    virtual int getDamage() override{
        return _contactDamage;
    }
    int getExplosionDamage(){
        return _baseExplosionDamage;
    }
    bool canAttack() const override{
        if (_attackCooldown > 60){
            std::cout <<"WTF\n";
        }
        return _attackCooldown >= 60;
    }
    
    virtual void resetAttack() override{
        _attackCooldown = 0;
    }
    virtual void executeDeath(OverWorld& overWorld) override;
    
    virtual int getAbsorbValue() const override{
        CULog("TODO ABSORB BOMB\n");
        return 1;
    }
    
    
protected:
    int _contactDamage;
    int _attackCooldown;
    int _baseExplosionDamage;
};
#endif /* BombEnemy_hpp */
