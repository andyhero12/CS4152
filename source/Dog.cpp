//
//  Dog.cpp
//
//  This class tracks all of the state (position, velocity, rotation) of a
//  single ship. In order to obey the separation of the model-view-controller
//  pattern, controller specific code (such as reading the keyboard) is not
//  present in this class.
//
//  With that said, you will notice several elements that look like they are
//  part of the view, namely the texture and sound.  But this is the texture
//  and sound DATA.  The actual view are the sprite batch and audio engine,
//  respectively, that use this data to provide feedback on the screen.
//
//  Author: Walker White
//  Based on original GameX Ship Demo by Rama C. Hoetzlein, 2002
//  Version: 1/20/22
//
#include "Dog.h"
#include <cmath>

using namespace cugl;

#define DRAW_WIREFRAME true

/**
 * Creates a ship wiht the given position and data.
 *
 * The JsonValue should be a reference of all of the constants
 * that necessary to set the "hidden physical properties".
 *
 * @param pos   The ship position
 * @param data  The data defining the physics constants
 */
Dog::Dog(const cugl::Vec2& pos, std::shared_ptr<cugl::JsonValue> data) {
    _ang  = 0;
    _refire = 0;
    _radius = 0;
    _absorbValue = 0;
    _modeTimer = 0;
    _healCooldown = 0;
    
    // Physics
    _mass = data->getFloat("mass",1.0);
    _firerate = data->getInt("fire rate",0);
    _healRate = data->getInt("heal rate",0);
    _shadows  = data->getFloat("shadow",0.0);
    _explosionRadius = data->getFloat("explosionRadius",100.0);
    _biteRadius = data->getFloat("biteRadius",150.0);
    _shootRadius = data->getFloat("shootRadius", 500.0);
    // Sprite sheet information
    _framecols = data->getInt("sprite cols",0);
    _framesize = data->getInt("sprite size",0);
    _frameflat = data->getInt("sprite frame",0);
    
    _health = data->getInt("health",0);
    _maxHealth = _health;
    _modeCooldown = data->getInt("mode cooldown",0);
    
    
    
}

/**
 * Sets the current ship health.
 *
 * When the health of the ship is 0, it is "dead"
 *
 * @param value The current ship health.
 */
void Dog::setHealth(int value) {
    if (value >= 0) {
        // Do not allow health to go negative
        _health = value;
    } else {
        _health = 0;
    }
}

void Dog::addAbsorb(int value) {
    _absorbValue += value;
    _absorbValue = fmin(_absorbValue, MAX_ABSORB);
}

void Dog::subAbsorb(int value) {
    _absorbValue -= value;
    if (_absorbValue < 0){
        _absorbValue = 0;
    }
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
void Dog::setRunTexture(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
    if (_framecols > 0) {
        int rows = _framesize/_framecols;
        if (_framesize % _framecols != 0) {
            rows++;
        }
        std::shared_ptr<cugl::SpriteSheet> _sprite ;
        std::vector<std::shared_ptr<cugl::SpriteSheet>> anims;
        for(auto& text : texture) {
            _sprite = SpriteSheet::alloc(text, rows, _framecols, _framesize);
            anims.push_back(_sprite);
        }
        runAnimation = Animation( anims, 10, _frameflat);
        Vec2 origin(runAnimation.getSprite()->getFrameSize()/2);
        runAnimation.setOrigin(origin);
        _radius = std::max(runAnimation.getSprite()->getFrameSize().width, _sprite->getFrameSize().height)/2;
    }
}


void Dog::setBiteTexture(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
//    std::cout << texture << std::endl;
    if (_framecols > 0) {
        int rows = _framesize/_framecols;
        if (_framesize % _framecols != 0) {
            rows++;
        }
        std::shared_ptr<cugl::SpriteSheet> _sprite ;
        std::vector<std::shared_ptr<cugl::SpriteSheet>> anims;
        for(auto& text : texture) {
            _sprite = SpriteSheet::alloc(text, rows, _framecols, _framesize);
            anims.push_back(_sprite);
        }
        
        biteAnimation = Animation(anims, 5, _frameflat);
        
        
        Vec2 origin(biteAnimation.getSprite()->getFrameSize()/2);
        biteAnimation.setOrigin(origin);
//        _radius = std::max(biteAnimation.getSprite()->getFrameSize().width, _sprite->getFrameSize().height)/2;
    }
}


/**
 * Draws this ship on the screen within the given bounds.
 *
 * This drawing code supports "wrap around". If the ship is partly off of
 * one edge, then it will also be drawn across the edge on the opposite
 * side.
 *
 * @param batch     The sprite batch to draw to
 * @param size      The size of the window (for wrap around)
 */
void Dog::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, Size bounds) {
    if(attack){
        biteAnimation.updateDirection();
        if (biteAnimation.getFrame() == biteAnimation.getSprite()->getSize() -1){
            attack = false;
        }
    }
    // Don't draw if sprite not set
    if (runAnimation.getSprite() && biteAnimation.getSprite()) {
        // Transform to place the ship
        Affine2 shiptrans;
        // super duper magic number
        shiptrans.scale(getScale() * 0.75f);
        // account for sprite size so we are by logic coordinates
        shiptrans.scale(1 / runAnimation.getSprite()->getFrameSize().height);
//        shiptrans.rotate(_ang*M_PI/180);
        shiptrans.translate(_boxObstacle->getPosition());
        // Transform to place the shadow, and its color
        Affine2 shadtrans = shiptrans;
        shadtrans.translate(_shadows,-_shadows);
        Color4f shadow(0,0,0,0.5f);

        
        
        if (!attack){
            runAnimation.getSprite()->draw(batch,shadow,shadtrans);
            runAnimation.getSprite()->draw(batch,shiptrans);
        }
        else{
            biteAnimation.getSprite()->draw(batch,shadow,shadtrans);
            biteAnimation.getSprite()->draw(batch,shiptrans);
        }
        if (DRAW_WIREFRAME) {
            Color4 wireframeColor = Color4("green");
            batch->setColor(wireframeColor);
            batch->setTexture(nullptr);
            batch->outline(Rect(getObstacle()->getPosition(), getObstacle()->getDimension()));
        }
    }
}

#pragma mark Movement
/**
 * Sets the position of this ship, supporting wrap-around.
 *
 * This is the preferred way to "bump" a ship in a collision.
 *
 * @param value     The position of this ship
 * @param size      The size of the window (for wrap around)
 */
void Dog::setPosition(cugl::Vec2 value, cugl::Vec2 size) {
    _boxObstacle->setPosition(value);
}


void Dog::setAttack(){
    attack = true;
    biteAnimation.resetAnimation(_prevTurn);
}

/**
 * Moves the ship by the specified amount.
 *
 * Forward is the amount to move forward, while turn is the angle to turn the ship
 * (used for the "banking" animation. This method performs no collision detection.
 * Collisions are resolved afterwards.
 *
 * @param forward    Amount to move forward
 * @param turn        Amount to turn the ship
 */
void Dog::move(float forward, float turn, Vec2 Vel, bool _UseJoystick, bool _Usekeyboard, Size size) {

    //    if (attack){
    //        return;
    //    }
    if (forward == 0.0f) {
        _vel = Vec2(0, 0);
    }
    // Use the Keyboard input
    if (_Usekeyboard) {
        _vel = Vec2(turn, forward);
    }
    // Use the Joystick input
    if (_UseJoystick) {
        _vel = Vel;
        //_vel = Vec2(turn, forward);
    }
    
    
    if (!(forward == 0 && turn == 0)) {
        _ang = atan2(forward, turn) * (180 / M_PI) - 90;
        setAngle(_ang);
    }

    if (abs(Vel.x) > 0.2 || abs(Vel.y) > 0.2) {
        _ang = atan2(Vel.y, Vel.x) * (180 / M_PI) - 90;
        setAngle(_ang);
    }

    
    // INVARIANT: 0 <= ang < 360
    if (_ang > 360)
        _ang -= 360;
    if (_ang < 0)
        _ang += 360;
    _vel = _vel.normalize();
    
    // Move the ship position by the ship velocity
    Vec2 pos = _boxObstacle->getPosition();
    pos += _vel * 0.06;
    while (pos.x > size.width) {
        pos.x = size.width;
    }
    while (pos.x < 0) {
        pos.x = 0;
    }
    while (pos.y > size.height) {
        pos.y = size.height;
    }
    while (pos.y < 0) {
        pos.y = 0;
    }
    setPosition(pos);

    //std::cout << _boxObstacle->get();
    //std::cout << "\n";

    //Increment the refire readiness counter
    if (_refire <= _firerate) {
        _refire++;
    }
    if (_healCooldown <= _healRate) {
        _healCooldown++;
    }
    
    if (_modeTimer <= _modeCooldown){
        _modeTimer++;
    }
    
    runAnimation.update(_vel.getAngle() - 90);
    _prevTurn = runAnimation.currentAnimationDirection;
    // #@!$
    getObstacle()->markDirty(true);
}


Poly2 Dog::getBlastRec(){
    Vec2 center = getPosition();
    float longSide = 400;
    float shortSide = 50;
    Rect org;
    if (_ang < 85){ // up
        org.set(center.x-20, center.y-20, shortSide, longSide);
    }else if (_ang < 175){ // left
        org.set(center.x - longSide, center.y-20, longSide, shortSide);
    }else if (_ang < 265){
        org.set(center.x-20, center.y- longSide-20, shortSide, longSide);
    }else{
        org.set(center.x, center.y-20, longSide, shortSide);
    }
    Poly2 resultingRect(org);
    return resultingRect;
}
