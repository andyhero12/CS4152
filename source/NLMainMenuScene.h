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
#ifndef __NL_MAINMENU_SCENE_H__
#define __NL_MAINMENU_SCENE_H__
#include <cugl/cugl.h>
#include <vector>
#include <chrono>
#include "GlobalConstants.h"
#include "NLInput.h"


/**
 * This class presents the menu to the player.
 *
 * There is no need for an input controller, as all input is managed by
 * listeners on the scene graph.  We only need getters so that the main
 * application can retrieve the state and communicate it to other scenes.
 */
class MainMenuScene : public cugl::Scene2 {
public:
    /**
     * The menu choice.
     *
     * This state allows the top level application to know what the user
     * chose.
     */
    enum Choice {
        /** User has not yet made a choice */
        SINGLE,
        /** User wants to host a game */
        COOP,
        /** User wants to join a game */
        SETTING,

        NONE
    };
    enum Isdown {
        /** User has not yet made a choice */
        isSINGLE,
        /** User wants to host a game */
        isCOOP,
        /** User wants to join a game */
        isSETTING,

        isNONE
    };

protected:
    /** The asset manager for this scene. */
    std::shared_ptr<cugl::AssetManager> _assets;
    /** The menu button for hosting a game */
    std::shared_ptr<cugl::scene2::Button> _hostbutton;
    /** The menu button for joining a game */
    std::shared_ptr<cugl::scene2::Button> _joinbutton;
    /** The player menu choice */
    Choice _choice;
    Isdown _isdown;

    InputController _input;
    // NO CONTROLLER (ALL IN SEPARATE THREAD)
    //InputController _input;
    std::chrono::steady_clock::time_point lastInputTime;
    // VIEW
    ///** The animated progress bar */
    //std::shared_ptr<cugl::scene2::ProgressBar>  _bar;
    ///** The engine name */
    //std::shared_ptr<cugl::scene2::SceneNode>  _brand;
    /** The "play" button */
    std::shared_ptr<cugl::scene2::Button>    _button1;
    std::shared_ptr<cugl::scene2::Button>    _button2;
    std::shared_ptr<cugl::scene2::Button>    _button3;
    std::vector<std::shared_ptr<cugl::scene2::Button>> _buttonset;

    // MODEL
    /** The progress displayed on the screen */
    float _progress;
    /** Whether or not the player has pressed play to continue */
    bool  _completed;

    bool _firstset;

    int _counter;

    float timeSinceLastSwitch;

    float switchFreq;

    ScreenEnums transition;
    
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new  menu scene with the default values.
     *
     * This constructor does not allocate any objects or start the game.
     * This allows us to use the object without a heap pointer.
     */
    MainMenuScene() : cugl::Scene2() {}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~MainMenuScene() { dispose(); }
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose() override;
    
    /**
     * Initializes the controller contents.
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
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    /**
     * Sets whether the scene is currently active
     *
     * This method should be used to toggle all the UI elements.  Buttons
     * should be activated when it is made active and deactivated when
     * it is not.
     *
     * @param value whether the scene is currently active
     */
    virtual void setActive(bool value) override;
    
    /**
     * Returns the user's menu choice.
     *
     * This will return NONE if the user had no yet made a choice.
     *
     * @return the user's menu choice.
     */
    Choice getChoice() const { return _choice; }
    
    virtual void update(float timestep) override;
};

#endif /* __NL_MENU_SCENE_H__ */
