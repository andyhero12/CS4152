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
//  Based on original GameX Dog Demo by Rama C. Hoetzlein, 2002
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
class Dog {
private:
    
    std::array<std::string,4> modes = {"SHOOT", "BUILD", "EXPLODE", "NOTHING"};
    
    /** Position of the ship */
    cugl::Vec2 _pos;
    /** Velocity of the ship */
    cugl::Vec2 _vel;
    
    int _mode = 0;
    
    int _absorbValue;
    // The following are protected, because they have no accessors
    /** Current angle of the ship */
    float _ang;
    /** Countdown to limit refire rate */
    int _refire;
    // heal timer
    int _healCooldown;
    /** The amount of health this ship has */
    int _health;
    int _maxHealth;
    int _prevTurn;
    

    // JSON DEFINED ATTRIBUTES
    /** Mass/weight of the ship. Used in collisions. */
    float _mass;
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
    
    int _modeCooldown;
    
    int _modeTimer;
    
    // Asset references. These should be set by GameScene
    /** Reference to the ships sprite sheet */
//    std::shared_ptr<cugl::SpriteSheet> _sprite;
    /** Radius of the ship in pixels (derived from sprite sheet) */
    float _radius;
    
    Animation runAnimation;
    Animation biteAnimation;
    bool attack;

public:
#pragma mark Constructors
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
    ~Dog() {}

    
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
    /**
     * Returns the position of this ship.
     *
     * This is location of the center pixel of the ship on the screen.
     *
     * @return the position of this ship
     */
    const cugl::Vec2& getPosition() const { return _pos; }
    
    /**
     * Sets the position of this ship.
     *
     * This is location of the center pixel of the ship on the screen.
     * Setting this value does NOT respect wrap around. It is possible
     * to use this method to place the ship off screen (so be careful).
     *
     * @param value the position of this ship
     */
    void setPosition(cugl::Vec2 value) { _pos = value; }
    
    /**
     * Sets the position of this ship, supporting wrap-around.
     *
     * This is the preferred way to "bump" a ship in a collision.
     *
     * @param value     The position of this ship
     * @param size      The size of the window (for wrap around)
     */
    void setPosition(cugl::Vec2 value, cugl::Vec2 size);
    
    /**
     * Returns the velocity of this ship.
     *
     * This value is necessary to control momementum in ship movement.
     *
     * @return the velocity of this ship
     */
    const cugl::Vec2& getVelocity() const { return _vel; }

    /**
     * Sets the velocity of this ship.
     *
     * This value is necessary to control momementum in ship movement.
     *
     * @param value the velocity of this ship
     */
    void setVelocity(cugl::Vec2 value) { _vel = value; }
    
    /**
     * Returns the angle that this ship is facing.
     *
     * The angle is specified in degrees. The angle is counter clockwise
     * from the line facing north.
     *
     * @return the angle of the ship
     */
    float getAngle() const { return _ang; }
    
    float getScale() const { return (1 + getAbsorb()/30.0f);}
    /**
     * Sets the angle that this ship is facing.
     *
     * The angle is specified in degrees. The angle is counter clockwise
     * from the line facing north.
     *
     * @param value the angle of the ship
     */
    void setAngle(float value) { _ang = value; }
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
    
    
    cugl::Poly2 getBlastRec();
    /**
     * Returns true if the ship can fire its weapon
     *
     * Weapon fire is subjected to a cooldown. You can modify the
     * value "fire rate" in the JSON file to make this faster or slower.
     *
     * @return true if the ship can fire
     */
    bool canFireWeapon() const{
        return !attack;
//        return _refire > _firerate;
    }
    
    bool canChangeMode() const {
        return (_modeCooldown < _modeTimer);
    }
    
    
    void setAttack();
    
    bool canHeal() const {
        return (_healCooldown > _healRate);
    }
    
    void resetHeal() {
        _healCooldown = 0;
    }
    void reloadMode(){
        _modeTimer = 0;
    }
    
    /**
     * Returns the mass of the ship.
     *
     * This value is necessary to resolve collisions. It is set by the
     * initial JSON file.
     *
     * @return the ship mass
     */
    float getMass() const {
        return _mass;
    }
    
    float getExplosionRadius() const{
        return _explosionRadius;
    }
    
    float getBiteRadius() const{
        return _biteRadius;
    }

    /**
     * Returns the radius of the ship.
     *
     * This value is necessary to resolve collisions. It is computed from
     * the sprite sheet.
     *
     * @return the ship radius
     */
    float getRadius() {
        return _radius;
    }
    
    std::string getMode(){
        return modes[_mode];
    }
    
    void toggleMode(){
        int length = sizeof(modes) / sizeof(modes[0]);
        _mode = (_mode + 1) % length;
    }
    
#pragma mark Graphics
    /**
     * Returns the sprite sheet for the ship
     *
     * The size and layout of the sprite sheet should already be specified
     * in the initializing JSON. Otherwise, the contents of the sprite sheet
     * will be ignored.     *
     * @return the sprite sheet for the ship
     */
    const std::shared_ptr<cugl::SpriteSheet>& getSprite() const {
        return runAnimation.getSprite();
    }

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
    void setRunTexture(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    void setBiteTexture(const std::vector<std::shared_ptr<cugl::Texture>>& texture);
    
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
    void move(float forward, float turn, cugl::Size size);
private:
    int direction(int dir);
};

#endif /* __SL_SHIP_H__ */
