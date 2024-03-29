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

void switchAnimation(std::shared_ptr<Animation>& animationPointer, Animation& newAnimation) {
       animationPointer = std::shared_ptr<Animation>(&newAnimation, [](Animation*){});
}


bool Dog::init(std::shared_ptr<cugl::JsonValue> data, const cugl::Vec2& pos, const cugl::Size& size, float scale){
    if (BoxObstacle::init(pos,size)){
        setDensity(1.0);
        setFriction(0.0f);
        // Physics
        
        _refire = 0;
        _absorbValue = 0;
        _modeTimer = 0;
        _healCooldown = 0;
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
        return true;
    }
    
    return false;
}


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
    _refire = 0;
    _absorbValue = 0;
    _modeTimer = 0;
    _healCooldown = 0;
    
    // Physics
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
void Dog::setRunTextureMedium(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
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
        runAnimationMedium = Animation( anims, 10, _frameflat);
        Vec2 origin(runAnimationMedium.getSprite()->getFrameSize()/2);
        runAnimationMedium.setOrigin(origin);
    }
}


void Dog::setRunTextureSmall(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
    setTexture(texture, runAnimationSmall, 5);
}
void Dog::setBiteTextureSmall(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
    setTexture(texture, biteAnimationSmall, 5);
}

void Dog::setShootTextureSmall(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
    setTexture(texture, shootAnimationSmall, 5);
}

void Dog::setIdleTextureSmall(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
    setTexture(texture, idleAnimationSmall, 20);
}

void Dog::setRunTextureLarge(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
    setTexture(texture, runAnimationLarge, 5);
}
void Dog::setBiteTextureLarge(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
    setTexture(texture, biteAnimationLarge, 5);
}

void Dog::setShootTextureLarge(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
    setTexture(texture, shootAnimationLarge, 5);
}

void Dog::setIdleTextureLarge(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
    setTexture(texture, idleAnimationLarge, 20);
}

void Dog::setBiteTextureMedium(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
    setTexture(texture, biteAnimationMedium, 5);
}

void Dog::setShootTextureMedium(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
    setTexture(texture, shootAnimationMedium, 5);
}

void Dog::setIdleTextureMedium(const std::vector<std::shared_ptr<cugl::Texture>> & texture) {
    setTexture(texture, idleAnimationMedium, 20);
}


void Dog::setTexture(const std::vector<std::shared_ptr<cugl::Texture>> &texture, Animation &animation, int speed) {
        if (_framecols > 0) {
            int rows = _framesize / _framecols;
            if (_framesize % _framecols != 0) {
                rows++;
            }
            std::vector<std::shared_ptr<cugl::SpriteSheet>> anims;
            for(auto& text : texture) {
                auto _sprite = cugl::SpriteSheet::alloc(text, rows, _framecols, _framesize);
                anims.push_back(_sprite);
            }

            animation = Animation(anims, speed, _frameflat);
            cugl::Vec2 origin(animation.getSprite()->getFrameSize() / 2);
            animation.setOrigin(origin);
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
    
//    // switch animation based on form
//    if (_absorbValue < 10){
//        switchAnimation(biteAnimation, biteAnimationSmall);
//        switchAnimation(runAnimation, runAnimationSmall);
//        switchAnimation(shootAnimation, shootAnimationSmall);
//        switchAnimation(idleAnimation, idleAnimationSmall);
//    }else if (_absorbValue < 25){
        switchAnimation(biteAnimation, biteAnimationMedium);
        switchAnimation(runAnimation, runAnimationMedium);
        switchAnimation(shootAnimation, shootAnimationMedium);
        switchAnimation(idleAnimation, idleAnimationMedium);
//    }
//    else{
//        switchAnimation(biteAnimation, biteAnimationLarge);
//        switchAnimation(runAnimation, runAnimationLarge);
//        switchAnimation(shootAnimation, shootAnimationLarge);
//        switchAnimation(idleAnimation, idleAnimationLarge);
//    }
//    if(bite){
//        biteAnimation->updateDirection();
//        if (biteAnimation->getFrame() == biteAnimation->getSprite()->getSize() -1){
//            bite = false;
//        }
//    }
//    else if(shoot){
//        shootAnimation->updateDirection();
//        if (shootAnimation->getFrame() == shootAnimation->getSprite()->getSize() -1){
//            shoot = false;
//        }
//    }
//    else if(idle && idleAnimation){
//        idleAnimation->updateDirection();
//    }
    // Don't draw if sprite not set
    if (runAnimation->getSprite() && biteAnimation->getSprite()) {
        // Transform to place the ship
        Affine2 shiptrans;
        // super duper magic number
        shiptrans.scale(getScale() * 0.75f);
        // account for sprite size so we are by logic coordinates
        shiptrans.scale(1 / runAnimation.getSprite()->getFrameSize().height);
//        shiptrans.rotate(_ang*M_PI/180);
        shiptrans.translate(getPosition());
        // Transform to place the shadow, and its color
        Affine2 shadtrans = shiptrans;
        shadtrans.translate(_shadows,-_shadows);
        Color4f shadow(0,0,0,0.5f);
        
        
//        if (bite){
//            biteAnimation->getSprite()->draw(batch,shadow,shadtrans);
//            biteAnimation->getSprite()->draw(batch,shiptrans);
//        }
//        else if(shoot){
//            shootAnimation->getSprite()->draw(batch,shadow,shadtrans);
//            shootAnimation->getSprite()->draw(batch,shiptrans);
//        }
//        else if (idle){
//            idleAnimation->getSprite()->draw(batch,shadow,shadtrans);
//            idleAnimation->getSprite()->draw(batch,shiptrans);
//        }
//        else{
            runAnimation->getSprite()->draw(batch,shadow,shadtrans);
            runAnimation->getSprite()->draw(batch,shiptrans);
//        }

    }
}

#pragma mark Movement


void Dog::setBite(){
    bite = true;
    biteAnimation->resetAnimation(_prevTurn);
}

void Dog::setIdle(){
    idle = true;
    if(idleAnimation){
        idleAnimation->update(_prevTurn);
    }
}

void Dog::setShoot(){
    shoot = true;
    shootAnimation->resetAnimation(_prevTurn);
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
    cugl::Vec2 _vel;
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
    }
    float _ang = getAngle();
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
    setPosition(getPosition() + (_vel*0.06));
//    while (getPosition().x > size.width) {
//        _pos.x = size.width;
//    }
//    while (_pos.x < 0) {
//        _pos.x = 0;
//    }
//    while (_pos.y > size.height) {
//        _pos.y = size.height;
//    }
//    while (_pos.y < 0) {
//        _pos.y = 0;
//    }
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
    
//    if(_vel.x == 0 && _vel.y == 0){
//        if(!bite && !shoot){
//            setIdle();
//        }
//        return;
//    }
//    
//    idle = false;
    
    if(runAnimation && runAnimation->getSprite()){
        runAnimation->update(_vel.getAngle() + 45);
        _prevTurn = runAnimation->currentAnimationDirection;
    }
}
