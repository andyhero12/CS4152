//
//  MenuScene.h
//
//  Author: Zekai Li
//  Version: 3/28/24
//
#ifndef __SL_MAINMENU_SCENE_H__
#define __SL_MAINMENU_SCENE_H__
#include <cugl/cugl.h>
#include "GlobalConstants.h"
#include "InputController.h"
#include <chrono>

/**
 * This class is a simple loading screen for asychronous asset loading.
 *
 * The screen will display a very minimal progress bar that displays the
 * status of the asset manager.  Make sure that all asychronous load requests
 * are issued BEFORE calling update for the first time, or else this screen
 * will think that asset loading is complete.
 *
 * Once asset loading is completed, it will display a play button.  Clicking
 * this button will inform the application root to switch to the gameplay mode.
 */
class MenuScene : public cugl::Scene2 {
protected:

    /** The asset manager for loading. */
    std::shared_ptr<cugl::AssetManager> _assets;
    // NO CONTROLLER (ALL IN SEPARATE THREAD)
    InputController _input;
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
     * Creates a new loading mode with the default values.
     *
     * This constructor does not allocate any objects or start the game.
     * This allows us to use the object without a heap pointer.
     */
    MenuScene() : cugl::Scene2(), _progress(0.0f),lastInputTime(std::chrono::steady_clock::now() - std::chrono::milliseconds(500)) {}

    ~MenuScene() { dispose(); }

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose() override;

    /**
     * @param assets    The (loaded) assets for this game mode
     *
     * @return true if the controller is initialized properly, false otherwise.
     */
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);


#pragma mark -
#pragma mark Progress Monitoring
    /**
     * The method called to update the game mode.
     *
     * This method updates the progress bar amount.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void update(float timestep) override;

    ScreenEnums getTransition() override;

    void resetTransition() override;

    /**
     * @return true if loading is complete, but the player has not pressed play
     */
    bool isPending() const;
};

#endif /* __SL_MAINMENU_SCENE_H__ */
