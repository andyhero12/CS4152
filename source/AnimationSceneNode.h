//
//  AnimationScene.hpp
//  Heaven
//
//  Created by Henry Gao on 4/1/24.
//

#ifndef AnimationScene_h
#define AnimationScene_h

#include <stdio.h>
#include <cugl/cugl.h>



class AnimationSceneNode : public cugl::scene2::SpriteNode {
public:
#pragma mark -
#pragma mark Directions
    /**
     * Enumeration to identify the Animation directions
     * From top to bottom correponds to index 0 to 7
     */
    enum class Directions : int {
        EAST,
        SOUTHEAST,
        SOUTH,
        SOUTHWEST,
        WEST,
        NORTHWEST,
        NORTH,
        NORTHEAST
    };
#pragma mark -
#pragma mark Helper Functions
private:
    bool createSpriteNodes(const std::vector<std::shared_ptr<cugl::Texture>>& textures,int rows, int cols);
    
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
    
#pragma mark -
#pragma mark Class Attributes
protected:
    /** The active animation frame */
    int _frame;
    /** All the different sprite directions*/
    std::vector<std::shared_ptr<cugl::scene2::SpriteNode>> _animationSprites;
    /** current direction animation is facing*/
    Directions _direction;
    /** Whether draw will draw animation or not*/
    bool _on;
    // FOR SLOWING DOWN ANIMATIONS
    /** Number of frames that passed*/
    int _timeSinceLastAnim;
    /** Attack animation frequency; represents the number of frames until update */
    int _animFreq;
    float _height;
    float _width;
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates an empty scene graph node with the degenerate texture.
     *
     * This constructor should never be called directly, as this is an abstract
     * class.
     */
    AnimationSceneNode();
    
    /**
     * Deletes this node, releasing all resources.
     */
    ~AnimationSceneNode() { dispose(); }
    
    /**
     * Disposes all of the resources used by this node.
     *
     * A disposed Node can be safely reinitialized. Any children owned by this
     * node will be released.  They will be deleted if no other object owns them.
     *
     * It is unsafe to call this on a Node that is still currently inside of
     * a scene graph.
     */
    virtual void dispose() override;
    
    
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
    bool initWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& textures, int rows, int cols) {
        return initWithTextures(textures,rows,cols,rows*cols);
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
    bool initWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& textures, int rows, int cols, int size){
            return initWithTextures(textures,rows,cols,size, 10);
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
     * @param freqAnimation      The number of frames until an update
     *
     * @return  true if the filmstrip is initialized properly, false otherwise.
     */
    bool initWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& textures, int rows, int cols, int size, int freqAnimation);

    /**
      * Returns a new polygon node from a Texture object.
      *
      * After creation, the polygon will be a rectangle. The vertices of this
      * polygon will be the corners of the image. The rectangle will be
      * triangulated with the standard two triangles.
      *
      * @param texture   A shared pointer to a Texture object.
      *
      * @return a new polygon node from a Texture object.
      */
     static std::shared_ptr<AnimationSceneNode> allocWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& texture, int rows, int cols, int size, int freqAnimation) {
         std::shared_ptr<AnimationSceneNode> node = std::make_shared<AnimationSceneNode>();
         return (node->initWithTextures(texture,rows, cols, size, freqAnimation) ? node : nullptr);
     }
#pragma mark -
#pragma mark Attribute Accessors
    /**
     * Returns the number of frames in this filmstrip.
     *
     * @return the number of frames in this filmstrip.
     */
    int getSpan() const { return _size; }
    
    /**
     * Returns the current active frame.
     *
     * @return the current active frame.
     */
    unsigned int getFrame() const { return _frame; }
    
    /**
     * Returns the number of frames
     *
     * @return the  number of frames
     */
    unsigned int getSize() const { return _size; }
    
    /**
     * Sets the active frame as the given index.
     *
     * If the frame index is invalid, an error is raised.
     *
     * @param frame the index to make the active frame
     */
    void setFrame(int frame);

    /**
     * Returns the animation node for the given animation direction
     *
     * @param  direction   The enumeration to identify the afterburner
     *
     * @return the animation node for the given direction
     */
    const std::shared_ptr<cugl::scene2::SpriteNode>& getAnimation(Directions direction) const;
    /**
    * Sets the anchor point in percentages.
    *
    * The anchor point defines the relative origin of Node with respect to its
    * parent.  It is a "pin" where the Node is attached to its parent.  In
    * effect, the translation of a Node is defined by its position plus
    * anchor point.
    *
    * The anchorPoint is normalized, like a percentage. (0,0) means the
    * bottom-left corner and (1,1) means the top-right corner. There are many
    * anchor point constants defined in {@link Vec2}.  However, there is
    * nothing preventing an anchor point higher than (1,1) or lower than (0,0).
    *
    * The default anchorPoint is (0.5,0.5), so it starts in the center of
    * the node. Changing the anchor will not move the contents of the node in
    * the parent space, but it will change the value of the Node position.
    *
    * This function is disabled if the node is using absolute positioning.
    * That is because anchors do not make sense when we are drawing polygons
    * directly to the screen.
    *
    * @param anchor    The anchor point of node.
    */
    virtual void setAnchor(const cugl::Vec2 anchor) override;


    /**
     * Sets the untransformed size of the node.
     *
     * The content size remains the same no matter how the node is scaled or
     * rotated. All nodes must have a size, though it may be degenerate (0,0).
     *
     * By default, the content size of a TexturedNode is the size of the
     * bounding box of the defining polygon. Resizing a texture node will
     * stretch the image to fill in the new size.
     *
     * @param size  The untransformed size of the node.
     */
    virtual void setContentSize(const cugl::Size size) override;

    
    void setPosition(const cugl::Vec2 &position);
    void setAngle(float angle);

#pragma mark -
#pragma mark Rendering
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
    virtual void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, const cugl::Affine2& transform, cugl::Color4 tint) override;
    
    /**
     * Animates the given direction.
     *
     * If the animation is not active, it will reset to the initial animation frame.
     *
     * @param direction    The reference to the animation direction
     * @param on        Whether the animation is active
     */
    void animate(Directions direction, bool on);
    
    
    /** Convert radians to Direction */
    static Directions convertRadiansToDirections(double rad);

    /** Convert Angle to Direction */
    static Directions convertAngleToDirections(float ang);
    
    float getWidth(){ return _width;}
    float getHeight(){ return _height;}
};
    
#endif /* AnimationScene_h */
