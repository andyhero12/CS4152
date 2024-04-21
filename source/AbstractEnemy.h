//
//  AbstractEnemy.h
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//

#ifndef AbstractEnemy_h
#define AbstractEnemy_h
#include <cugl/cugl.h>
#include "OverWorld.h"
#include "AnimationSceneNode.h"
#define MAGIC_NUMBER_ENEMY_ANIMATION_FREQUENECY 4
// Default physics values
/** The density of this rocket */
#define DEFAULT_DENSITY 5.0f
/** The friction of this rocket */
#define DEFAULT_FRICTION 0.0f
/** The restitution of this rocket */
#define DEFAULT_RESTITUTION 0.0f

/** The number of frames before a new goal can be set */
#define PATHFIND_COOLDOWN 20


#define CLOSE_DISTANCE 2
#define STRAY_DISTANCE 3

#include "stlastar.h"
#include "WorldSearchVertex.h"

class AbstractEnemy : public cugl::physics2::BoxObstacle {
    
public:
    
    enum class EnemyActions : int {
        RUN,
        ATTACK
    };
    
    AbstractEnemy(){}
    // Virtual destructor
    virtual ~AbstractEnemy() {}
    
    bool init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex){
        bool result = physics2::BoxObstacle::init(m_pos,m_size);
        
        if (result){
            _counter = 0;
            updateRate = 15;
            clearSharingDirtyBits();
            setShared(true);
            setDensity(DEFAULT_DENSITY);
            setFriction(DEFAULT_FRICTION);
            setRestitution(DEFAULT_RESTITUTION);
            setFixedRotation(true);
            
            curAction = EnemyActions::RUN;
            _health = m_health;
            _maxHealth = m_health;
            targetIndex = m_targetIndex;
            _prevDirection =AnimationSceneNode::Directions::EAST;
            _curDirection = AnimationSceneNode::Directions::EAST;
            _pathfinder = std::make_shared<AStarSearch<WorldSearchVertex>>();
            
            return true;
        }
        return false;
    }
    
    void update(float delta) override{
        Obstacle::update(delta);
        _prevDirection =_curDirection;
        Vec2 direction = getLinearVelocity();
        _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());

        runAnimations->animate(_curDirection, curAction == EnemyActions::RUN);
        attackAnimations->animate(_curDirection, curAction == EnemyActions::ATTACK);
    }
    
    virtual void preUpdate(float dt, OverWorld& overWorld) = 0;
    
    virtual int getDamage() = 0;
    virtual bool canAttack() const = 0;
    virtual void resetAttack() = 0;
    virtual int getAbsorbValue() const = 0;
    virtual void executeDeath(OverWorld& overWorld) {
        CULog("executing Death\n");
    }
    
    int getTargetIndex() const{
        return targetIndex;
    }
    void setTargetIndex(int index){
        targetIndex = index;
    }
    int getHealth() const {
        return _health;
    }
    void setHealth(int m_health) {
        _health = m_health;
        _healthBar->setProgress((float)_health/_maxHealth);
    }
    
    
    
    cugl::Vec2 getTargetPositionFromIndex(OverWorld& overWorld){
        const std::shared_ptr<Dog>& curDog = overWorld.getDog();
        const std::shared_ptr<BaseSet> baseSet = overWorld.getBaseSet();
        const std::shared_ptr<DecoySet> decoySet = overWorld.getDecoys();
        cugl::Vec2 target_pos;
        int _targetIndex  = getTargetIndex();
        if (_targetIndex == 0){
            target_pos = curDog->getPosition();
        }else if (_targetIndex <= baseSet->_bases.size()){
            target_pos = baseSet->_bases.at(_targetIndex-1)->getPos();
        }else{
            target_pos = decoySet->getCurrentDecoys().at(_targetIndex-1-baseSet->_bases.size())->getPos();
        }
        return target_pos;
    }
    
    void setHealthBar(std::shared_ptr<cugl::scene2::ProgressBar> bar){
        _healthBar = bar;
        _healthBar->setScale(0.1);
        _healthBar->setAnchor(Vec2::ANCHOR_CENTER);
    }
    
    void setFinalEnemy(std::shared_ptr<cugl::scene2::SceneNode> baseNode){
        topLevelPlaceHolder = baseNode;
        topLevelPlaceHolder->removeAllChildren();
        runAnimations->setPosition(topLevelPlaceHolder->getAnchor());
        topLevelPlaceHolder->addChild(runAnimations);
        attackAnimations->setPosition(topLevelPlaceHolder->getAnchor());
        topLevelPlaceHolder->addChild(attackAnimations);
        _healthBar->setPosition(0, runAnimations->getHeight()/2);
        topLevelPlaceHolder->addChild(_healthBar);
    }
    void setWalkingSceneNode(std::shared_ptr<AnimationSceneNode> walkingNode){
        runAnimations = walkingNode;
    }
    
    void setAttackingSceneNode(std::shared_ptr<AnimationSceneNode> attackingNode){
        attackAnimations = attackingNode;
    }
    
    std::shared_ptr<cugl::scene2::SceneNode> getTopLevelNode(){
        return topLevelPlaceHolder;
    }
    
protected:
    int _maxHealth;
    int _health;
    int targetIndex;
    int updateRate;
    int _counter;
    
    /** The next step along the enemy's path */
    Vec2 _nextStep = Vec2(-1, -1);
    
    EnemyActions curAction;
    AnimationSceneNode::Directions _prevDirection;
    AnimationSceneNode::Directions _curDirection;
    std::shared_ptr<cugl::scene2::SceneNode> topLevelPlaceHolder;
    std::shared_ptr<AnimationSceneNode> runAnimations;
    std::shared_ptr<AnimationSceneNode> attackAnimations;
    std::shared_ptr<AStarSearch<WorldSearchVertex>> _pathfinder;
    
    /** The health  bar */
    std::shared_ptr<cugl::scene2::ProgressBar>  _healthBar;
    
    /** Timers */
    int _pathfindTimer = PATHFIND_COOLDOWN;
    
    /** Sets a new goal for this enemy to go to. Returns true if pathfinding to the goal was successful */
    bool setGoal(Vec2 goal, std::shared_ptr<World> world){
        
        // If we already pathfound recently, don't path find again
        if(_pathfindTimer < PATHFIND_COOLDOWN){
            return searchSuccess();
        }
        
        // Garbage collect the nodes used for the previous path if they exist
        if(_pathfinder->GetSolutionEnd() && _pathfinder->SearchStep() == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SUCCEEDED){
//            CULog("Garbage Collecting Path...");
            _pathfinder->FreeSolutionNodes();
        }
        
        cugl::Vec2 my_pos = getPosition();
        
        // Initialize start and end for the search
        WorldSearchVertex start = WorldSearchVertex(static_cast<int>(my_pos.x), static_cast<int>(my_pos.y), world);
        WorldSearchVertex end = WorldSearchVertex(static_cast<int>(goal.x), static_cast<int>(goal.y), world);
        _pathfinder->SetStartAndGoalStates(start, end);
        
        // Perform the search
        unsigned int SearchState = _pathfinder->SearchStep();
        while( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SEARCHING )
        {
            SearchState = _pathfinder->SearchStep();
        }
        
        // Check if the search was successful
        if( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SUCCEEDED ){
            
//            CULog("Found Solution from (%d, %d) to (%d, %d)", start.x, start.y, end.x, end.y);
            
            _pathfinder->GetSolutionStart();
            WorldSearchVertex* nextNode = _pathfinder->GetSolutionNext();
            
            if(nextNode){
                _nextStep = Vec2((int) nextNode->x, (int) nextNode->y);
            } else{
                _nextStep = my_pos;
            }
            
            return true;
        }
        
        if( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_FAILED ){
            CULog("Search Failed");
        }
        
        if( SearchState == AStarSearch<WorldSearchVertex>::SEARCH_STATE_OUT_OF_MEMORY ){
            CULog("Search Out of Memory");
        }
        
        return false;
    };
    
    /** Goes towards the goal. If there is no goal, do nothing. */
    void goToGoal(){
        
        // Get the goal
        WorldSearchVertex* goalNode = _pathfinder->GetSolutionEnd();
        cugl::Vec2 direction;
        
        // If there is no goal or we are already at the goal, do nothing
        if(!goalNode || atGoal() || _nextStep.x == -1){
            return;
        }
        
        Vec2 goalTile = Vec2(goalNode->x + 0.5, goalNode->y + 0.5);
        
        // If we are very close to the goal, go directly to it instead of using pathfinding
        if(getPosition().distance(goalTile) < CLOSE_DISTANCE){
            direction = goalTile - getPosition();
        }
        else {
            
            // If we strayed too far from the pathfinding path, restart pathfinding
            Vec2 nextTile = Vec2(_nextStep.x + 0.5, _nextStep.y + 0.5);
            if(getPosition().distance(nextTile) > STRAY_DISTANCE){
                CULog("Recalculating Path...");
                setGoal(Vec2(goalNode->x, goalNode->y), goalNode->_world);
            }
            
            // If we already reached the next tile, get the next node along the path and set it as the next tile
            if(atTile(_nextStep)){
                
                WorldSearchVertex* nextNode = _pathfinder->GetSolutionNext();
                
                if(nextNode){
                    _nextStep = Vec2((int) nextNode->x, (int) nextNode->y);
                } else{
                    return;
                }
            
            }
            
            direction = _nextStep - getPosition();
        }
        
        //Move towards the next tile
        setVX(direction.normalize().x * 0.5);
        setVY(direction.normalize().y * 0.5);
        setX(getX());
        setY(getY());
        _prevDirection =_curDirection;
        _curDirection = AnimationSceneNode::convertRadiansToDirections(direction.getAngle());
        
        
        
        
        
        // If we have been stuck on the same tile for too long, move randomly and then restart pathfinding
        
    };
    
    /** Returns whether the enemy is at its goal */
    bool atGoal(){
        // Get the goal
        WorldSearchVertex* goalNode = _pathfinder->GetSolutionEnd();
        
        Vec2 goal = Vec2(goalNode->x, goalNode->y);
        
        return atTile(goal);
    };
    
    /** Returns whether the enemy is on the given tile , i. e. close  to its center. Tiles should have integer coordinates*/
    bool atTile(Vec2 tile){
        
        // Get the center of the tile
        Vec2 tileCenter = Vec2(tile.x + 0.5, tile.y + 0.5);
        
        // Check if the enemy position is close to the center
        if(getPosition().distance(tileCenter) < 0.05 ){
            return true;
        };
        
        return false;
    };
    
    /** Returns whether the last pathfind was successful */
    bool searchSuccess(){
        return _pathfinder->GetSolutionEnd() && _pathfinder->SearchStep() == AStarSearch<WorldSearchVertex>::SEARCH_STATE_SUCCEEDED;
    }
    
};
#endif /* AbstractEnemy_h */
