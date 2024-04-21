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

class TextureAnimationNode :  public cugl::scene2::SceneNode {
#pragma mark -
#pragma mark Helper Functions
private:
    bool createTextureNodes(const std::vector<std::shared_ptr<cugl::Texture>>& textures);
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
    GLfloat maxS;
    GLfloat maxT;
    GLfloat minS;
    GLfloat minT;
    /** The active animation frame; corresponds to index */
    int _frame;
    /** All the different sprite directions*/
    std::vector<std::shared_ptr<cugl::scene2::PolygonNode>> _animationTexture;
    /** Number of frames that passed*/
    int _timeSinceLastAnim;
    /** Attack animation frequency; represents the number of frames until update */
    int _animFreq;
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates an empty scene graph node with the degenerate texture.
     *
     * This constructor should never be called directly, as this is an abstract
     * class.
     */
    TextureAnimationNode();
    
    /**
     * Deletes this node, releasing all resources.
     */
    ~TextureAnimationNode() { dispose(); }
    
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
     * Initializes the animationTexture with top left and bottom right pixel coordinates
     */
    bool initWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& textures, int x1, int y1, int x2, int y2, int freqAnimation);
    
    /**
     * Initializes the animationTexture with portion of sprite
     */
    bool initWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& textures, GLfloat minS, GLfloat maxS, GLfloat minT, GLfloat maxT, int freqAnimation);
    
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
    static std::shared_ptr<TextureAnimationNode> allocWithTextures(const std::vector<std::shared_ptr<cugl::Texture>>& textures, GLfloat minS, GLfloat maxS, GLfloat minT, GLfloat maxT, int freqAnimation){
         std::shared_ptr<TextureAnimationNode> node = std::make_shared<TextureAnimationNode>();
         return (node->initWithTextures(textures, minS, maxS, minT, maxT, freqAnimation) ? node : nullptr);
     }
#pragma mark -
#pragma mark Attribute Accessors

    
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
    
};
#endif /* TextureAnimation_hpp */
