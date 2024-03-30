//
//  TextureAnimation.hpp
//  Heavan
//
//  Created by Henry Gao on 3/29/24.
//

#ifndef TextureAnimation_h
#define TextureAnimation_h

#include <stdio.h>
#include <vector>
#include <cugl/cugl.h>

class TextureAnimation {
private:
    /** Tracks the texture currently being used */
    std::shared_ptr<cugl::Texture> currentTexture;
    /** Array of textures to form a sprite */
    std::vector<std::shared_ptr<cugl::Texture>> allTexture;
    /** */
    GLfloat minS;
    GLfloat maxS;
    GLfloat minT;
    GLfloat maxT;
    
    /** Current frame number*/
    int frame;
    /** Index of the current animation direction*/
    int currentAnimationDirection;
    /** Number of frames that passed*/
    float timeSinceLastAnim;
    /** Attack animation frequency */
    float animFreq;
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
    TextureAnimation(){}
    /** Defaults to using the entire texture*/
    TextureAnimation(std::vector<std::shared_ptr<cugl::Texture>>& animTextures, float freq, int startFrame);
    /** minS, maxS, minT, maxT range from 0 to 1, representing the percentage of the texture's width and height.*/
    TextureAnimation(std::vector<std::shared_ptr<cugl::Texture>>& animTextures, float freq, int startFrame, GLfloat minS, GLfloat maxS, GLfloat minT, GLfloat maxT);
    /** x1,y1 is the location of the pixel's top left corner  and x2,y2 is the location of the pixel's bottom right corner*/
    TextureAnimation(std::vector<std::shared_ptr<cugl::Texture>>& animTextures, float freq, int startFrame, int x1, int x2, int y1, int y2);
    
    ~TextureAnimation() {}
    

    
    void updateTexture();
};
#endif /* TextureAnimation_hpp */
