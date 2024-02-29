//
//  SLGameScene.cpp
//  Ship Lab
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

#include "SLGameScene.h"
//#include "GLCollisionController.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Level Layout

// Lock the screen size to fixed height regardless of aspect ratio
#define SCENE_HEIGHT 720

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
    
    // Get the background image and constant values
    _background = assets->get<Texture>("background");
    _constants = assets->get<JsonValue>("constants");

    // Make a ship and set its texture
    _ship = std::make_shared<Ship>(getSize()/2, _constants->get("ship"));
    _ship->setTexture(assets->get<Texture>("ship"));

    // Initialize the asteroid set
    _asteroids.init(_constants->get("asteroids"));
    _asteroids.setTexture(assets->get<Texture>("asteroid1"));

    
    // Init spawner controller
    _spawnerController.init(_constants->get("spawner"));
    _spawnerController.setTexture(assets->get<Texture>("spawner"));

    // Initialize the Photon set
    _photons.init(_constants->get("photons"));
    _photons.setTexture(assets->get<Texture>("photon"));

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
    _ship->setPosition(getSize()/2);
    _ship->setAbsorbValue(0);
    _ship->setAngle(0);
    _ship->setVelocity(Vec2::ZERO);
    _ship->setHealth(_constants->get("ship")->getInt("health",0));
    _asteroids.init(_constants->get("asteroids"));
    _photons.init(_constants->get("photons"));
    _spawnerController.init(_constants->get("spawner"));
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
    _input.readInput();
    if (_input.didPressReset()) {
        reset();
    }
    if (_gameEnded){
        return;
    }
    if (_input.didPressFire() && _ship->canFireWeapon()){
        Vec2 p(_ship->getPosition().x,
                _ship->getPosition().y);
        float rads = M_PI*_ship->getAngle()/180.0f + M_PI_2;
        Vec2 v =
        _photons._speed * Vec2::forAngle(rads) +_ship->getVelocity();
        _ship->reloadWeapon();
        _photons.spawnPhoton(p,v);
        AudioEngine::get()->play("laser", _laser, false, _laser->getVolume(), true);
        _ship->subAbsorb(2);
    }
    
    // Move the ships and photons forward (ignoring collisions)
    _ship->move( _input.getForward(),  _input.getTurn(), getSize());
    
    // Move the asteroids
    _asteroids.update(getSize());
    
    // Move the photons
    _photons.update(getSize());
    _spawnerController.update(_asteroids);
    
    // Check for collisions and play sound
    if (_collisions.resolveCollision(_ship, _asteroids)) {
        AudioEngine::get()->play("bang", _bang, false, _bang->getVolume(), true);
    }
    
    // Check for collisions later for photons
    if (_collisions.resolveCollision(_photons, _asteroids)){
        AudioEngine::get()->play("blast", _blast, false, _blast->getVolume(), true);
    }
    // Update the health meter
    _text->setText(strtool::format("Health %d, Absorb %d", _ship->getHealth(), _ship->getAbsorb()));
    _text->layout();
    
    // Check if game ended
    if (_asteroids.isEmpty()){
        _gameEnded = true;
    }else if (_ship->getHealth() == 0){
        _gameEnded = true;
    }
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
    
    batch->draw(_background,Rect(Vec2::ZERO,getSize()));
    _asteroids.draw(batch,getSize());
    _spawnerController.draw(batch, getSize());
    _photons.draw(batch, getSize());
    _ship->draw(batch,getSize());


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
    
    if (_asteroids.isEmpty()){
        trans.translate(Vec2(getSize().width/2.0f - scale_factor * _textWin->getBounds().size.width/2.0f, getSize().height/2.0f));
        batch->setColor(Color4::GREEN);
        batch->drawText(_textWin,trans);
        batch->setColor(Color4::WHITE);
    }else if (_ship->getHealth() == 0){
        trans.translate(Vec2(getSize().width/2.0f - scale_factor * _textLose->getBounds().size.width/2.0f, getSize().height/2.0f));
        batch->setColor(Color4::RED);
        batch->drawText(_textLose, trans);
        batch->setColor(Color4::WHITE);
    }
    batch->end();
}

