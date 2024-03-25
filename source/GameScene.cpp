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
bool GameScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    std::cout << "INIT GAMESCENE\n";
    transition = ScreenEnums::GAMEPLAY;
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_HEIGHT/dimen.height;
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
    
    // Start up the input handler
    _assets = assets;
    _input.init();

    // Get the background image and constant values
    sand = assets->get<Texture>("sand");
    water = assets->get<Texture>("water");
    tile = assets->get<Texture>("tile");
    _constants = assets->get<JsonValue>("constants");

    overWorld.init(assets, getSize());
    // Make a ship and set its texture
    _ship = overWorld.getDog();
    std::vector<std::shared_ptr<cugl::Texture>> textures;
    
    // Init spawner controller
    _spawnerController.init(_constants->get("spawner"));
    _spawnerController.setTexture(assets->get<Texture>("spawner"));

    _monsterController.setMeleeAnimationData(_constants->get("asteroids"), assets);
    _monsterController.setBombAnimationData(_constants->get("bomb"), assets);
    
    // Get the bang sound
    _bang = assets->get<Sound>("bang");
    _laser = assets->get<Sound>("laser");
    _blast = assets->get<Sound>("blast");
    
    // Create and layout the health meter
    std::string msg = strtool::format("Health %d", _ship->getHealth());
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
void GameScene::dispose() {
    if (_active) {
        removeAllChildren();
        _active = false;
    }
}


#pragma mark -
#pragma mark Gameplay Handling
/**
 * Resets the status of the game so that we can play again.
 */
void GameScene::reset() {
    _gameEnded = false;
    _spawnerController.init(_constants->get("spawner"));
    _monsterController.init(_constants->get("asteroids"),overWorld);
    
}

/**
 * The method called to update the game mode.
 *
 * This method contains any gameplay code that is not an OpenGL call.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void GameScene::update(float timestep) {
    // Read the keyboard for each controller.
    _input.readInput_joystick();
    _input.readInput();

    if (_input.didPressReset()) {
        overWorld.reset(getSize());
        reset();
    }
    if (_gameEnded){
        return;
    }
    overWorld.update(_input, getSize(),timestep);
    
//    if (_input.didPressFire() && _ship->canFireWeapon()){
//        AudioEngine::get()->play("laser", _laser, false, _laser->getVolume(), true);
//    }
//    
    _spawnerController.update(_monsterController,overWorld, timestep);
    _monsterController.update(getSize(), timestep, overWorld);
    
    _collisions.intraOverWorldCollisions(overWorld);
    _collisions.overWorldMonsterControllerCollisions(overWorld, _monsterController);
    _collisions.attackCollisions(overWorld, _monsterController, _spawnerController);
    
    std::shared_ptr<BaseSet> baseSet = overWorld.getBaseSet();
    // Update the health meter
    _text->setText(strtool::format("Health %d, Absorb %d, Base_Health %d Mode %s", _ship->getHealth(), _ship->getAbsorb(), overWorld.getBaseSet()->getFirstHealth(), _ship->getMode().c_str()));
    _text->layout();
    
    // Check if game ended
    if (_monsterController.isEmpty() && _spawnerController.win()){
        _gameEnded = true;
    }else if (_ship->getHealth() == 0){
        _gameEnded = true;
    }else if (baseSet->baseLost()){
        _gameEnded = true;
    }
    _monsterController.postUpdate(getSize(), timestep);
    overWorld.postUpdate();
}


void GameScene::createMap(){
    const int rows = 10;
     const int cols = 10;
     std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));

     for (int i = 0; i < rows; ++i) {
         for (int j = 0; j < cols; ++j) {
             matrix[i][j] = rand() % 2; // Assign random 0 or 1
         }
     }
    
    std::vector<std::vector<int>> other(rows, std::vector<int>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            other[i][j] = (rand() % 10) + 1; // Assign random 0 or 1
        }
    }
    
    _world = World(Vec2(0, 0), other , matrix, tile);
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
void GameScene::render(const std::shared_ptr<cugl::SpriteBatch>& batch) {
    // For now we render 3152-style
    // DO NOT DO THIS IN YOUR FINAL GAME

    //shift camera to follow ship; draw ingame objects here
    cugl::Vec3 pos = cugl::Vec3();
    pos.add(_ship->getPosition());
    getCamera()->setPosition(pos);
    getCamera()->update();
    batch->begin(getCamera()->getCombined());
    //draw bg
//    CULog("%d %d", bgCellX, bgCellY);
//    for (int i = -2; i <= 30; i++) {
//        for (int j = -2; j <= 30; j++) {
//            Color4 tint;
//                tint = Color4("white");
//            batch->draw(_background, tint, Rect(Vec2(38*0, 38*j), Size(40,40)));
//        }
//    }
    _world.draw(batch);
    _spawnerController.draw(batch, getSize());
    _monsterController.draw(batch, getSize(),_assets->get<Font>("pixel32"));
    overWorld.draw(batch, getSize());
    // draw actions
    
    // shift camera to draw for absolute positioning
    getCamera()->setPosition(cugl::Vec3(getSize().width / 2.0f, getSize().height / 2.0f, 0));
    getCamera()->update();
    batch->setPerspective(getCamera()->getCombined());


    batch->setColor(Color4::BLACK);
    batch->drawText(_text,Vec2(10,getSize().height-_text->getBounds().size.height));
    batch->setColor(Color4::WHITE);
    cugl::Affine2 trans;
    float scale_factor = 3.0f;
    trans.scale(scale_factor);
    
    std::shared_ptr<BaseSet> baseSet = overWorld.getBaseSet();
    if (_monsterController.isEmpty() && _spawnerController.win()){
        trans.translate(Vec2(getSize().width/2.0f - scale_factor * _textWin->getBounds().size.width/2.0f, getSize().height/2.0f));
        batch->setColor(Color4::GREEN);
        batch->drawText(_textWin,trans);
        batch->setColor(Color4::WHITE);
    }else if (_ship->getHealth() == 0 || baseSet->baseLost()){
        trans.translate(Vec2(getSize().width/2.0f - scale_factor * _textLose->getBounds().size.width/2.0f, getSize().height/2.0f));
        batch->setColor(Color4::RED);
        batch->drawText(_textLose, trans);
        batch->setColor(Color4::WHITE);
    }
    batch->end();
}

ScreenEnums GameScene::getTransition(){return transition;}


void GameScene::resetTransition(){
    transition = ScreenEnums::GAMEPLAY;
}
