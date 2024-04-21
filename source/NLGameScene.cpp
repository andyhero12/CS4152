//
//  NLGameScene.h
//  Networked Physics Demo
//
//  This is the most important class in this demo.  This class manages the
//  gameplay for this demo.  It also handles collision detection. There is not
//  much to do for collisions; our ObstacleWorld class takes care of all
//  of that for us.  This controller mainly transforms input into gameplay.
//
//  You will notice that we do not use a Scene asset this time.  While we could
//  have done this, we wanted to highlight the issues of connecting physics
//  objects to scene graph objects.  Hence we include all of the API calls.
//
//  WARNING: There are a lot of shortcuts in this design that will do not adapt
//  well to data driven design.  This demo has a lot of simplifications to make
//  it a bit easier to see how everything fits together.  However, the model
//  classes and how they are initialized will need to be changed if you add
//  dynamic level loading.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/10/17
//
#include "NLGameScene.h"
#include <box2d/b2_world.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_collision.h>

#include <ctime>
#include <string>
#include <iostream>
#include <sstream>

using namespace cugl;
using namespace cugl::physics2::net;

#pragma mark -
#pragma mark Level Geography

/** This is the size of the active portion of the screen */
#define SCENE_WIDTH 1280
#define SCENE_HEIGHT 800

#define CANVAS_TILE_HEIGHT 8

/** Width of the game world in Box2d units */
#define DEFAULT_WIDTH 100.0f
/** Height of the game world in Box2d units */
#define DEFAULT_HEIGHT 100.0f
/** The default value of gravity (going down) */
#define DEFAULT_GRAVITY -0.0f

#define DEFAULT_TURN_RATE 0.05f

#pragma mark Assset Constants
/** The key for the fire textures in the asset manager */
#define MAIN_FIRE_TEXTURE "flames"
#define RGHT_FIRE_TEXTURE "flames-right"
#define LEFT_FIRE_TEXTURE "flames-left"

/** Color to outline the physics nodes */
#define STATIC_COLOR Color4::WHITE
/** Opacity of the physics outlines */
#define DYNAMIC_COLOR Color4::YELLOW

/** The key for collisions sounds */
#define COLLISION_SOUND "bump"
/** The key for the main afterburner sound */
#define MAIN_FIRE_SOUND "burn"
/** The key for the right afterburner sound */
#define RGHT_FIRE_SOUND "right-burn"
/** The key for the left afterburner sound */
#define LEFT_FIRE_SOUND "left-burn"

/** The key for the font reference */
#define PRIMARY_FONT "retro"

#pragma mark Physics Constants

// Physics constants for initialization
/** Density of non-crate objects */
#define BASIC_DENSITY 0.0f
/** Friction of non-crate objects */
#define BASIC_FRICTION 0.1f
/** Collision restitution for all objects */
#define BASIC_RESTITUTION 0.1f
/** Threshold for generating sound on collision */
#define SOUND_THRESHOLD 3

#define FIXED_TIMESTEP_S 0.02f
#define ROOT_NODE_SCALE 1

#pragma mark -
#pragma mark Constructors
/**
 * Creates a new game world with the default values.
 *
 * This constructor does not allocate any objects or start the controller.
 * This allows us to use a controller without a heap pointer.
 */
GameScene::GameScene() : cugl::Scene2(),
                         _debug(false),
                         _isHost(false)
{
}

/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * The game world is scaled so that the screen coordinates do not agree
 * with the Box2d coordinates.  This initializer uses the default scale.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool GameScene::init(const std::shared_ptr<AssetManager> &assets, const std::shared_ptr<cugl::physics2::net::NetEventController> network, bool isHost, std::string level)
{
    return init(assets, Rect(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT), Vec2(0, DEFAULT_GRAVITY), network, isHost, level);
}

/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * The game world is scaled so that the screen coordinates do not agree
 * with the Box2d coordinates.  The bounds are in terms of the Box2d
 * world, not the screen.
 *
 * @param assets    The (loaded) assets for this game mode
 * @param rect      The game bounds in Box2d coordinates
 *
 * @return  true if the controller is initialized properly, false otherwise.
 */
bool GameScene::init(const std::shared_ptr<AssetManager> &assets, const Rect rect, const std::shared_ptr<NetEventController> network, bool isHost, std::string level)
{
    return init(assets, rect, Vec2(0, DEFAULT_GRAVITY), network, isHost, level);
}

/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * The game world is scaled so that the screen coordinates do not agree
 * with the Box2d coordinates.  The bounds are in terms of the Box2d
 * world, not the screen.
 *
 * @param assets    The (loaded) assets for this game mode
 * @param rect      The game bounds in Box2d coordinates
 * @param gravity   The gravitational force on this Box2d world
 *
 * @return  true if the controller is initialized properly, false otherwise.
 */
bool GameScene::init(const std::shared_ptr<AssetManager> &assets, const Rect rect, const Vec2 gravity, const std::shared_ptr<NetEventController> network, bool isHost, std::string level_string)
{
    Size dimen = computeActiveSize();

    if (assets == nullptr)
    {
        return false;
    }
    else if (!Scene2::init(dimen))
    {
        return false;
    }

    _isHost = isHost;

    _network = network;

    // Start up the input handler
    _level = assets->get<LevelModel>(level_string);
    _level->setTileSetAssets(assets);
    _constants = assets->get<JsonValue>("constants");
    if (_level == nullptr)
    {
        // Might need to change later if too many assets Copy Tiled Demo?
        CULog("Fail!");
        return false;
    }
    _assets = assets;
    _input.init();
    _input.update();

    // IMPORTANT: SCALING MUST BE UNIFORM
    // This means that we cannot change the aspect ratio of the physics world
    // Shift to center if a bad fit
    Vec2 offset((dimen.width - SCENE_WIDTH) / 2.0f, (dimen.height - SCENE_HEIGHT) / 2.0f);
    float zoom = dimen.height / CANVAS_TILE_HEIGHT;

    //    _backgroundWrapper = std::make_shared<World>(Vec2(0, 0), _level->getTiles(), _level->getBoundaries(), assets->get<Texture>("tile"));
    _backgroundWrapper = std::make_shared<World>(_level, _assets);
    _worldnode = scene2::SceneNode::alloc();
    _worldnode->setScale(zoom);
    _worldnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);

    _monsterSceneNode = scene2::SceneNode::alloc();

    _debugnode = scene2::SceneNode::alloc();
    _debugnode->setScale(zoom);
    _debugnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);

    _uinode = scene2::SceneNode::alloc();

    _rootnode = scene2::ScrollPane::allocWithBounds(computeActiveSize() * zoom * 10);
    _rootnode->setContentSize(computeActiveSize() * zoom * 10);
    _rootnode->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    _rootnode->setPosition(offset);

    addChild(_rootnode);
    _rootnode->addChild(_worldnode);
    _rootnode->addChild(_debugnode);

    addChild(_uinode);

    populate();
    std::function<void(const std::shared_ptr<physics2::Obstacle> &, const std::shared_ptr<scene2::SceneNode> &)> linkSceneToObsFunc = [=](const std::shared_ptr<physics2::Obstacle> &obs, const std::shared_ptr<scene2::SceneNode> &node)
    {
        this->linkSceneToObs(obs, node);
    };

    _network->enablePhysics(_world, linkSceneToObsFunc);

    _spawnerController.init(_level->getSpawnersPos(), assets);
    _spawnerController.setRootNode(_worldnode, _isHost);
    _worldnode->addChild(_monsterSceneNode);
    overWorld.init(assets, _level, computeActiveSize(), _network, isHost, _backgroundWrapper);
    overWorld.setRootNode(_worldnode, _debugnode, _world);
    if (isHost)
    {
        _uinode->addChild(overWorld.getDog()->getUINode());
    }
    else
    {
        _uinode->addChild(overWorld.getClientDog()->getUINode());
    }

    _monsterController.setNetwork(_network);
    _monsterController.setMeleeAnimationData(_constants->get("basicEnemy"), assets);
    _monsterController.setSpawnerAnimationData(_constants->get("spawnerEnemy"), assets);
    _monsterController.setBombAnimationData(_constants->get("bomb"), assets);
    _monsterController.setAbsorbAnimationData(_constants->get("absorbEnemy"), assets);
    _monsterController.init(overWorld, _debugnode);

    _collisionController.init();

    _active = true;
    setDebug(false);

    _network->attachEventType<DecoyEvent>();
    _network->attachEventType<BiteEvent>();
    _network->attachEventType<WinEvent>();
    _network->attachEventType<LoseEvent>();
    _network->attachEventType<RecallEvent>();
    _network->attachEventType<ExplodeEvent>();
    _network->attachEventType<DashEvent>();
    _network->attachEventType<SizeEvent>();
    _network->attachEventType<ShootEvent>();
    _network->attachEventType<GameResEvent>();

    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);

    _uinode->setContentSize(dimen);
    _uinode->doLayout();
    
    CULog("%s", computeActiveSize().toString().data());

    loseNode = cugl::scene2::PolygonNode::allocWithTexture(_assets->get<cugl::Texture>("lose_screen"));
    loseNode->setContentSize(dimen);
    loseNode->setAnchor(Vec2::ANCHOR_CENTER);
    loseNode->setPosition(0.5 * loseNode->getSize());
    
    CULog("%s", computeActiveSize().toString().data());
    winNode = cugl::scene2::PolygonNode::allocWithTexture(_assets->get<cugl::Texture>("win_screen"));
    winNode->setContentSize(dimen);
    winNode->setAnchor(Vec2::ANCHOR_CENTER);
    winNode->setPosition(0.5 * winNode->getSize());
    
    _uinode->addChild(loseNode);
    _uinode->addChild(winNode);
    loseNode->setVisible(false);
    winNode->setVisible(false);
    
    _pause = std::make_shared<PauseScene>();
    _pause->init(_assets, computeActiveSize());
    _pause->setPause(false);
    _uinode->addChild(_pause);
    
    _pause->setContentSize(dimen);
    _pause->doLayout();

    _zoom = ROOT_NODE_SCALE;

    if (isHost)
    {
        Vec2 delta = overWorld.getDog()->getDogNode()->getWorldPosition();
        delta -= (computeActiveSize() / 2);
        _rootnode->applyPan(-delta / _zoom);
        _rootnode->setScale(_zoom);
    }
    else
    {
        Vec2 delta = overWorld.getClientDog()->getDogNode()->getWorldPosition();
        delta -= (computeActiveSize() / 2);
        _rootnode->applyPan(-delta / _zoom);
        _rootnode->setScale(_zoom);
    }

    addChildForeground();
    
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameScene::dispose()
{
    if (_active)
    {
        removeAllChildren();
        _pause->dispose();
        //        _input.dispose();
        _world = nullptr;
        _worldnode = nullptr;
        _debugnode = nullptr;
        _debug = false;
        Scene2::dispose();
    }
}

#pragma mark -
#pragma mark Level Layout
/**
 * Lays out the game geography.
 *
 * Pay close attention to how we attach physics objects to a scene graph.
 * The simplest way is to make a subclass.  However,
 * for simple objects you can just use a callback function to lightly couple
 * them.  This is what we do with the crates.
 *
 * This method is really, really long.  In practice, you would replace this
 * with your serialization loader, which would process a level file.
 */
void GameScene::populate()
{
    _world = physics2::net::NetWorld::alloc(Rect(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT), Vec2(0, DEFAULT_GRAVITY));
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact *contact)
    {
        beginContact(contact);
    };
    _world->beforeSolve = [this](b2Contact *contact, const b2Manifold *oldManifold)
    {
        beforeSolve(contact, oldManifold);
    };

#pragma mark : Background
    addChildBackground();
}
void GameScene::linkSceneToObs(const std::shared_ptr<physics2::Obstacle> &obj,
                               const std::shared_ptr<scene2::SceneNode> &node)
{

    node->setPosition(obj->getPosition());
    _monsterSceneNode->addChild(node);
    // Dynamic objects need constant updating
    if (obj->getBodyType() == b2_dynamicBody)
    {
        scene2::SceneNode *weak = node.get(); // No need for smart pointer in callback
        obj->setListener([=](physics2::Obstacle *obs)
                         {
            float leftover = Application::get()->getFixedRemainder() / 1000000.f;
            Vec2 pos = obs->getPosition() + leftover * obs->getLinearVelocity();
            float angle = obs->getAngle() + leftover * obs->getAngularVelocity();
            weak->setPosition(pos);
            weak->setAngle(angle); });
    }
}

/**
 * Adds the physics object to the physics world and loosely couples it to the scene graph
 *
 * There are two ways to link a physics object to a scene graph node on the
 * screen.  One way is to make a subclass of a physics object.
 * The other is to use callback functions to loosely couple
 * the two.  This function is an example of the latter.
 *
 * param obj    The physics object to add
 * param node   The scene graph node to attach it to
 */
void GameScene::addInitObstacle(const std::shared_ptr<physics2::Obstacle> &obj,
                                const std::shared_ptr<scene2::SceneNode> &node)
{
    _world->initObstacle(obj);
    obj->setDebugScene(_debugnode);
    if (_isHost)
    {
        _world->getOwnedObstacles().insert({obj, 0});
    }
    linkSceneToObs(obj, node);
}

#pragma mark
#pragma mark Physics Handling

void GameScene::preUpdate(float dt)
{
    _input.update();
    if (_input.didPressExit())
    {
        Application::get()->quit();
    }
    if (_input.didPressPause())
    {
        CULog("pressed Pause");
        _pause->setPause(_input.getPause());
    }

    if (_input.didPressReset())
    {
        _pause->exitToMain();
    }
    if (loseNode->isVisible() || winNode->isVisible()){
        return;
    }
    if (_isHost){
        float zoom = _zoom - (ROOT_NODE_SCALE - 0.6f* (float)overWorld.getDog()->getAbsorb() / (float)overWorld.getDog()->getMaxAbsorb());
        _zoom -= fmin(zoom, 0.01f) * (zoom < 0 ? 0.12f : 0.3f);
        _rootnode->setScale(_zoom);
    }else{
        float zoom = _zoom - (ROOT_NODE_SCALE - 0.6f* (float)overWorld.getClientDog()->getAbsorb() / (float)overWorld.getClientDog()->getMaxAbsorb());
        _zoom -= fmin(zoom, 0.01f) * (zoom < 0 ? 0.12f : 0.3f);
        _rootnode->setScale(_zoom);
    }

    // Process the toggled key commands
    if (_input.didPressDebug())
    {
        setDebug(!isDebug());
    }
    overWorld.update(_input, computeActiveSize(), dt);
    _spawnerController.update(_monsterController, overWorld, dt);
    _monsterController.update(dt, overWorld);

    if (_isHost)
    {
        _collisionController.intraOverWorldCollisions(overWorld);
        _collisionController.overWorldMonsterControllerCollisions(overWorld, _monsterController);
        _collisionController.attackCollisions(overWorld, _monsterController, _spawnerController);
        
        if (_monsterController.isEmpty() && _spawnerController.win() && !winNode->isVisible())
        {
//            winNode->setVisible(true);
            _network->pushOutEvent(WinEvent::allocWinEvent(overWorld.getDog()->getPosition(), _isHost));
        }
        else if ((overWorld.getDog()->getHealth() == 0 || overWorld.getBaseSet()->baseLost()) && !loseNode->isVisible())
        {
//            loseNode->setVisible(true);
            _network->pushOutEvent(LoseEvent::allocLoseEvent(overWorld.getDog()->getPosition(), _isHost));
        }
    }else{
        if (overWorld.getClientDog()->getHealth() ==  0 && !loseNode->isVisible()){
//            loseNode->setVisible(true);
            _network->pushOutEvent(LoseEvent::allocLoseEvent(overWorld.getClientDog()->getPosition(), _isHost));
        }
    }
    
}

void GameScene::postUpdate(float dt)
{
    // Nothing to do now
    if (loseNode->isVisible() || winNode->isVisible()){
        return;
    }
    _monsterController.postUpdate();
    overWorld.postUpdate();

    _rootnode->resetPane();
    for (int i = 0; i < _decorToHide.size(); i++)
    {
        if (_decorToHide.at(i))
        {
            _decorToHide.at(i)->setColor(Color4::WHITE);
        }
    }
    _decorToHide.clear();
    Vec2 delta;

    if (_isHost)
    {
        delta = overWorld.getDog()->getDogNode()->getWorldPosition();
        delta -= (computeActiveSize() / 2);
        _rootnode->applyPan(-delta / _zoom);

        delta = overWorld.getDog()->getPosition();
    }
    else
    {
        delta = overWorld.getClientDog()->getDogNode()->getWorldPosition();
        delta -= (computeActiveSize() / 2);
        _rootnode->applyPan(-delta / _zoom);

        delta = overWorld.getClientDog()->getPosition();
    }

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            _decorToHide.push_back(_worldnode->getChildByName("decoration" + std::to_string(int(delta.y + i)) + " " + std::to_string(int(delta.x + j))));
        }
    }

    for (int i = 0; i < _decorToHide.size(); i++)
    {
        if (_decorToHide.at(i))
        {
            _decorToHide.at(i)->setColor(Color4f(1, 1, 1, 0.7f));
        }
    }

    // hiding decorations
}

void GameScene::fixedUpdate()
{
    if (loseNode->isVisible() || winNode->isVisible()){
        return;
    }
    // TODO: check for available incoming events from the network controller and call processCrateEvent if it is a CrateEvent.

    // Hint: You can check if ptr points to an object of class A using std::dynamic_pointer_cast<A>(ptr). You should always check isInAvailable() before popInEvent().

#pragma mark BEGIN SOLUTION
    while (_network->isInAvailable())
    {
        auto e = _network->popInEvent();
        if (auto decoyEvent = std::dynamic_pointer_cast<DecoyEvent>(e))
        {
            //            CULog("Decoy Event Got");
            overWorld.getDecoys()->addNewDecoy(Vec2(decoyEvent->getPos().x, decoyEvent->getPos().y));
        }
        if (auto biteEvent = std::dynamic_pointer_cast<BiteEvent>(e))
        {
            overWorld.processBiteEvent(biteEvent);
        }
        if (auto recallEvent = std::dynamic_pointer_cast<RecallEvent>(e))
        {
            overWorld.processRecallEvent(recallEvent);
        }
        if (auto explodeEvent = std::dynamic_pointer_cast<ExplodeEvent>(e))
        {
            //            CULog("Explode Event Got");
            overWorld.processExplodeEvent(explodeEvent);
        }
        if (auto shootEvent = std::dynamic_pointer_cast<ShootEvent>(e))
        {
            //            CULog("Explode Event Got");
            overWorld.processShootEvent(shootEvent);
        }
        if (auto dashEvent = std::dynamic_pointer_cast<DashEvent>(e))
        {
            //            CULog("Explode Event Got");
            overWorld.processDashEvent(dashEvent);
        }
        if (auto sizeEvent = std::dynamic_pointer_cast<SizeEvent>(e))
        {
            //            CULog("Explode Event Got");
            overWorld.processSizeEvent(sizeEvent);
        }
        if (auto winEvent = std::dynamic_pointer_cast<WinEvent>(e))
        {
            winNode->setVisible(true);
//            CULog("winEvent Event Got");
        }
        if (auto loseEvent = std::dynamic_pointer_cast<LoseEvent>(e))
        {
            loseNode->setVisible(true);
//            CULog("loseEvent Event Got");
        }
    }
#pragma mark END SOLUTION

    _world->update(FIXED_TIMESTEP_S);
}

/**
 * Executes the core gameplay loop of this world.
 *
 * This method contains the specific update code for this mini-game. It does
 * not handle collisions, as those are managed by the parent class WorldController.
 * This method is called after input is read, but before collisions are resolved.
 * The very last thing that it should do is apply forces to the appropriate objects.
 *
 * @param  delta    Number of seconds since last animation frame
 */
void GameScene::update(float dt)
{
    // deprecated
}

/**
 * Processes the start of a collision
 *
 * Since this game has no real need to determine collisions, right now this is left empty.
 *
 * @param  contact  The two bodies that collided
 */
void GameScene::beginContact(b2Contact *contact)
{
}

/**
 * Handles any modifications necessary before collision resolution
 *
 * This method is called just before Box2D resolves a collision.  We use this method
 * to implement sound on contact, using the algorithms outlined in Ian Parberry's
 * "Introduction to Game Physics with Box2D".
 *
 * @param  contact  	The two bodies that collided
 * @param  oldManfold  	The collision manifold before contact
 */
void GameScene::beforeSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
}

/**
 * Returns the active screen size of this scene.
 *
 * This method is for graceful handling of different aspect
 * ratios
 */
Size GameScene::computeActiveSize() const
{
    Size dimen = Application::get()->getDisplaySize();
    float ratio1 = dimen.width / dimen.height;
    float ratio2 = ((float)SCENE_WIDTH) / ((float)SCENE_HEIGHT);
    if (ratio1 < ratio2)
    {
        dimen *= SCENE_WIDTH / dimen.width;
    }
    else
    {
        dimen *= SCENE_HEIGHT / dimen.height;
    }
    return dimen;
}
void GameScene::addChildBackground()
{
    const std::vector<std::vector<std::shared_ptr<TileInfo>>> &currentBackground = _backgroundWrapper->getTileWorld();
    int originalRows = (int)currentBackground.size();
    int originalCols = (int)currentBackground.at(0).size();
    for (int j = 0; j < originalCols; j++)
    {
        for (int i = originalRows - 1; i > -1; i--)
        {
            std::shared_ptr<TileInfo> t = currentBackground.at(i).at(j);
            auto image = t->texture;
            auto sprite = scene2::PolygonNode::allocWithTexture(image);
            sprite->setContentSize(Vec2(1, 1));
            sprite->setPosition(t->getPosition());
            _worldnode->addChildWithName(sprite, "tileworld" + std::to_string(i) + std::to_string(j));
        }
    }
    const std::vector<std::vector<std::shared_ptr<TileInfo>>> &currentBoundaries = _backgroundWrapper->getBoundaryWorld();
    for (int i = 0; i < originalRows; i++)
    {
        for (int j = 0; j < originalCols; j++)
        {
            std::shared_ptr<TileInfo> t = currentBoundaries.at(i).at(j);
            if (t->texture != nullptr)
            {
                t->setDebugColor(DYNAMIC_COLOR);
                _world->initObstacle(t);
                t->setDebugScene(_debugnode);
                if (_isHost)
                {
                    _world->getOwnedObstacles().insert({t, 0});
                }
            }
        }
    }
    const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> &lowerDecorWorld = _backgroundWrapper->getLowerDecorWorld();
    for (int n = 0; n < lowerDecorWorld.size(); n++)
    {
        for (int i = 0; i < originalRows; i++)
        {
            for (int j = 0; j < originalCols; j++)
            {
                std::shared_ptr<TileInfo> t = lowerDecorWorld.at(n).at(i).at(j);
                if (t->texture != nullptr)
                {
                    auto image = t->texture;
                    auto sprite = scene2::PolygonNode::allocWithTexture(image);
                    sprite->setContentSize(Vec2(1, 1));
                    sprite->setPosition(t->getPosition());
                    _worldnode->addChild(sprite);
                }
            }
        }
    }
}
void GameScene::addChildForeground()
{

    const std::vector<std::vector<std::vector<std::shared_ptr<TileInfo>>>> &upperDecorWorld = _backgroundWrapper->getUpperDecorWorld();
    if (upperDecorWorld.size() == 0)
    {
        return;
    }
    int originalRows = (int)upperDecorWorld.at(0).size();
    int originalCols = (int)upperDecorWorld.at(0).at(0).size();
    for (int n = 0; n < upperDecorWorld.size(); n++)
    {
        for (int i = 0; i < originalRows; i++)
        {
            for (int j = 0; j < originalCols; j++)
            {
                std::shared_ptr<TileInfo> t = upperDecorWorld.at(n).at(i).at(j);
                if (t->texture != nullptr)
                {
                    auto image = t->texture;
                    auto sprite = scene2::PolygonNode::allocWithTexture(image);
                    sprite->setContentSize(Vec2(1, 1));
                    sprite->setPosition(t->getPosition());
                    _worldnode->addChildWithName(sprite, "decoration" + std::to_string(i) + " " + std::to_string(j));
                }
            }
        }
    }
}
