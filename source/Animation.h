//
//  Animation.hpp
//  Dog
//
//  Created by Henry Gao on 2/29/24.
//

#ifndef Animation_hpp
#define Animation_hpp

#include <stdio.h>
#include <vector>
#include <cugl/cugl.h>
#include <cassert>

class Animation {
private:
    /** Tracks the texture currently being used */
    std::shared_ptr<cugl::SpriteSheet> currentAnimSprite;
    /** Array of texture that represent the sprites for each walking direction */
    std::vector<std::shared_ptr<cugl::SpriteSheet>> animSprite;
public:
    /** Current frame number*/
    int frame;
    /** Index of the current animation direction*/
    int currentAnimationDirection;
    /** Number of frames that passed*/
    float timeSinceLastAnim;
    /** Attack animation frequency */
    float animFreq;
    /** Number of animation directions for the entity */
    // int numAnimDirections;
    
    Animation(){}
    Animation(std::vector<std::shared_ptr<cugl::SpriteSheet>>& animTextures, float freq, int startFrame);
    ~Animation() {}
    
    void setFrame(int frame);
    const int getFrame() const {return frame;}

    /**
    Returns true if enough time has passed since the last animation frame update
    @param
     */
    bool frameUpdateReady();
    
    /**
    Start the animation at a new direction
    @param aimingDir corresopnds to thei index of the sprite you want to draw
     */
    void resetAnimation(int aimingDir);
    /**
    Update frame number
     */
    void stepAnimation();
    /**
    Update numer of frames that have passed
     */
    void updateAnimTime();
    /**
    Returns the spritesheet of the Animation
     */
    const std::shared_ptr<cugl::SpriteSheet>& getSprite() const;
    
    void setOrigin(cugl::Vec2 &origin);
    
    int convertToQuadrant(double radian);
    
    void updateDirection();
    
    void update(double radian);
    void update(int prevDir);
    
};
#endif /* Animation_hpp */
