//
//  pauseScene.hpp
//  Heaven
//
//  Created by Henry Gao on 4/9/24.
//

#ifndef PauseScene_h
#define PauseScene_h
#include <cugl/cugl.h>
#include "GlobalConstants.h"


#define PAUSE_SCALE 6



class PauseScene:  public cugl::scene2::SceneNode 
{
    
public:
    /**
     * The menu choice.
     *
     * This state allows the top level application to know what the user
     * chose.
     */
    enum Choice {
        /** User wants to host a game */
        GAME,
        /** User wants to join a game */
        EXIT
    };
private:
    
    Choice status;
    
    bool paused;

    /** The "pause" button */
    std::shared_ptr<cugl::scene2::Button>  resumeButton;
    std::shared_ptr<cugl::scene2::Button>  exitButton;
    std::shared_ptr<cugl::AssetManager> _assets;
    cugl::Size _screenSize;
    
public:
    
    PauseScene(){};
    
    bool init(std::shared_ptr<cugl::AssetManager> &assets, cugl::Size screenSize);
    bool init() override;
    
    void dispose() override;
    /**
     * Returns true if the gameplay controller is currently active
     *
     * @return true if the gameplay controller is currently active
     */
    bool getPause() const { return paused; }
    
    void exitToMain();

    /**
     * Returns true if the gameplay controller is currently active
     *
     * @return true if the gameplay controller is currently active
     */
    void setPause(bool value);
    
    Choice getStatus(){return status;}
    
    
};
#include <stdio.h>

#endif /* pauseScene_hpp */
