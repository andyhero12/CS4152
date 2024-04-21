//
//  NLLoadingScene.cpp
//  Networked Physics Demo
//
//  This module provides a very barebones loading screen.  Most of the time you
//  will not need a loading screen, because the assets will load so fast.  But
//  just in case, this is a simple example you can use in your games.
//
//  We know from 3152 that you all like to customize this screen.  Therefore,
//  we have kept it as simple as possible so that it is easy to modify. In
//  fact, this loading screen uses the new modular JSON format for defining
//  scenes.  See the file "loading.json" for how to change this scene.
//
//  Author: Walker White
//  Version: 1/10/17
//
#include "LevelScene.h"

using namespace cugl;

/** This is the ideal size of the logo */
#define SCENE_SIZE  800

#pragma mark -
#pragma mark Constructors

/**
 * Initializes the controller contents, making it ready for loading
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool LevelScene::init(const std::shared_ptr<AssetManager> &assets)
{
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    // Lock the scene to a reasonable resolution
    if (dimen.width > dimen.height)
    {
        dimen *= SCENE_SIZE / dimen.width;
    }
    else
    {
        dimen *= SCENE_SIZE / dimen.height;
    }
    if (assets == nullptr)
    {
        return false;
    }
    else if (!Scene2::init(dimen))
    {
        return false;
    }
    _input.init_withlistener();
    // IMMEDIATELY load the splash screen assets
    _assets = assets;
    _assets->loadDirectory("json/level.json");
    std::shared_ptr<cugl::scene2::SceneNode> layer = assets->get<scene2::SceneNode>("level");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen

    _bar = std::dynamic_pointer_cast<scene2::ProgressBar>(assets->get<scene2::SceneNode>("level_bar"));
    //_brand = assets->get<scene2::SceneNode>("level_name");
    _button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("level_play"));
    _button->addListener([=](const std::string &name, bool down){
        CULog("Current Level %d  %d %d", level1, level2, level3);
        if(down && level1){
            _level = Level::L1;
        }else if (down && level2){
            _level = Level::L2;
        }else if (down && level3){
            _level = Level::L3;
        } });
    background = cugl::scene2::SpriteNode::allocWithSheet(_assets->get<cugl::Texture>("Background"), 1, 15);
    background->setScale(2.7);
    background->setPosition(0.5 * background->getSize());
    addChild(background);
    layer->setColor(Color4(0, 0, 0, 1));
    Application::get()->setClearColor(Color4(192, 192, 192, 255));
    addChild(layer);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LevelScene::dispose()
{
    // Deactivate the button (platform dependent)
    if (isPending())
    {
        _button->deactivate();
    }
    _button = nullptr;
    //_brand = nullptr;
    _bar->removeFromParent();
    _bar = nullptr;
    _assets = nullptr;
    _progress = 0.0f;
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
void LevelScene::update(float progress)
{
    _input.update();
    
    if (curMoveAnim <= moveCooldown){
        curMoveAnim++;
    }
    std::cout<<_input._Leftright<<std::endl;
    if(_input._Leftright == 1 && readyToChangeLevel()){
        _goright = true;
    }

    if(_input._Leftright == -1 && readyToChangeLevel()){
        _goleft = true;
    }

    if (_input.didPressRight()){
        CULog("Pressed Right");
    }
    if(_input.didPressRight() && readyToChangeLevel()){
        _goright = true;
    }
    if(_input.didPressLeft() && readyToChangeLevel()){
        _goleft = true;
    }
    
    updatelevelscene();
    resetgochange();
    if (level1 && readToAnim()){
        if(background->getFrame() != 2){
            resetAnimCD();
           if (background->getFrame() < 2){
               background->setFrame(background->getFrame() + 1);
           }else if (background->getFrame() >  2){
               background->setFrame(background->getFrame() - 1);
           }
       }
    }else if(level2 && readToAnim()) {
        if(background->getFrame() != 8){
            resetAnimCD();
            if (background->getFrame() < 8){
                background->setFrame(background->getFrame() + 1);
            }else if (background->getFrame() >  8){
                background->setFrame(background->getFrame() - 1);
            }
       }
    }else if(level3 && readToAnim()){
        if(background->getFrame() !=14){
            resetAnimCD();
            if (background->getFrame() < 14){
                background->setFrame(background->getFrame() + 1);
            }else if (background->getFrame() >  14){
                background->setFrame(background->getFrame() - 1);
            }
        }
    }
    if (_input._confirm &&readyToChangeLevel()){
        _button->setDown(true);
    }
    _input.resetcontroller();

    if (_progress < 1)
    {
        _progress = _assets->progress();
        if (_progress >= 1)
        {
            _progress = 1.0f;
            _bar->setVisible(false);
            //_brand->setVisible(false);
            _button->setVisible(false);
            _button->activate();
        }
        _bar->setProgress(_progress);
        _button->setVisible(false);
        //_brand->setVisible(false);
    }
}

/**
 * Returns true if loading is complete, but the player has not pressed play
 *
 * @return true if loading is complete, but the player has not pressed play
 */
bool LevelScene::isPending() const
{
    return _button != nullptr && _button->isVisible();
}

void LevelScene::setActive(bool value)
{

    if (isActive() != value)
    {
        Scene2::setActive(value);
        if (value)
        {
            _level = Level::NONE;
            _button->activate();
        }
        else
        {
            _button->deactivate();
            _button->setDown(false);
            _progress = 0.0f;
            frame = 0.0f;
        }
    }
}
