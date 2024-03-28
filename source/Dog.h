//
//  Dog.h
//
//  This class tracks all of the state (position, velocity, rotation) of a
//  single ship. In order to obey the separation of the model-view-controller
//  pattern, controller specific code (such as reading the keyboard) is not
//  present in this class.
//
//  With that said, you will notice several elements that look like they are
//  part of the view, namely the texture and sound.  But this is the texture
//  and sound DATA.  The actual view are the sprite batch and audio engine,
//  respectively, that use this
//
//  Author: Walker White
//  Based on original GameX Ship Demo by Rama C. Hoetzlein, 2002
//  Version: 1/20/22
//
#ifndef __GL_SHIP_H__
#define __GL_SHIP_H__
#include <cugl/cugl.h>
#include "Animation.h"
#include <vector>
#include <array>
#include <string>
/** The number of frames until we can fire again */
#define RELOAD_RATE 3
#define MAX_ABSORB 30

/**
 * Model class representing an alien ship.
 */
class Dog : public cugl::physics2::BoxObstacle{
    
private:
    CU_DISALLOW_COPY_AND_ASSIGN(Dog);
    std::array<std::string,4> modes = {"SHOOT", "BUILD", "EXPLODE", "NOTHING"};
    int _mode = 0;
    int _absorbValue;
    int _refire;
    // heal timer
    int _healCooldown;
    /** The amount of health this ship has */
    int _health;
    int _maxHealth;
    int _prevTurn;
    

    // JSON DEFINED ATTRIBUTES
    /** The number of frames until we can fire again */
    int _firerate;
    /** The number of frames until we can fire again */
    int _healRate;
    /** The number of columns in the sprite sheet */
    int _framecols;
    /** The number of frames in the sprite sheet */
    int _framesize;
    /** The sprite sheet frame for being at rest */
    int _frameflat;
    /** The shadow offset in pixels */
    float _shadows;
    float _explosionRadius;
    float _biteRadius;
    float _shootRadius;
    
    int _modeCooldown;
    
    int _modeTimer;
    
    // Asset references. These should be set by GameScene
    /** Reference to the ships sprite sheet */
    
    Animation runAnimationSmall;
    Animation biteAnimationSmall;
    Animation shootAnimationSmall;
    Animation idleAnimationSmall;
    
    Animation runAnimationMedium;
    Animation biteAnimationMedium;
    Animation shootAnimationMedium;
    Animation idleAnimationMedium;
    
    Animation runAnimationLarge;
    Animation biteAnimationLarge;
    Animation shootAnimationLarge;
    Animation idleAnimationLarge;
    
    
    std::shared_ptr<Animation> runAnimation;
    std::shared_ptr<Animation> biteAnimation;
    std::shared_ptr<Animation> shootAnimation;
    std::shared_ptr<Animation> idleAnimation;
    bool bite;
    bool shoot;
    bool idle;
    
    
    void setTexture(const std::vector<std::shared_ptr<cugl::Texture>> &texture, Animation &animation, int speed);
    
    void dispose(){
        _geometry = nullptr;
    }
public:
    
#pragma mark -
#pragma mark Static Constructors
    
    /**
     * Creates a new Dog at the given position.
     *
     * The Dog is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param size  The size of the dude in world units
     * @param scale The drawing scale (world to screen)
     *
     * @return  A newly allocated DudeModel at the given position with the given scale
     */
    static std::shared_ptr<Dog> alloc(std::shared_ptr<cugl::JsonValue> data, const cugl::Vec2& pos, const cugl::Size& size, float scale) {
        std::shared_ptr<Dog> result = std::make_shared<Dog>();
        return (result->init(data, pos, size, scale) ? result : nullptr);
    }
#pragma mark Constructors
    /**
     * Initializes a new Dog at the given position.
     *
     * The dude is sized according to the given drawing scale.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param size  The size of the dude in world units
     * @param scale The drawing scale (world to screen)
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(std::shared_ptr<cugl::JsonValue> data, const cugl::Vec2& pos, const cugl::Size& size, float scale);

    
    /**
     * Creates a ship wiht the given position and data.
     *
     * The JsonValue should be a reference of all of the constants
     * that necessary to set the "hidden physical properties".
     *
     * @param pos   The ship position
     * @param data  The data defining the physics constants
     */
    Dog(const cugl::Vec2& pos, std::shared_ptr<cugl::JsonValue> data);
    
    /**
     * Disposes the ship, releasing all resources.
     */
    ~Dog() {
        dispose();
    }
    Dog(){
        
    }
    
#pragma mark Properties
    // Explodes if too big
    bool tooBig(){
        return _absorbValue >= 30;
    }
    void addAbsorb(int value);
    void subAbsorb(int value);
    const int getAbsorb() const{ return _absorbValue;}
    void setAbsorbValue(int x){
        _absorbValue = x;
        _absorbValue = fmin(_absorbValue, MAX_ABSORB);
    }
    
    float getScale() const { return (1 + getAbsorb()/30.0f);}
    /**
     * Returns the current ship health.
     * 
     * When the health of the ship is 0, it is "dead"
     *
     * @return the current ship health.
     */
    int getHealth() const { return _health; }
    int getMaxHealth() const { return _maxHealth; }
    /**
     * Sets the current ship health.
     * 
     * When the health of the ship is 0, it is "dead"
     *
     * @param value The current ship health.
     */
    void setHealth(int value);
    /**
     * Returns true if the ship can fire its weapon
     *
     * Weapon fire is subjected to a cooldown. You can modify the
     * value "fire rate" in the JSON file to make this faster or slower.
     *
     * @return true if the ship can fire
     */
    bool canFireWeapon() const{
        return _refire > _firerate;
    }
    
    void reloadWeapon() {
        _refire = 0;
    }
    bool canChangeMode() const {
        return (_modeCooldown < _modeTimer);
    }
    
    
    void setBite();
    void setShoot();
    void setIdle();
    
    bool canHeal() const {
        return (_healCooldown > _healRate);
    }
    
    void resetHeal() {
        _healCooldown = 0;
    }
    void reloadMode(){
        _modeTimer = 0;
    }
    
    float getExplosionRadius() const{
        return _explosionRadius;
    }
    
    float getBiteRadius() const{
        return _biteRadius;
    }
    float getShootRadius() const {
        return _shootRadius;
    }
    
    std::string getMode(){
        return modes.at(_mode);
    }
    
    void toggleMode(){
        int length = sizeof(modes) / sizeof(modes.at(0));
        _mode = (_mode + 1) % length;
    }
    
#pragma mark Graphics

    /**
     * Sets the texture for this ship.
     *
     * The texture should be formated as a sprite sheet, and the size and 
     * layout of the sprite sheet should already be specified in the 
     * initializing JSON. If so, this method will construct a sprite sheet
     * from this texture. Otherwise, the texture will be ignored.
     *
     * @param texture   The texture for the sprite sheet
     */
    void setRunTextureSmall(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    void setBiteTextureSmall(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    void setShootTextureSmall(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    void setIdleTextureSmall(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    
    
    void setRunTextureMedium(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    void setBiteTextureMedium(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    void setShootTextureMedium(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    void setIdleTextureMedium(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    
    void setRunTextureLarge(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    void setBiteTextureLarge(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    void setShootTextureLarge(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    void setIdleTextureLarge(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    

    /**
     * Draws this ship to the sprite batch within the given bounds.
     *
     * This drawing code supports "wrap around". If the ship is partly off of
     * one edge, then it will also be drawn across the edge on the opposite
     * side.
     *
     * @param batch     The sprite batch to draw to
     * @param size      The size of the window (for wrap around)
     */
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size);

#pragma mark Movement
    /**
     * Moves the ship by the specified amount.
     *
     * Forward is the amount to move forward, while turn is the angle to turn
     * the ship (used for the "banking" animation). Turning is dampened so that
     * the ship does not turn forever. However, velocity has inertia and must
     * be counter-acted (as with the classics Asteroids game).
     *
     * This movement code supports "wrap around".  If the ship goes off one
     * edge of the screen, then it appears across the edge on the opposite
     * side. However, this method performs no collision detection. Collisions
     * are resolved afterwards.
     *
     * @param forward   Amount to move forward
     * @param turn      Amount to turn the ship
     * @param size      The size of the window (for wrap around)
     */
    void move(float forward, float turn, cugl::Vec2 Vel, bool _UseController, bool _UseKeyboard, cugl::Size size);
private:
    int direction(int dir);
};

#endif /* __SL_SHIP_H__ */

