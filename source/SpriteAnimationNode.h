//
//  SpriteAnimationNode.hpp
//  Heaven
//
//  Created by Henry Gao on 4/17/24.
//

#ifndef SpriteAnimationNode_h
#define SpriteAnimationNode_h

#include <stdio.h>
#include <cugl/cugl.h>


class SpriteAnimationNode : public cugl::scene2::SpriteNode {
#pragma mark Helper Functions
private:
    /** Number of frames that passed*/
    int _timeSinceLastAnim;
    /** Attack animation frequency; represents the number of frames until update */
    int _animFreq;
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
    SpriteAnimationNode(){}
    /**
     * Initializes the film strip with the given texture.
     *
     * This initializer assumes that the filmstrip is rectangular, and that
     * there are no unused frames.
     *
     * The size of the node is equal to the size of a single frame in the
     * filmstrip. To resize the node, scale it up or down.  Do NOT change the
     * polygon, as that will interfere with the animation.
     *
     * @param texture   The texture image to use
     * @param rows      The number of rows in the filmstrip
     * @param cols      The number of columns in the filmstrip
     *
     * @return  true if the filmstrip is initialized properly, false otherwise.
     */
    bool initWithSheet(const std::shared_ptr<cugl::Texture>& texture, int rows, int cols, int freq) {
        return initWithSheet(texture,rows,cols,rows*cols, freq);
    }
    
    /**
     * Initializes the film strip with the given texture.
     *
     * The parameter size is to indicate that there are unused frames in the
     * filmstrip.  The value size must be less than or equal to rows*cols, or
     * this constructor will raise an error.
     *
     * The size of the node is equal to the size of a single frame in the
     * filmstrip. To resize the node, scale it up or down.  Do NOT change the
     * polygon, as that will interfere with the animation.
     *
     * @param texture   The texture image to use
     * @param rows      The number of rows in the filmstrip
     * @param cols      The number of columns in the filmstrip
     * @param size      The number of frames in the filmstrip
     *
     * @return  true if the filmstrip is initialized properly, false otherwise.
     */
    bool initWithSheet(const std::shared_ptr<cugl::Texture>& texture, int rows, int cols, int size, int freq);
  
    
    static std::shared_ptr<SpriteAnimationNode> allocWithSheet(const std::shared_ptr<cugl::Texture>& texture, int rows, int cols, int size, int freq) {
        std::shared_ptr<SpriteAnimationNode> node = std::make_shared<SpriteAnimationNode>();
        return (node->initWithSheet(texture, rows, cols, size, freq) ? node : nullptr);
    }
    
    static std::shared_ptr<SpriteAnimationNode> allocWithSheet(const std::shared_ptr<cugl::Texture>& texture, int rows, int cols, int freq) {
        return allocWithSheet(texture, rows, cols, rows*cols, freq);
    }
    
    void update();
    
};

#endif /* SpriteAnimationNode_hpp */
