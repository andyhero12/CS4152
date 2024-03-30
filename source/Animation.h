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
    /** Current frame number*/
    int frame;
    /** Index of the current animation direction*/
    int currentAnimationDirection;
    /** Number of frames that passed*/
    float timeSinceLastAnim;
    /** Attack animation frequency; represents the number of frames until update */
    float animFreq;
    
    int convertToQuadrant(double radian);
    /**
    Returns true if enough time has passed since the last animation frame update
    @param
     */
    bool frameUpdateReady();
    /**
    Update frame number
     */
    void stepAnimation();
    /**
    Update numer of frames that have passed
     */
    void updateAnimTime();
public:
    /** Number of animation directions for the entity */
    // int numAnimDirections;
    
    Animation(){}
    /**
    Initializes a Animation Object
    @param freq is the number of frames until the sprite steps to the next animation
     */
    Animation(std::vector<std::shared_ptr<cugl::SpriteSheet>>& animTextures, float freq, int startFrame);
    ~Animation() {}
    
    void setFrame(int frame);
    const int getFrame() const {return frame;}
    const int getCurrentDirection() const {return currentAnimationDirection;}
    /**
    Returns the spritesheet of the Animation
     */
    const std::shared_ptr<cugl::SpriteSheet>& getSprite() const;

    /**
     Specifies the origin of EVERY sprite
     */
    void setOrigin(cugl::Vec2 &origin);
    
    /**
    Start the animation at a new direction
    @param aimingDir corresopnds to thei index of the sprite you want to draw
     */
    void resetAnimation(int aimingDir);
    /**
     Updates the animation in the current direction
     */
    void updateCurrentDirection();
    /**
     Changes the animation based on the angle provided. Afterwards updates the animation corresponding to the specified angle
     @param radian an angle in radians; can retrieve via vector.getAngle()
     */
    void updateByAngle(double radian);
    /**
     Changes the animation based on the direction provided.. Afterwards updates the animation corresponding to the specified angle
     @param prevDir the direction you want the character to move in. prevDir should be in the range (0, # direction - 1 )
     */
    void updateByDirection(int prevDir);
    
};
#endif /* Animation_hpp */
