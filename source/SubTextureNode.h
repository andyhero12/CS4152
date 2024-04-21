//
//  SubTextureNode.hpp
//  Heaven
//
//  Created by Henry Gao on 4/3/24.
//

#ifndef SubTextureNode_h
#define SubTextureNode_h

#include <stdio.h>
#include <cugl/cugl.h>

class SubTextureNode : public cugl::scene2::PolygonNode {
#pragma mark Values
protected:
    /** full texture */
    std::shared_ptr<cugl::Texture> original;
public:
    /**
     * Initializes a polygon node from a Texture object.
     *
     * After creation, the polygon will be a rectangle. The vertices of this
     * polygon will be the corners of the image. The rectangle will be
     * triangulated with the standard two triangles.
     *
     * @param texture   A shared pointer to a Texture object.
     *
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithTexture(const std::shared_ptr<cugl::Texture>& texture) override;
    
    /**
     * Initializes a polygon node from a Texture object and the given rect.
     *
     * The vertices of the rectangle will define the portion of the texture shown
     * (in image space). The rectangle will be triangulated with the standard two
     * triangles.
     *
     * @param texture   A shared pointer to a Texture object.
     * @param rect      The rectangle to texture
     *
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    bool initWithTexturePoly(const std::shared_ptr<cugl::Texture>& texture, const cugl::Rect rect) {
        setPolygon(rect);
        return TexturedNode::initWithTexture(texture);
    }
    
#pragma mark -
#pragma mark Static Constructors

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
    static std::shared_ptr<SubTextureNode> allocWithTexture(const std::shared_ptr<cugl::Texture>& texture) {
        std::shared_ptr<SubTextureNode> node = std::make_shared<SubTextureNode>();
        return (node->initWithTexture(texture) ? node : nullptr);
    }
    /**
     * Returns a new polygon node from a Texture object and the given rect.
     *
     * The vertices of the rectangle will define the portion of the texture shown
     * (in image space). The rectangle will be triangulated with the standard two
     * triangles.
     *
     * @param texture   A shared pointer to a Texture object.
     * @param rect      The rectangle to texture
     *
     * @return a new polygon node from a Texture object and the given rect.
     */
    static std::shared_ptr<SubTextureNode> allocWithTexture(const std::shared_ptr<cugl::Texture>& texture,
                                                         const cugl::Rect rect)  {
        std::shared_ptr<SubTextureNode> node = std::make_shared<SubTextureNode>();
        return (node->initWithTexturePoly(texture,rect) ? node : nullptr);
    }
#pragma mark -
#pragma mark Subtexture
    void setSubtexture(GLfloat minS, GLfloat maxS, GLfloat minT, GLfloat maxT);
};

#endif /* SubTextureNode_hpp */
