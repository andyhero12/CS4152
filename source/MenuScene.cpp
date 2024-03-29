//
//  MenuScene.cpp
//  Author: Walker White
//  Version: 1/20/22
//
#include "MenuScene.h"

using namespace cugl;

/** This is the ideal size of the logo */
#define SCENE_SIZE  1024
#define SCENE_HEIGHT  800
#pragma mark -
#pragma mark Constructors

/**
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool MenuScene::init(const std::shared_ptr<AssetManager>& assets) {
    //transition = ScreenEnums::LOADING;
    // Initialize the scene to a locked width
    
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_HEIGHT / dimen.height;
    if (assets == nullptr) {
        return false;
    }
    else if (!Scene2::init(dimen)) {
        return false;
    }

    // IMMEDIATELY load the splash screen assets
    _assets = assets;
    _assets->loadDirectory("json/mainassets.json");
    std::shared_ptr<scene2::SceneNode> layer = _assets->get<scene2::SceneNode>("Menu");
    std::cout << dimen.width << "  " << dimen.height << std::endl;
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    
    _button1 = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("Menu_startmenu_startmenu_menu_button1"));
    _button2 = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("Menu_startmenu_startmenu_menu_button2"));
    _button3 = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("Menu_startmenu_startmenu_menu_button3"));
    
    _active = false;
    _button1->addListener([=](const std::string& name, bool down) {
        if (down) {
            std::cout << "Play" << std::endl;
            transition = ScreenEnums::GAMEPLAY;
        }
        });

     _button2->addListener([=](const std::string& name, bool down) {
        if (down) {
            std::cout << "Level" << std::endl;
            transition = ScreenEnums::GAMEPLAY;
        }
        });

     _button3->addListener([=](const std::string& name, bool down) {
        if (down) {
            std::cout << "Setting" << std::endl;
            transition = ScreenEnums::GAMEPLAY;
        }
        });    

    _firstset = false;

    Application::get()->setClearColor(Color4(192, 192, 192, 255));
    addChild(layer);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void MenuScene::dispose() {
    // Deactivate the button (platform dependent)
    if (isPending()) {
        _button1->deactivate();
    }
    _button1= nullptr;
    _button2 = nullptr;
    _button3 = nullptr;
    _assets = nullptr;
    _firstset = false;
}


#pragma mark -
#pragma mark Progress Monitoring
/**
 * The method called to update the game mode.
 *
 * This method updates the progress bar amount.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void MenuScene::update(float progress) {
    if (!_firstset) {
        _button1->activate();
        _button2->activate();
        _button3->activate();
        _firstset = true;
    }

}



/**
 * Returns true if loading is complete, but the player has not pressed play
 *
 * @return true if loading is complete, but the player has not pressed play
 */
bool MenuScene::isPending() const {
    return _button1 != nullptr && _button1->isVisible();
}



ScreenEnums MenuScene::getTransition() {
    return transition;
}

void MenuScene::resetTransition() {
    transition = ScreenEnums::LOADING;
}
