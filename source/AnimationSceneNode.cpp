//
//  AnimationScene.cpp
//  Heaven
//
//  Created by Henry Gao on 4/1/24.
//

#include "AnimationSceneNode.h"

#pragma mark -
#pragma mark Constructors

/**
 * Constructs a ActionNode with no filmstrip
 *
 * You must initialize this object before use.
 *
 * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate an object on
 * the heap, use one of the static constructors instead.
 */
AnimationSceneNode::AnimationSceneNode() :
SpriteNode(),
_animationSprites(8),
_frame(0),
_on(true)
{
    _classname = "AnimationNode";
}


void AnimationSceneNode::dispose(){
    for (auto& sprite : _animationSprites) {
        sprite->dispose();
    }
}

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
bool AnimationSceneNode::initWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& textures,
                                          int rows, int cols, int size, int freqAnimation) {
    CUAssertLog(size <= rows*cols, "Invalid strip size for %dx%d", rows,cols);
    this->_animFreq = freqAnimation;
    this->_timeSinceLastAnim = 0;
    this->_on = false;
    _direction = Directions::SOUTH;
    createSpriteNodes(textures,rows, cols);
    return this->initWithSheet(textures.at(0),rows, cols);
}


#pragma mark -
#pragma mark Attribute Accessors

void AnimationSceneNode::setFrame(int frame){
    _frame = frame;
}

const std::shared_ptr<cugl::scene2::SpriteNode>& AnimationSceneNode::getAnimation(Directions direction) const {
    size_t index = static_cast<std::size_t>(direction);
    return _animationSprites.at(index);
}

bool AnimationSceneNode::frameUpdateReady() {
    if(_timeSinceLastAnim > (_animFreq)){
        _timeSinceLastAnim = 0;
        return true;
    }
    return false;
}

void AnimationSceneNode::updateAnimTime(){
    _timeSinceLastAnim += (_timeSinceLastAnim <= (_animFreq) ? 1 : 0);
}

void AnimationSceneNode::stepAnimation(){
    _frame = (_frame + 1)% _size;
}

void AnimationSceneNode::setAnchor(const cugl::Vec2 anchor){
    _anchor = anchor;
//    std::cout << "animation" << anchor.x << " " <<anchor.y << std::endl;
    for(auto &node : _animationSprites){
        node->setAnchor(anchor);
    }
}

void AnimationSceneNode::setContentSize(const cugl::Size size) {
    SpriteNode::setContentSize(size);
//    _contentSize = size;
//    for (std::shared_ptr<cugl::scene2::SpriteNode> &node : _animationSprites) {
//        node->setContentSize(size);
//    }
}

void AnimationSceneNode::setPosition(const cugl::Vec2 &position){
    _position = position;
    for(std::shared_ptr<cugl::scene2::SpriteNode> &node : _animationSprites){
        node->setPosition(position);
    }
}

void AnimationSceneNode::setAngle(float angle) {
    _angle = angle;
    for(auto &node : _animationSprites){
        node->setAngle(_angle);
    }
}
#pragma mark -
#pragma mark Helper Functions

/**
 initializes the animationSprites vector
 */
bool AnimationSceneNode::createSpriteNodes(const std::vector<std::shared_ptr<cugl::Texture>>& textures, int rows, int cols){
    CUAssertLog(textures.size()==8, "Invalid number of elements in to initialize a spriteNode, need 8 not %zu", textures.size());
    for (size_t i = 0; i < textures.size(); ++i) {
        _animationSprites.at(i) = cugl::scene2::SpriteNode::allocWithSheet(textures.at(i), rows, cols);
    }
    _height = _animationSprites.at(0)->getHeight()/rows;
    _width = _animationSprites.at(0)->getHeight()/cols;
    return true;
}



/** Convert radians to Direction */
AnimationSceneNode::Directions AnimationSceneNode::convertRadiansToDirections(double rad){
    return convertAngleToDirections(rad * (180 / M_PI));
}

/** Convert Angle to Direction: 0 radians is east */
AnimationSceneNode::Directions AnimationSceneNode::convertAngleToDirections(float ang){
    ang += 90.0f;
    if (ang >= 360){
         ang -= 360.0f;
     }
    if (ang < 0) {
        ang += 360.0f;
    }
    if (ang <= 0) return Directions::SOUTH;
    else if (ang <= 45.0f) return Directions::SOUTHEAST;
    else if (ang <= 90.0f) return Directions::EAST;
    else if (ang <= 135.0f) return Directions::NORTHEAST;
    else if (ang <= 180.0f) return Directions::NORTH;
    else if (ang <= 225.0f) return Directions::NORTHWEST;
    else if (ang <= 270.0f) return Directions::WEST;
    else if (ang <= 315.0f) return Directions::SOUTHWEST;
    else return Directions::SOUTH;
}

#pragma mark -
#pragma mark Rendering

/**
 * Animates the given direction.
 *
 * If the animation is not active, it will reset to the initial animation frame.
 *
 * @param direction    The reference to the animation direction
 * @param on        Whether the animation is active
 */
void AnimationSceneNode::animate(Directions direction, bool on){
    if(_direction != direction){
        _timeSinceLastAnim = 0;
        _frame = 0;
    }
    
    _direction = direction;
    _on = on;
    if(on){
        updateAnimTime();
        if (frameUpdateReady()){
            stepAnimation();
        }
    }
    else{
        _timeSinceLastAnim = 0;
        _frame = 0;
    }
}


/**
 * Draws this polygon node via the given SpriteBatch.
 *
 * This method only worries about drawing the current node.  It does not
 * attempt to render the children.
 *
 * @param batch     The SpriteBatch to draw with.
 * @param transform The global transformation matrix.
 * @param tint      The tint to blend with the Node color.
 */
void AnimationSceneNode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, const cugl::Affine2& transform, cugl::Color4 tint){
    if(_on){
        const std::shared_ptr<cugl::scene2::SpriteNode> dir = getAnimation(_direction);
        dir->setFrame(_frame);
        dir->draw(batch, transform, tint);
    }
}

