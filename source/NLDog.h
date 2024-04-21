//
//  Dog.h
//  Rocket Demo
//
//  This encapsulates all of the information for the rocket demo.  Note how this
//  class combines physics and animation.  This is a good template for models in
//  your game.
//
//  WARNING: There are a lot of shortcuts in this design that will do not adapt
//  well to data driven design.  This demo has a lot of simplifications to make
//  it a bit easier to see how everything fits together.  However, the model
//  classes and how they are initialized will need to be changed if you add
//  dynamic level loading.
//
//  Pay close attention to how this class designed. This class uses our standard
//  shared-pointer architecture which is common to the entire engine.
//
//  1. The constructor does not perform any initialization; it just sets all
//     attributes to their defaults.
//
//  2. All initialization takes place via init methods, which can fail if an
//     object is initialized more than once.
//
//  3. All allocation takes place via static constructors which return a shared
//     pointer.
//
//  Note that this object manages its own texture, but DOES NOT manage its own
//  scene graph node.  This is a very common way that we will approach complex
//  objects.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White
//  Version: 1/10/17
//
#ifndef __NL_ROCKET_MODEL_H__
#define __NL_ROCKET_MODEL_H__
#include <cugl/cugl.h>
#include "NLInput.h"
#include "AnimationSceneNode.h"
#include "UIController.h"
#include <array>

/** The thrust factor to convert player input into thrust */
#define DEFAULT_THRUST 2.0f

#define DOG_SIZE Vec2(1.5, 1.5)
/**
 * This class is the player avatar for the rocket lander game.
 *
 * There are two ways to link a physics object to a scene graph node on the
 * screen.  One way is with callback functions, like we did with the crates.
 * The other way is to create a subclass of a physics object and have it track
 * its own scene graph node.  This class is an example of the latter.
 */
class Dog : public cugl::physics2::BoxObstacle {
public:
    enum class Actions : int {
        IDLE,
        RUN,
        SHOOT,
        BITE,
        RECALL,
        DASH
    };
    enum class DogSize : int {
        SMALL,
        MEDIUM,
        LARGE
    };
private:
    /** This macro disables the copy constructor (not allowed on scene graphs) */
    CU_DISALLOW_COPY_AND_ASSIGN(Dog);
    
protected:
    Actions action;
    DogSize dogSize;
    
    AnimationSceneNode::Directions prevDirection;
    cugl::Vec2 dir;
    cugl::Vec2 _ang;
    std::shared_ptr<cugl::scene2::OrderedNode> baseBlankNode;
    std::shared_ptr<cugl::scene2::SceneNode> frontEffectsNode;
    std::shared_ptr<cugl::scene2::SceneNode> backEffectsNode;
    
    std::shared_ptr<AnimationSceneNode> idleAnimation;
    std::shared_ptr<AnimationSceneNode> runAnimation;
    std::shared_ptr<AnimationSceneNode> biteAnimation;
    std::shared_ptr<AnimationSceneNode> shootAnimation;
    std::shared_ptr<AnimationSceneNode> dashAnimation;
    
    std::shared_ptr<AnimationSceneNode> idleAnimationSmall;
    std::shared_ptr<AnimationSceneNode> runAnimationSmall;
    std::shared_ptr<AnimationSceneNode> biteAnimationSmall;
    std::shared_ptr<AnimationSceneNode> shootAnimationSmall;
    std::shared_ptr<AnimationSceneNode> dashAnimationSmall;
    
    std::shared_ptr<AnimationSceneNode> idleAnimationMedium;
    std::shared_ptr<AnimationSceneNode> runAnimationMedium;
    std::shared_ptr<AnimationSceneNode> biteAnimationMedium;
    std::shared_ptr<AnimationSceneNode> shootAnimationMedium;
    std::shared_ptr<AnimationSceneNode> dashAnimationMedium;
    
    std::shared_ptr<AnimationSceneNode> idleAnimationLarge;
    std::shared_ptr<AnimationSceneNode> runAnimationLarge;
    std::shared_ptr<AnimationSceneNode> biteAnimationLarge;
    std::shared_ptr<AnimationSceneNode> shootAnimationLarge;
    std::shared_ptr<AnimationSceneNode> dashAnimationLarge;
    
    std::shared_ptr<AnimationSceneNode> recallAnimation;
    std::shared_ptr<AnimationSceneNode> belowPenta;
    std::shared_ptr<UIController> _uiController;

    std::array<std::string,4> modes = {"SHOOT", "BAIT", "BOMB", "RECALL"};
    bool _startDash;
    bool _startBite;
    bool _startShoot;
    bool _startRecall;
    bool _sendSize;
    int _dashCounter;
    int _dashRate;
    int _mode;
    int _refire;
    int _absorbValue;
    int _maxAbsorb;
    int _firerate;
    int _healCooldown;
    int _health;
    int _maxHealth;
    int _healRate;
    float _explosionRadius;
    float _biteRadius;
    float _shootRadius;
    AnimationSceneNode::Directions _curDirection;
public:
#pragma mark Constructors
    /**
     * Creates a new rocket at the origin.
     *
     * NEVER USE A CONSTRUCTOR WITH NEW. If you want to allocate a model on
     * the heap, use one of the static constructors instead.
     */
    Dog(void) : BoxObstacle(), action(Actions::IDLE), dir(cugl::Vec2(0,0)), prevDirection(AnimationSceneNode::Directions::SOUTH){ }
    
    /**
     * Destroys this rocket, releasing all resources.
     */
    virtual ~Dog(void) { dispose(); }
    
    /**
     * Disposes all resources and assets of this rocket
     *
     * Any assets owned by this object will be immediately released.  Once
     * disposed, a rocket may not be used until it is initialized again.
     */
    void dispose();
    
    /**
     * Initializes a new rocket at the origin.
     *
     * The rocket is 1 unit by 1 unit in size. The rocket is scaled so that
     * 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init() override { return init(cugl::Vec2::ZERO,cugl::Size::ZERO); }
    
    /**
     * Initializes a new rocket with the given position
     *
     * The rocket is 1 unit by 1 unit in size. The rocket is scaled so that
     * 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const cugl::Vec2 pos) override { return init(pos,cugl::Size(1,1)); }
    
    /**
     * Initializes a new rocket with the given position and size.
     *
     * The rocket size is specified in world coordinates.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  size       The dimensions of the box.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const cugl::Vec2 pos, const cugl::Size size) override;
    
    
    void setRecallAnimation(std::shared_ptr<AnimationSceneNode> recall);
    void setBelowPenta(std::shared_ptr<AnimationSceneNode> belowPenta);
    void setSmallAnimation(std::shared_ptr<AnimationSceneNode> idle, std::shared_ptr<AnimationSceneNode> run, std::shared_ptr<AnimationSceneNode> bite, std::shared_ptr<AnimationSceneNode> shoot,std::shared_ptr<AnimationSceneNode> dash);
    void setMediumAnimation(std::shared_ptr<AnimationSceneNode> idle, std::shared_ptr<AnimationSceneNode> run, std::shared_ptr<AnimationSceneNode> bite, std::shared_ptr<AnimationSceneNode> shoot,std::shared_ptr<AnimationSceneNode> dash);
    void setLargeAnimation(std::shared_ptr<AnimationSceneNode> idle, std::shared_ptr<AnimationSceneNode> run, std::shared_ptr<AnimationSceneNode> bite, std::shared_ptr<AnimationSceneNode> shoot,std::shared_ptr<AnimationSceneNode> dash);
    
    void setUIController(std::shared_ptr<UIController> controller){ _uiController = controller; }
    
    void setFinalDog(std::shared_ptr<cugl::scene2::OrderedNode> baseNode);
    void resetCurrentAnimations(DogSize dogSize);
    void updateLocalAnimations(DogSize dogSize);
    
    void updateDogSize(int absorbValue);
    void addEffects(std::shared_ptr<cugl::scene2::SceneNode> fnode, std::shared_ptr<cugl::scene2::SceneNode> bnode);
#pragma mark Static Constructors
    /**
     * Returns a newly allocate rocket at the origin.
     *
     * The rocket is 1 unit by 1 unit in size. The rocket is scaled so that
     * 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return a newly allocate rocket at the origin.
     */
    static std::shared_ptr<Dog> alloc() {
        std::shared_ptr<Dog> result = std::make_shared<Dog>();
        return (result->init() ? result : nullptr);
    }
    
    /**
     * Returns a newly allocated rocket with the given position
     *
     * The rocket is 1 unit by 1 unit in size. The rocket is scaled so that
     * 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     *
     * @return a newly allocated rocket with the given position
     */
    static std::shared_ptr<Dog> alloc(const cugl::Vec2& pos) {
        std::shared_ptr<Dog> result = std::make_shared<Dog>();
        return (result->init(pos) ? result : nullptr);
    }
    
    /**
     * Returns a newly allocated rocket with the given position and size
     *
     * The rocket size is specified in world coordinates.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param pos   Initial position in world coordinates
     * @param size  The dimensions of the box.
     *
     * @return a newly allocated rocket with the given position
     */
    static std::shared_ptr<Dog> alloc(const cugl::Vec2& pos, const cugl::Size& size) {
        std::shared_ptr<Dog> result = std::make_shared<Dog>();
        return (result->init(pos,size) ? result : nullptr);
    }
    
#pragma mark -
#pragma mark Accessors
    
    void startDash(){
        _startDash = true;
    }
    void resetSendSize(){
        _sendSize = false;
    }
    bool shouldSendSize(){
        return _sendSize;
    }
    void startBite(){
        _startBite = true;
    }
    void startRecall(){
        _startRecall = true;
    }
    void startShoot(){
        _startShoot = true;
    }
    std::shared_ptr<scene2::SceneNode> getDogNode(){
        return baseBlankNode;
    }
    std::shared_ptr<scene2::SceneNode> getUINode(){
        return _uiController->getUINode();
    }
    void addAbsorb(int value);
    void subAbsorb(int value);
    const int getAbsorb() const{ 
        return _absorbValue;
    }
    /**
     * Returns the amount of thrust that this rocket has.
     *
     * Multiply this value times the horizontal and vertical values in the
     * input controller to get the force.
     *
     * @return the amount of thrust that this rocket has.
     */
    float getThrust() const { return DEFAULT_THRUST; }
    /**
     * Returns the current ship health.
     *
     * When the health of the ship is 0, it is "dead"
     *
     * @return the current ship health.
     */
    int getHealth() const { return _health; }
    
    int getMaxHealth() const { return _maxHealth; }
    
    bool readyToRecall() const { return action == Actions::RECALL && recallAnimation->getFrame() == recallAnimation->getSize()/2; }
    int getMaxAbsorb() const { return _maxAbsorb; }
    /**
     * Sets the current ship health.
     *
     * When the health of the ship is 0, it is "dead"
     *
     * @param value The current ship health.
     */
    void setHealth(int value);
    
    void setAbsorbValue(int value);
    
    void setMode(int mode);
    
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
    
    
    bool canDash() const{
        return _dashCounter > _dashRate;
    }
    void resetDash() {
        _dashCounter = 0;
    }
    void reloadWeapon() {
        _refire = 0;
    }
    
    
    bool canHeal() const {
        return (_healCooldown > _healRate);
    }

    void resetHeal() {
        _healCooldown = 0;
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
    
    AnimationSceneNode::Directions getDirection() const{
        return _curDirection;
    }
    
    cugl::Vec2 getDir() const{
        return dir;
    }
    
    float getDirInDegrees() const {
        // normalize to our coordinates
        float res =_ang.getAngle() * (180 / M_PI);
        res -= 90.0f;
        if (res < 0){
            res += 360.0f;
        }
        
        return res;
    }
    
    Vec2 getBiteCenter() const{
        return getPosition();
    }
    Vec2 getShootCenter() const{
        return _ang*0.3 +  getPosition();
    }
    std::string getMode(){
        return modes.at(_mode);
    }
    
    void toggleMode(){
        int length = (int) modes.size();
        _mode = (_mode + 1) % length;
        _uiController->setToggle(getMode());
    }
    void setDogSize(DogSize size);

    
#pragma mark -
#pragma mark Animation
    
    
    void dogActions();
    
    
#pragma mark -
#pragma mark Physics
    void updateClientAnimations();
    void updateUI();
    /**
     * Applies the Velocity Movement for the Physics Engine
     *
     */
    void moveOnInputSetAction(InputController& _input);
    /**
     * Updates the object's physics state (NOT GAME LOGIC).
     *
     * This method is called AFTER the collision resolution state. Therefore, it
     * should not be used to process actions or any other gameplay information.
     * Its primary purpose is to adjust changes to the fixture, which have to
     * take place after collision.
     *
     * In other words, this is the method that updates the scene graph.  If you
     * forget to call it, it will not draw your changes.
     *
     * @param delta Timing values from parent loop
     */
    virtual void update(float delta) override;
};

#endif /* __RD_ROCKET_MODEL_H__ */
