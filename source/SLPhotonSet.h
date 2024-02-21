//
//  PhotonSet.h
//
//  In the intro class we implemented the photons as a "particle system". That
//  is because memory was a tricky resource in Java. While we obviously need
//  to worry about memory in C++, we don't need to use anything as advanced as
//  free lists just yet.  Smart pointers will take care of us!
//
//  With that said, the design of this class is entirely up to you.  Follow
//  the instructions carefully.
//
//  Author:  YOUR NAME HERE
//  Version: THE DATE HERE
//
#ifndef __SL_PHOTON_SET_H__
#define __SL_PHOTON_SET_H__
#include <cugl/cugl.h>
#include <unordered_set>

class PhotonSet{
#pragma mark Photon
    
public:
    /**
     * An inner class that represents a single Photon.
     *
     */
    class Photon{
        
    public:
        cugl::Vec2 position;
        cugl::Vec2 velocity;
    private:
        float _scale;
        int _age;
        int _maxage;
    public:
        /**
         * Allocates an Photon by setting its position and velocity.
         *
         * @param p     The position
         * @param v     The velocity
         * @param mx   The max age
         */
        Photon(const cugl::Vec2 p, const cugl::Vec2 v, int mx);
        
        /**
         * Returns the scale of this photon.
         *
         * @return the scale of this photon.
         */
        float getScale() const { return _scale; }
        
        /**
         * Returns the age of this photon.
         *
         * @return the age of this photon.
         */
        int getAge() const { return _age; }
        
        /**
         * Moves the Photon one animation frame
         *
         * This movement code supports "wrap around".  If the photon
         * goes off one edge of the screen, then it appears across the
         * edge on the opposite side. However, this method performs no
         * collision detection. Collisions are resolved afterwards.
         */
        void update(cugl::Size size);
    };
    
public:
    /** The texture for the photon sprite sheet. */
    std::shared_ptr<cugl::Texture> _texture;
    /** The speed of a general photon */
    float _speed;
    /** The mass of a general photon */
    float _mass;
    /** The radius of a general photon */
    float _radius;
    /** The maxage of a general photon */
    int _maxage;
    
    
public:
    /** The collection of all ACTIVE photons. Allow the user direct access */
    std::unordered_set<std::shared_ptr<Photon>> current;
    /**
     * Creates an photon set with the default values.
     *
     * To properly initialize the photon set, you should call the init
     * method with the JSON value. We cannot do that in this constructor
     * because the JSON value does not exist at the time the constructor
     * is called (because we do not create this object dynamically).
     */
    PhotonSet();
    
    /**
     * Initializes photon data with the given JSON
     *
     * This JSON contains all shared information for photons
     *
     * If this method is called a second time, it will reset all
     * photon data.
     *
     * @param data  The data defining the asteroid settings
     *
     * @return true if initialization was successful
     */
    bool init(std::shared_ptr<cugl::JsonValue> data);
    
    /**
     * Returns true if the photon set is empty.
     *
     * This means that the current set is empty.
     *
     * @return true if the asteroid set is empty.
     */
    bool isEmpty() const { return current.empty(); }
    
    /**
     * Returns the default mass of a photon
     *
     * This value should be multiplied by the photon scale to get the
     * true mass.
     */
    float getMass() const { return _mass; }
    /**
     * Returns the default radius of a photon
     *
     * This value should be multiplied by the photon scale to get the
     * true radius.
     */
    float getRadius() const { return _radius; }
    
    /**
     * Moves all the photons in the active set.
     *
     * This movement code supports "wrap around".  If the photon goes off one
     * edge of the screen, then it appears across the edge on the opposite
     * side. However, this method performs no collision detection. Collisions
     * are resolved afterwards.
     */
    
    /**
     * Returns the image for a single photon; reused by all photon.s
     *
     * This value should be loaded by the GameScene and set there. However,
     * @return the image for a single asteroid; reused by all asteroids.
     */
    const std::shared_ptr<cugl::Texture>& getTexture() const {
        return _texture;
    }
    
    void setTexture(const std::shared_ptr<cugl::Texture>& value);
    
    /**
     * Moves all the photons in the active set.
     */
    void update(cugl::Size size);
    
    /**
     * Spawns a Photon and includes it into the photon set
     */
    void spawnPhoton(cugl::Vec2 p, cugl::Vec2 v);
    
    /**
     * Draws all active Photons to the sprite batch within the given bounds.
     *
     * This drawing code supports "wrap around". If a photon is partly off of
     * one edge, then it will also be drawn across the edge on the opposite
     * side.
     *
     * @param size      The size of the window (for wrap around)
     */
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);
};
#endif /* __SL_PHOTON_SET_H__ */
