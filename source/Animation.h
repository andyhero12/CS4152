//
//  Animation.hpp
//  Ship
//
//  Created by Henry Gao on 2/29/24.
//

#ifndef Animation_hpp
#define Animation_hpp

#include <stdio.h>
#include <vector>
#include <cugl/cugl.h>

class Animation {
private:
    /** Tracks the texture currently being used */
    std::shared_ptr<cugl::SpriteSheet> currentAnimSprite;
    /** Array of texture that represent the sprites for each walking direction */
    std::vector<std::shared_ptr<cugl::SpriteSheet>> animSprite;
    /** stores integers representing the current frame for each direction */
    std::vector<int> animSpriteFrames;

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
    int numAnimDirections;
    
    Animation(){}
    Animation(int numDirections, std::vector<std::shared_ptr<cugl::SpriteSheet>>& animTextures, float freq, int startFrame);
    ~Animation() {}
    
    void setFrame(int frame);

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
};
#endif /* Animation_hpp */
