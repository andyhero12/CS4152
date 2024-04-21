//
//  NLMenuScene.h
//  Network Lab
//
//  This class presents the initial menu scene.  It allows the player to
//  choose to be a host or a client.
//
//  Author: Walker White, Aidan Hobler
//  Version: 2/8/22
//
#include <cugl/cugl.h>
#include <iostream>
#include <sstream>

#include "NLMainMenuScene.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Level Layout

/** Regardless of logo, lock the height to this */
#define SCENE_HEIGHT  800


#pragma mark -
#pragma mark Constructors
/**
 * Initializes the controller contents, and starts the game
 *
 * In previous labs, this method "started" the scene.  But in this
 * case, we only use to initialize the scene user interface.  We
 * do not activate the user interface yet, as an active user
 * interface will still receive input EVEN WHEN IT IS HIDDEN.
 *
 * That is why we have the method {@link #setActive}.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool MainMenuScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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
    _input.init_withlistener();
    // Acquire the scene built by the asset loader and resize it the scene
    //std::shared_ptr<scene2::SceneNode> scene = _assets->get<scene2::SceneNode>("menu");
    //scene->setContentSize(dimen);
    //scene->doLayout(); // Repositions the HUD
    //_choice = Choice::NONE;
    //_hostbutton = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("menu_host"));
    //_joinbutton = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("menu_join"));
    
    _assets->loadDirectory("json/mainmenuassets.json");
    std::shared_ptr<scene2::SceneNode> layer = _assets->get<scene2::SceneNode>("Menu");
    std::cout << dimen.width << "  " << dimen.height << std::endl;
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    _buttonset.push_back(_button1 = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("Menu_startmenu_button1")));
    _buttonset.push_back(_button2 = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("Menu_startmenu_button2")));
    _buttonset.push_back(_button3 = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("Menu_startmenu_button3")));
    // Program the buttons
    _button1->addListener([this](const std::string& name, bool down) {
        //std::cout << _input._confirm << std::endl;
        if (down) {  
            if(_input.getState()==InputController::State::CONTROLLER){
                _isdown = Isdown::isSINGLE;
            }
            else{
                _choice = Choice::SINGLE;
            }
            
        }
    });
    _button2->addListener([this](const std::string& name, bool down) {
        if (down) {
            if(_input.getState()==InputController::State::CONTROLLER){
                _isdown = Isdown::isCOOP;
            }
            else{
                _choice = Choice::COOP;
            }
        }
    });

    _button3->addListener([this](const std::string& name, bool down) {
        if (down) {
            //_choice = Choice::LEVEL;
        }
        });
    _counter = 0;
    switchFreq = 0.2;
    _isdown = Isdown::isNONE;
    addChild(layer);
    setActive(false);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void MainMenuScene::dispose() {
    if (_active) {
        removeAllChildren();
        _active = false;
    }
}


/**
 * Sets whether the scene is currently active
 *
 * This method should be used to toggle all the UI elements.  Buttons
 * should be activated when it is made active and deactivated when
 * it is not.
 *
 * @param value whether the scene is currently active
 */
void MainMenuScene::setActive(bool value) {
    if (isActive() != value) {
        Scene2::setActive(value);
        if (value) {
            _choice = Choice::NONE;
            _isdown = Isdown::isNONE;
            //_hostbutton->activate();
            //_joinbutton->activate();
            _button1->activate();
            _button2->activate();
            _button3->activate();
        } else {
            _button1->deactivate();
            _button2->deactivate();
            _button3->deactivate();
            _button1->setDown(false);
            _button2->setDown(false);
            _button3->setDown(false);
            //_hostbutton->deactivate();
            //_joinbutton->deactivate();
            // If any were pressed, reset them
            //_hostbutton->setDown(false);
            //_joinbutton->setDown(false);
        }
    }
}

void MainMenuScene::update(float timestep)
{
    timeSinceLastSwitch += timestep;
    //std::cout << timeSinceLastSwitch << std::endl;
    if (timeSinceLastSwitch >= switchFreq) {
        if (_input._updown != 0) {
            if (_input._updown == 1 && _counter > 0) {
                _buttonset.at(_counter)->setDown(false);
                _counter--;
                _buttonset.at(_counter)->setDown(true);
            }
            else if (_input._updown == -1 && _counter < _buttonset.size() - 1) {
                _buttonset.at(_counter)->setDown(false);
                _counter++;
                _buttonset.at(_counter)->setDown(true);
            }
            timeSinceLastSwitch = 0;

        }
    }
    //std::cout << _input._confirm << std::endl;
    if (_isdown == Isdown::isSINGLE&&_input._confirm) {
        _choice = Choice::SINGLE;
    }
    else if(_isdown == Isdown::isCOOP && _input._confirm){
        _choice = Choice::COOP;
    }
    else if (_isdown == Isdown::isSETTING && _input._confirm) {
        _choice = Choice::SETTING;
    }
    else if (_isdown == Isdown::isNONE && _input._confirm) {
    }

    _input.resetcontroller();
}
