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
    Animation(int numDirections, std::vector<std::shared_ptr<cugl::SpriteSheet>> animTextures, float freq);
    Animation(int numDirections, std::vector<std::shared_ptr<cugl::SpriteSheet>> animTextures, float freq, int startFrame);
    ~Animation() {}
    
    void setFrame(int frame);

    bool frameUpdateReady();
    void resetAnimation(int aimingDir);
    void stepAnimation();
    void updateAnimTime(float dt);
    
    
    int prevAnimDir;
    float timeSinceLastAnim;
    /** Attack animation frequency */
    float animFreq;
    /** Number of animation directions for the entity */
    int numAnimDirections;
};
#endif /* Animation_hpp */
