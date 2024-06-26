//
//  NLApp.cpp
//  Networked Physics Demo
//
//  This is the root class for your game.  The file main.cpp accesses this class
//  to run the application.  While you could put most of your game logic in
//  this class, we prefer to break the game up into player modes and have a
//  class for each mode.
//
//  Author: Walker White
//  Version: 1/10/17
//
#include "NLApp.h"

using namespace cugl;


#pragma mark -
#pragma mark Application State

/**
 * The method called after OpenGL is initialized, but before running the application.
 *
 * This is the method in which all user-defined program intialization should
 * take place.  You should not create a new init() method.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to FOREGROUND,
 * causing the application to run.
 */
void NetApp::onStartup() {
    _assets = AssetManager::alloc();
    _batch  = SpriteBatch::alloc();
    
    // Start-up basic input
#ifdef CU_TOUCH_SCREEN
    Input::activate<Touchscreen>();
#else
    Input::activate<Mouse>();
#endif
    
    Input::activate<Keyboard>();
    Input::activate<TextInput>();
    
    _assets->attach<Font>(FontLoader::alloc()->getHook());
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Sound>(SoundLoader::alloc()->getHook());
    _assets->attach<scene2::SceneNode>(Scene2Loader::alloc()->getHook());
    _assets->attach<JsonValue>(JsonLoader::alloc()->getHook());
    _assets->attach<WidgetValue>(WidgetLoader::alloc()->getHook());
    _assets->attach<LevelModel>(GenericLoader<LevelModel>::alloc()->getHook());
    
    _loading.init(_assets);
    _status = LOAD;
    
    // Que up the other assets
    AudioEngine::start(24);
    _assets->loadDirectoryAsync("json/assets.json",nullptr);
    _assets->loadAsync<LevelModel>(LEVEL_ONE_KEY,LEVEL_ONE_FILE,nullptr);
    _assets->loadAsync<LevelModel>(LEVEL_TWO_KEY,LEVEL_TWO_FILE,nullptr);
    _assets->loadAsync<LevelModel>(LEVEL_THREE_KEY,LEVEL_THREE_FILE,nullptr);
    cugl::net::NetworkLayer::start(net::NetworkLayer::Log::INFO);
    
    Application::onStartup(); // YOU MUST END with call to parent
    setDeterministic(true);
}

/**
 * The method called when the application is ready to quit.
 *
 * This is the method to dispose of all resources allocated by this
 * application.  As a rule of thumb, everything created in onStartup()
 * should be deleted here.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to NONE,
 * causing the application to be deleted.
 */
void NetApp::onShutdown() {
    _gameplay.dispose();
    _mainmenu.dispose();
    _menu.dispose();
    _hostgame.dispose();
    _joingame.dispose();
    _assets = nullptr;
    _batch = nullptr;
    
    // Shutdown input
#ifdef CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif
    
	AudioEngine::stop();
	Application::onShutdown();  // YOU MUST END with call to parent
}

/**
 * The method called when the application is suspended and put in the background.
 *
 * When this method is called, you should store any state that you do not
 * want to be lost.  There is no guarantee that an application will return
 * from the background; it may be terminated instead.
 *
 * If you are using audio, it is critical that you pause it on suspension.
 * Otherwise, the audio thread may persist while the application is in
 * the background.
 */
void NetApp::onSuspend() {
    AudioEngine::get()->pause();
}

/**
 * The method called when the application resumes and put in the foreground.
 *
 * If you saved any state before going into the background, now is the time
 * to restore it. This guarantees that the application looks the same as
 * when it was suspended.
 *
 * If you are using audio, you should use this method to resume any audio
 * paused before app suspension.
 */
void NetApp::onResume() {
    AudioEngine::get()->resume();
}


#pragma mark -
#pragma mark Application Loop

void NetApp::preUpdate(float timestep){
//    std::cout << _status << std::endl;
    if (_status == LOAD && _loading.isActive()) {
        _loading.update(0.01f);
    }
    else if (_status == LOAD) {
        _network = NetEventController::alloc(_assets);
        _loading.dispose(); // Disables the input listeners in this mode
        _mainmenu.init(_assets);
        _menu.init(_assets);
        _level.init(_assets);
        _mainmenu.setActive(true);
        _hostgame.init(_assets,_network);
        _joingame.init(_assets,_network);
        //_gameplay.init(_assets);
        _status = MAINMENU;
    }
    else if(_status == MAINMENU){
        updateMainScene(timestep);
    }
    else if (_status == MENU) {
        updateMenuScene(timestep);
    }
    else if(_status == LEVEL){
        updateLevelScene(timestep);
    }
    else if (_status == HOST){
        updateHostScene(timestep);
    }
    else if (_status == CLIENT){
        updateClientScene(timestep);
    }
    else if (_status == GAME){
        updateGameScene(timestep);
    }

}

void NetApp::postUpdate(float timestep) {
    if (_status == GAME) {
        _gameplay.postUpdate(timestep);
    }
}

void NetApp::fixedUpdate() {
    if (_status == GAME) {
        _gameplay.fixedUpdate();
    }
    if(_network){
        _network->updateNet();
    }
}

/**
 * The method called to update the application data.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should contain any code that is not an OpenGL call.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void NetApp::update(float timestep) {
    //deprecated
}


/**
 * Inidividualized update method for the menu scene.
 *
 * This method keeps the primary {@link #update} from being a mess of switch
 * statements. It also handles the transition logic from the menu scene.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void NetApp::updateMenuScene(float timestep) {
    _menu.update(timestep);
    if (_menu.getBackclick()) {
        _menu.setActive(false);
        _mainmenu.setActive(true);
        _status = MAINMENU;
    }
    else{
        switch (_menu.getChoice()) {
        case MenuScene::Choice::HOST:
            _menu.setActive(false);
            _hostgame.setActive(true);
            _status = HOST;
            break;
        case MenuScene::Choice::JOIN:
            _menu.setActive(false);
            _joingame.setActive(true);
            _status = CLIENT;
            break;
        case MenuScene::Choice::NONE:
            // DO NOTHING
            break;
        }
    }
    
}

/**
 * Inidividualized update method for the host scene.
 *
 * This method keeps the primary {@link #update} from being a mess of switch
 * statements. It also handles the transition logic from the host scene.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void NetApp::updateHostScene(float timestep) {
    _hostgame.update(timestep);
    if(_hostgame.getBackClicked()){
        _status = MAINMENU;
        _hostgame.setActive(false);
        _mainmenu.setActive(true);
        _level.setActive(false);
        //_menu.setActive(true);
    }
    else if (_network->getStatus() == NetEventController::Status::HANDSHAKE && _network->getShortUID()) {
        switch (_level.getLevel()) {
            case LevelScene::Level::L1:
                _gameplay.init(_assets, _network, true, LEVEL_ONE_KEY);
                break;
            case LevelScene::Level::L2:
                _gameplay.init(_assets, _network, true, LEVEL_TWO_KEY);
                break;
            case LevelScene::Level::L3:
                _gameplay.init(_assets, _network, true, LEVEL_THREE_KEY);
                break;
            default :
                CUAssertLog(false, "bad level");
                break;
        }
        _network->markReady();
    }
    else if (_network->getStatus() == NetEventController::Status::INGAME) {
        _hostgame.setActive(false);
        _gameplay.setActive(true);
        _status = GAME;
    }
    else if (_network->getStatus() == NetEventController::Status::NETERROR) {
        _network->disconnect();
		_hostgame.setActive(false);
		_mainmenu.setActive(true);
        _gameplay.dispose();
		_status = MAINMENU;
	}
}

/**
 * Inidividualized update method for the client scene.
 *
 * This method keeps the primary {@link #update} from being a mess of switch
 * statements. It also handles the transition logic from the client scene.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void NetApp::updateClientScene(float timestep) {
    //TODO: Write transition logic for client scene
#pragma mark SOLUTION
    _joingame.update(timestep);
    if(_joingame.getBackClicked()){
        _status = MAINMENU;
        _joingame.setActive(false);
        _mainmenu.setActive(true);
    }
    else if (_network->getStatus() == NetEventController::Status::HANDSHAKE && _network->getShortUID()) {
        _gameplay.init(_assets, _network, false, LEVEL_TWO_KEY);
        _network->markReady();
    }
    else if (_network->getStatus() == NetEventController::Status::INGAME) {
        _joingame.setActive(false);
        _gameplay.setActive(true);
        _status = GAME;
    }
    else if (_network->getStatus() == NetEventController::Status::NETERROR) {
        _network->disconnect();
		_joingame.setActive(false);
		_mainmenu.setActive(true);
        _gameplay.dispose();
		_status = MAINMENU;
	}
#pragma mark END SOLUTION
}


void NetApp::updateGameScene(float timestep) {
    _gameplay.preUpdate(timestep);
    if(_gameplay.getStatus() == PauseScene::EXIT){
        _gameplay.dispose();
        _network->disconnect();
        _mainmenu.setActive(true);
        _hostgame.setActive(false);
        _hostgame.endGame();
        _joingame.setActive(false);
        _status = MAINMENU;
    }
}

void NetApp::updateMainScene(float timestep)
{
    _mainmenu.update(timestep);
    switch (_mainmenu.getChoice()) {
    case MainMenuScene::Choice::SINGLE:
        _mainmenu.setActive(false);
        _status = LEVEL;
        _level.resetLevel();
        break;
    case MainMenuScene::Choice::COOP:
        _mainmenu.setActive(false);
        //_menu.setActive(true);
        _joingame.setActive(true);
        _status = CLIENT;
        break;
    case MainMenuScene::Choice::SETTING:
        //_mainmenu.setActive(false);
        //_menu.setActive(true);
        //_status = MENU;
        break;
    case MainMenuScene::Choice::NONE:
        // DO NOTHING
        break;
    }
}

void NetApp::updateLevelScene(float timestep)
{
    _level.update(timestep);
    
    switch (_level.getLevel()) {
    case LevelScene::Level::L1:
        _level.setActive(false);
        _hostgame.setActive(true);
        _status = HOST;
        break;
    case LevelScene::Level::L2:
        _level.setActive(false);
        _hostgame.setActive(true);
        _status = HOST;
        break;
    case LevelScene::Level::L3:
        _level.setActive(false);
        _hostgame.setActive(true);
        _status = HOST;
        break;
    default :
        // DO NOTHING
        break;
    }
    
}

/**
 * The method called to draw the application to the screen.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should OpenGL and related drawing calls.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 */
void NetApp::draw() {
    switch (_status) {
        case LOAD:
            _loading.render(_batch);
            break;
        case MAINMENU:
            _mainmenu.render(_batch);
            break;
        case MENU:
            _menu.render(_batch);
            break;
        case LEVEL:
            _level.render(_batch);
            break;
        case HOST:
            _hostgame.render(_batch);
            break;
        case CLIENT:
            _joingame.render(_batch);
            break;
        case GAME:
            _gameplay.render(_batch);
        default:
            break;
    }
}

