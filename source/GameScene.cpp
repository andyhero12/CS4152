//
//  SLGameScene.cpp
//
//  This is the primary class file for running the game.  You should study this file
//  for ideas on how to structure your own root class. This class is a reimagining of
//  the first game lab from 3152 in CUGL.
//
//  Author: Walker White
//  Based on original GameX Ship Demo by Rama C. Hoetzlein, 2002
//  Version: 1/20/22
//
#include <cugl/cugl.h>
#include <box2d/b2_world.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_collision.h>
#include <iostream>
#include <sstream>

#include "GameScene.h"
#include "CollisionController.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Level Layout

// Lock the screen size to fixed height regardless of aspect ratio
#define SCENE_HEIGHT 720
#define WORLD_SIZE 3
/** Width of the game world in Box2d units */
#define DEFAULT_WIDTH   1000.0f
/** Height of the game world in Box2d units */
#define DEFAULT_HEIGHT  1000.0f
#define DEFAULT_GRAVITY 0
/** Friction of most platforms */
#define BASIC_FRICTION  0.4f
/** The restitution for all physics objects */
#define BASIC_RESTITUTION   0.1f
// number of tiles of height to be displayed
#define CANVAS_TILE_HEIGHT 8
#define DRAW_WIREFRAME true
#define POSITION_ITERATIONS 18

#pragma mark -
#pragma mark Constructors
/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool GameScene::init(const std::shared_ptr<cugl::AssetManager> &assets)
{
    std::cout << "INIT GAMESCENE\n";
    transition = ScreenEnums::GAMEPLAY;
    obstacleWorld = physics2::ObstacleWorld::alloc(Rect(0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT),Vec2(0,DEFAULT_GRAVITY));
    obstacleWorld->activateCollisionCallbacks(true);
    obstacleWorld->onBeginContact = [this](b2Contact* contact) {
      beginContact(contact);
    };
    obstacleWorld->onEndContact = [this](b2Contact* contact) {
      endContact(contact);
    };
    //obstacleWorld->setPositionIterations(POSITION_ITERATIONS);
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_HEIGHT / dimen.height;
    if (assets == nullptr)
    {
        return false;
    }
    else if (!Scene2::init(dimen))
    {
        return false;
    }

    // Start up the input handler
    _assets = assets;
    _input.init();


    // Get the background image and constant values
    tile = assets->get<Texture>("tile");
    _constants = assets->get<JsonValue>("constants");

    overWorld.init(assets, getSize());
    std::vector<std::shared_ptr<cugl::Texture>> textures;

    _parser = LevelParser(_assets, assets->get<JsonValue>("ugly_level"));
    _parser.processLayers();

    // Init spawner controller
//    _spawnerController.init(_constants->get("spawner"), _parser.getSpawnersPos()); 
//    _spawnerController.setTexture(assets->get<Texture>("spawner"));
    
    
    // used to create progress bars
    std::shared_ptr<cugl::Texture> barImage = assets->get<Texture>("progress");
    
    float textureWidth = barImage->getWidth();
    float textureHeight = barImage->getHeight();
    
    std::shared_ptr<cugl::Texture> bg = barImage->getSubTexture(0/textureWidth, 320/textureWidth, 0/textureHeight, 45/textureHeight);
    std::shared_ptr<cugl::Texture> fg = barImage->getSubTexture(24/textureWidth, 296/textureWidth, 45/textureHeight, 90/textureHeight);
    std::shared_ptr<cugl::Texture> left_cap = barImage->getSubTexture(0/textureWidth, 24/textureWidth, 45/textureHeight, 90/textureHeight);
    std::shared_ptr<cugl::Texture> right_cap = barImage->getSubTexture(296/textureWidth, 320/textureWidth, 45/textureHeight, 90/textureHeight);
    
     _bar = cugl::scene2::ProgressBar::allocWithCaps(bg, fg, left_cap, right_cap);
    _bar->setProgress(0);
    _monsterController.setHealthBar(_bar);
    
    
    // init monster controller
    _monsterController.setMeleeAnimationData(_constants->get("basicEnemy"), assets);
    _monsterController.setBombAnimationData(_constants->get("bomb"), assets);
    
    // Get the bang sound
    _bang = assets->get<Sound>("bang");
    _laser = assets->get<Sound>("laser");
    _blast = assets->get<Sound>("blast");

    // Create and layout the health meter
    std::string msg = strtool::format("Health %d", overWorld.getDog()->getHealth());
    _text = TextLayout::allocWithText(msg, assets->get<Font>("pixel32"));
    _text->layout();
    std::string winMsg = strtool::format("You Win!");
    _textWin = TextLayout::allocWithText(winMsg, assets->get<Font>("pixel32"));
    _textWin->layout();
    std::string lossMsg = strtool::format("You Lose!");
    _textLose = TextLayout::allocWithText(lossMsg, assets->get<Font>("pixel32"));
    _textLose->layout();
    _collisions.init(getSize());
    createMap();
    reset();
    

    
    
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
        _active = false;
    }
}

#pragma mark -
#pragma mark Gameplay Handling
/**
 * Resets the status of the game so that we can play again.
 */
void GameScene::reset()
{
    createMap();
    overWorld.reset(getSize());
    _gameEnded = false;
    obstacleWorld->clear();
    for (auto& row : _world.overworld){
        for(auto& tile : row){
            if (tile->type == Terrain::IMPASSIBLE){
                obstacleWorld->addObstacle(tile);
            }
        }
    }
//    for (auto& row : _world.overworld) {
//        for (auto& tile : row) {
//            if (tile.type == World::Terrain::IMPASSIBLE) {
//                std::shared_ptr<cugl::physics2::BoxObstacle> boxObstacle = physics2::BoxObstacle::alloc(tile.boundaryRect.origin, Vec2(0.99, 0.99));
//                boxObstacle->setBodyType(b2_staticBody);
//                boxObstacle->setDensity(10.f);
//                boxObstacle->setFriction(BASIC_FRICTION);
//                boxObstacle->setRestitution(BASIC_RESTITUTION);
//                obstacleWorld->addObstacle(boxObstacle);
//            }
//        }
//    }
    obstacleWorld->addObstacle(overWorld.getDog());
    
    
//    _spawnerController.init(_constants->get("spawner"), _parser.getSpawnersPos());
    _monsterController.init(_constants->get("asteroids"), overWorld);
}

/**
 * The method called to update the game mode.
 *
 * This method contains any gameplay code that is not an OpenGL call.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void GameScene::update(float timestep)
{
    // Read the keyboard for each controller.
    _input.readInput_joystick();
    _input.readInput();

    if (_input.didPressReset())
    {
        reset();
    }
    if (_gameEnded)
    {
        return;
    }
    overWorld.update(_input, Size(_world.overworld.at(0).size() * 22, _world.overworld.size() * 22), timestep);

    _spawnerController.update(_monsterController, overWorld, timestep);
    _monsterController.update(getSize(), timestep, overWorld);

    _collisions.intraOverWorldCollisions(overWorld);
    _collisions.overWorldMonsterControllerCollisions(overWorld, _monsterController);
    _collisions.attackCollisions(overWorld, _monsterController, _spawnerController);

    std::shared_ptr<BaseSet> baseSet = overWorld.getBaseSet();
    // Update the health meter
    _text->setText(strtool::format("Health %d, Absorb %d, Base_Health %d Mode %s", overWorld.getDog()->getHealth(), overWorld.getDog()->getAbsorb(), overWorld.getBaseSet()->getFirstHealth(), overWorld.getDog()->getMode().c_str()));
    _text->layout();

    // Check if game ended
    if (_monsterController.isEmpty() && _spawnerController.win())
    {
        _gameEnded = true;
    }
    else if (overWorld.getDog()->getHealth() == 0)
    {
        _gameEnded = true;
    }
    else if (baseSet->baseLost())
    {
        _gameEnded = true;
    }

    _monsterController.postUpdate(getSize(), timestep);
    overWorld.postUpdate();

    // We may need to consider determinism in the future
    obstacleWorld->update(timestep);
}

void GameScene::createMap()
{
    std::vector<std::vector<int>> matrix = _parser.getTile();
    std::vector<std::vector<int>> other = _parser.getBoundaries();
    _world = World(Vec2(0, 0), matrix, other, tile);
}
/**
 * Draws all this scene to the given SpriteBatch.
 *
 * The default implementation of this method simply draws the scene graph
 * to the sprite batch.  By overriding it, you can do custom drawing
 * in its place.
 *
 * @param batch     The SpriteBatch to draw with.
 */
void GameScene::render(const std::shared_ptr<cugl::SpriteBatch> &batch)
{
    // For now we render 3152-style
    // DO NOT DO THIS IN YOUR FINAL GAME

    // shift camera to follow ship; draw ingame objects here
    cugl::Vec3 pos = cugl::Vec3();
    std::dynamic_pointer_cast<OrthographicCamera>(getCamera())->setZoom(SCENE_HEIGHT / CANVAS_TILE_HEIGHT);
    pos.add(overWorld.getDog()->getPosition());
    getCamera()->setPosition(pos);
    getCamera()->update();
    batch->begin(getCamera()->getCombined());
    _world.draw(batch);
    
    _spawnerController.draw(batch, getSize());
    _monsterController.draw(batch, getSize(), _assets->get<Font>("pixel32"));
    overWorld.draw(batch, getSize());
    // draw actions

    //draw physics
    if (DRAW_WIREFRAME) {
        for (const auto& obstacle : obstacleWorld->getObstacles()) {
            // Attempt to cast the shared_ptr<Obstacle> to shared_ptr<BoxObstacle>
            std::shared_ptr<physics2::BoxObstacle> boxObstacle = std::dynamic_pointer_cast<physics2::BoxObstacle>(obstacle);

            // Check if the cast was successful
            if (boxObstacle) {
                // The obstacle is indeed an instance of BoxObstacle, so do the specific action
                Color4 wireframeColor = Color4("green");
                batch->setColor(wireframeColor);
                batch->setTexture(nullptr);
                batch->outline(Rect(boxObstacle->getPosition(), boxObstacle->getDimension()));
            }
        }
    }

    // shift camera to draw for absolute positioning
    std::dynamic_pointer_cast<OrthographicCamera>(getCamera())->setZoom(1);
    getCamera()->setPosition(cugl::Vec3(getSize().width / 2.0f, getSize().height / 2.0f, 0));
    getCamera()->update();
    batch->setPerspective(getCamera()->getCombined());

    batch->setColor(Color4::BLACK);
    batch->drawText(_text, Vec2(10, getSize().height - _text->getBounds().size.height));
    batch->setColor(Color4::WHITE);
    cugl::Affine2 trans;
    float scale_factor = 3.0f;
    trans.scale(scale_factor);

    std::shared_ptr<BaseSet> baseSet = overWorld.getBaseSet();
    if (_monsterController.isEmpty() && _spawnerController.win())
    {
        trans.translate(Vec2(getSize().width / 2.0f - scale_factor * _textWin->getBounds().size.width / 2.0f, getSize().height / 2.0f));
        batch->setColor(Color4::GREEN);
        batch->drawText(_textWin, trans);
        batch->setColor(Color4::WHITE);
    }
    else if (overWorld.getDog()->getHealth() == 0 || baseSet->baseLost())
    {
        trans.translate(Vec2(getSize().width / 2.0f - scale_factor * _textLose->getBounds().size.width / 2.0f, getSize().height / 2.0f));
        batch->setColor(Color4::RED);
        batch->drawText(_textLose, trans);
        batch->setColor(Color4::WHITE);
    }
    batch->end();
}

ScreenEnums GameScene::getTransition() { return transition; }

void GameScene::resetTransition()
{
    transition = ScreenEnums::GAMEPLAY;
}



#pragma mark -
#pragma mark Collision Handling
/**
 * Processes the start of a collision
 *
 * This method is called when we first get a collision between two objects.  We use
 * this method to test if it is the "right" kind of collision.  In particular, we
 * use it to test if we make it to the win door.
 *
 * @param  contact  The two bodies that collided
 */
void GameScene::beginContact(b2Contact* contact) {
    b2Fixture* fix1 = contact->GetFixtureA();
    b2Fixture* fix2 = contact->GetFixtureB();

    b2Body* body1 = fix1->GetBody();
    b2Body* body2 = fix2->GetBody();

    std::string* fd1 = reinterpret_cast<std::string*>(fix1->GetUserData().pointer);
    std::string* fd2 = reinterpret_cast<std::string*>(fix2->GetUserData().pointer);

    physics2::Obstacle* bd1 = reinterpret_cast<physics2::Obstacle*>(body1->GetUserData().pointer);
    physics2::Obstacle* bd2 = reinterpret_cast<physics2::Obstacle*>(body2->GetUserData().pointer);
    
    if (bd1 == overWorld.getDog().get() || bd2 == overWorld.getDog().get()){
        std::cout << "CONTACT WITH WALL\n";
    }
//
//
//    // See if we have landed on the ground.
//    if ((_avatar->getSensorName() == fd2 && _avatar.get() != bd1) ||
//        (_avatar->getSensorName() == fd1 && _avatar.get() != bd2)) {
//        _avatar->setGrounded(true);
//        // Could have more than one ground
//        _sensorFixtures.emplace(_avatar.get() == bd1 ? fix2 : fix1);
//    }
//
//    // If we hit the "win" door, we are done
//    if((bd1 == _avatar.get()   && bd2 == _goalDoor.get()) ||
//        (bd1 == _goalDoor.get() && bd2 == _avatar.get())) {
//        setComplete(true);
//    }
}

/**
 * Callback method for the start of a collision
 *
 * This method is called when two objects cease to touch.  The main use of this method
 * is to determine when the characer is NOT on the ground.  This is how we prevent
 * double jumping.
 */
void GameScene::endContact(b2Contact* contact) {
    b2Fixture* fix1 = contact->GetFixtureA();
    b2Fixture* fix2 = contact->GetFixtureB();

    b2Body* body1 = fix1->GetBody();
    b2Body* body2 = fix2->GetBody();

    std::string* fd1 = reinterpret_cast<std::string*>(fix1->GetUserData().pointer);
    std::string* fd2 = reinterpret_cast<std::string*>(fix2->GetUserData().pointer);

    physics2::Obstacle* bd1 = reinterpret_cast<physics2::Obstacle*>(body1->GetUserData().pointer);
    physics2::Obstacle* bd2 = reinterpret_cast<physics2::Obstacle*>(body2->GetUserData().pointer);
//
//    if ((_avatar->getSensorName() == fd2 && _avatar.get() != bd1) ||
//        (_avatar->getSensorName() == fd1 && _avatar.get() != bd2)) {
//        _sensorFixtures.erase(_avatar.get() == bd1 ? fix2 : fix1);
//        if (_sensorFixtures.empty()) {
//            _avatar->setGrounded(false);
//        }
//    }
}
