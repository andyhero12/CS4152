 
// //
// //  levelselection.h
// //
// //  Author: Zekai Li
// //  Version: 3/28/24
// //
// #ifndef __SL_MAINMENU_SCENE_H__
// #define __SL_MAINMENU_SCENE_H__
// #include <cugl/cugl.h>
// #include "GlobalConstants.h"


// class MenuScene : public cugl::Scene2 {
// protected:
//     /** The asset manager for loading. */
//     std::shared_ptr<cugl::AssetManager> _assets;
//     // NO CONTROLLER (ALL IN SEPARATE THREAD)

//     // VIEW
//     ///** The animated progress bar */
//     //std::shared_ptr<cugl::scene2::ProgressBar>  _bar;
//     ///** The engine name */
//     //std::shared_ptr<cugl::scene2::SceneNode>  _brand;
//     /** The "play" button */
//     std::shared_ptr<cugl::scene2::Button>    _button1;
//     std::shared_ptr<cugl::scene2::Button>    _button2;
//     std::shared_ptr<cugl::scene2::Button>    _button3;
    

//     // MODEL
//     /** The progress displayed on the screen */
//     float _progress;
//     /** Whether or not the player has pressed play to continue */
//     bool  _completed;

//     bool _firstset;

//     ScreenEnums transition;


// public:
// #pragma mark -
// #pragma mark Constructors
//     /**
//      * Creates a new loading mode with the default values.
//      *
//      * This constructor does not allocate any objects or start the game.
//      * This allows us to use the object without a heap pointer.
//      */
//     MenuScene() : cugl::Scene2(), _progress(0.0f) {}

//     /**
//      * Disposes of all (non-static) resources allocated to this mode.
//      *
//      * This method is different from dispose() in that it ALSO shuts off any
//      * static resources, like the input controller.
//      */
//     ~MenuScene() { dispose(); }

//     /**
//      * Disposes of all (non-static) resources allocated to this mode.
//      */
//     void dispose() override;

//     /**
//      * Initializes the controller contents, making it ready for loading
//      *
//      * @param assets    The (loaded) assets for this game mode
//      *
//      * @return true if the controller is initialized properly, false otherwise.
//      */
//     bool init(const std::shared_ptr<cugl::AssetManager>& assets);


// #pragma mark -
// #pragma mark Progress Monitoring
//     /**
//      * The method called to update the game mode.
//      *
//      * This method updates the progress bar amount.
//      *
//      * @param timestep  The amount of time (in seconds) since the last frame
//      */
//     void update(float timestep) override;

//     ScreenEnums getTransition() override;

//     void resetTransition() override;

//     /**
//      * Returns true if loading is complete, but the player has not pressed play
//      *
//      * @return true if loading is complete, but the player has not pressed play
//      */
//     bool isPending() const;
// };

// #endif /* __SL_MAINMENU_SCENE_H__ */