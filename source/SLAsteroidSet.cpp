//
//  AsteroidSet.cpp
//
//  This class implements a collection of asteroids. As with the ship, we are
//  going to dynamically allocate asteroids and use smart pointers to reference
//  them. That way they are garbage collected when we delete an asteroid.
//
//  But we also need something to store all these smart pointers.  We cannot
//  have a variable for each asteriod -- we don't know how many there will be.
//  Technically, we could just use an unordered_set for this.  This is a collection
//  type in C++. But as we saw in the intro class, sometimes it makes sense to
//  factor out information that is common to all asteroids into the container
//  class.  And that is what we are doing here.
//
//  Author: Walker White
//  Based on original GameX Ship Demo by Rama C. Hoetzlein, 2002
//  Version: 2/21/21
//
#include "SLAsteroidSet.h"
#include <random>

using namespace cugl;

#pragma mark Asteroid
/**
 * Allocates an asteroid by setting its position and velocity.
 *
 * A newly allocated asteroid has type 3, the largest type.
 *
 * @param p     The position
 * @param v     The velocity
 */
AsteroidSet::Asteroid::Asteroid(const cugl::Vec2 p, const cugl::Vec2 v) : Asteroid(p, v, 3) {}

// Function to generate a random value between 1 and 3
int generateRandomValuelowToHigh(int low, int high)
{
    // Static used for the seed to ensure it's only seeded once
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high); // Range is 1 to 3, inclusive
    return dis(gen);
}

/**
 * Allocates an asteroid by setting its position, velocity, and type
 *
 * @param p     The position
 * @param v     The velocity
 * @param type  The type (1, 2, or 3)
 */
AsteroidSet::Asteroid::Asteroid(const cugl::Vec2 p, const cugl::Vec2 v, int type): Asteroid(p, v, type, 0, 5) {}


int AsteroidSet::Asteroid::getDamage() {
    if (_attackCooldown == 60){
        _attackCooldown = 0;
        return _damage;
    }
    else{
        return 0;
    }
}



AsteroidSet::Asteroid::Asteroid(const cugl::Vec2 p, const cugl::Vec2 v, int type, int target, int damage): _damage(damage)
{
    position = p;
    velocity = v;
    _targetIndex = target;
    setType(type);
    _attackCooldown = 15;
    _health = type;
}


/**
 * Returns the type of this asteroid.
 *
 * All asteroids have types 1, 2, or 3.  3 is the largest type of
 * asteroid (scale 1.25), while 1 is the smallest (scale of 0.5).
 *
 * @param type  The type of this asteroid.
 */
void AsteroidSet::Asteroid::setType(int type)
{
    CUAssertLog(type > 0 && type <= 3, "type must be 1, 2, or 3");
    _type = type;
    switch (type)
    {
    case 3:
        _scale = 1.25;
        break;
    case 2:
        _scale = 0.85;
        break;
    case 1:
        _scale = .5;
        break;
    default:
        _scale = 0.0f;
        break;
    }
}

/**
 * Sets the current ship health.
 *
 * When the health of the ship is 0, it is "dead"
 *
 * @param value The current ship health.
 */
void AsteroidSet::Asteroid::setHealth(int value) {
    if (value >= 0) {
        // Do not allow health to go negative
        _health = value;
    } else {
        _health = 0;
    }
}

/**
 * Sets the sprite sheet for this asteroid.
 *
 * Note the parameter type. The const is applied to the pointer,
 * not the sprite sheet. So this means it is not okay to
 * change the contents of the pointer, but it is okay to
 * change the contents (the frame) of the sprite sheet
 *
 * @param texture   The sprite sheet for this asteroid.
 */
void AsteroidSet::Asteroid::setSprite(const std::vector<std::shared_ptr<cugl::Texture>>& value, int rows, int _framecols, int _framesize, cugl::Vec2 origin ){

    std::vector<std::shared_ptr<cugl::SpriteSheet>> anims;
    for(auto& text : value) {

        std::shared_ptr<cugl::SpriteSheet> _sprite = SpriteSheet::alloc(text, rows, _framecols, _framesize);
        anims.push_back(_sprite);
//
    }
    _animations.setOrigin(origin);
    _animations = Animation(1, anims, 10, 0);
}

int convertToQuadrant(double radian) {
//    radian = radian + (M_PI / 16);
//    int quadrant = (radian + 2 * M_PI) / (M_PI / 2);  // Determine the quadrant
    double angleInDegrees = radian * (180 / M_PI);
    // Calculate the "quadrant" in an 8-part division
    int quadrant = static_cast<int>(std::floor(angleInDegrees / 45.0)) % 8;
//    std::cout<< angle << " " << (quadrant + 4 ) % 8 << std::endl;
    return ( quadrant + 8 ) % 8;  // Return the quadrant number (0 to 7)
}

/**
 * Moves the asteroid one animation frame
 *
 * This movement code supports "wrap around".  If the asteroid
 * goes off one edge of the screen, then it appears across the
 * edge on the opposite side. However, this method performs no
 * collision detection. Collisions are resolved afterwards.
 */
void AsteroidSet::Asteroid::update(Size size, const std::vector<cugl::Vec2>& bases, const std::shared_ptr<Ship>& ship, std::vector<std::shared_ptr<Decoy>>& decoys)
{


    if (_attackCooldown < 60){
        _attackCooldown += 1;
    }
  
//    _animations.resetAnimation();
    
    cugl::Vec2 target_pos;
    if (_targetIndex == 0){
        target_pos = ship->getPosition();
    }else if (_targetIndex <= bases.size()){
        target_pos = bases[_targetIndex-1];
    }else{
        target_pos = decoys[_targetIndex-1-bases.size()]->getPos();
    }
    cugl::Vec2 direction = target_pos- position;
    int dir_quad = convertToQuadrant(direction.getAngle());
    if (_prevDir != dir_quad)
    {
        _animations.resetAnimation(dir_quad);
    }
    _prevDir = dir_quad;
    _animations.updateAnimTime();
    if (_animations.frameUpdateReady()){
        _animations.stepAnimation();
    }
//    CULog("x %f, y %f", direction.x, direction.y);
    position += direction.normalize();
//    position = destination;
    //    position += velocity;
    while (position.x > size.width) {
        position.x = size.width;
    }
    while (position.x < 0) {
        position.x = 0;
    }
    while (position.y > size.height) {
        position.y = size.height;
    }
    while (position.y < 0) {
        position.y = 0;
    }
}

#pragma mark Asteroid Set
/**
 * Creates an asteroid set with the default values.
 *
 * To properly initialize the asteroid set, you should call the init
 * method with the JSON value. We cannot do that in this constructor
 * because the JSON value does not exist at the time the constructor
 * is called (because we do not create this object dynamically).
 */
AsteroidSet::AsteroidSet() : _mass(0),
                             _radius(0),
                             _framecols(0),
                             _framesize(0) {}

/**
 * Initializes asteroid data with the given JSON
 *
 * This JSON contains all shared information like the mass and the
 * sprite sheet dimensions. It also contains a list of asteroids to
 * spawn initially.
 *
 * If this method is called a second time, it will reset all
 * asteroid data.
 *
 * @param data  The data defining the asteroid settings
 *
 * @return true if initialization was successful
 */
bool AsteroidSet::init(std::shared_ptr<cugl::JsonValue> data,std::shared_ptr<Ship> shipParam)
{
    if (data)
    {
        // Reset all data
        current.clear();
        _pending.clear();
        _currentDecoys.clear();
        _pendingDecoys.clear();
        _target.clear();

        _mass = data->getFloat("mass", 0);
        _damage = data->getInt("damage", 0);
        _hitratio = data->getFloat("hit ratio", 1);
        _framecols = data->getFloat("sprite cols", 0);
        _framesize = data->getFloat("sprite size", 0);
        if (data->get("bases"))
        {
            auto base = data->get("bases")->children();
            for (auto it = base.begin(); it != base.end(); ++it)
            {
                std::shared_ptr<JsonValue> entry = (*it);
                Vec2 pos;
                pos.x = entry->get(0)->asFloat(0);
                pos.y = entry->get(1)->asFloat(0);
                _target.emplace_back(pos);
            }
        }

        // This is an iterator over all of the elements of rocks
        if (data->get("start"))
        {
            auto rocks = data->get("start")->children();
            for (auto it = rocks.begin(); it != rocks.end(); ++it)
            {
                std::shared_ptr<JsonValue> entry = (*it);
                Vec2 pos;
                pos.x = entry->get(0)->get(0)->asFloat(0);
                pos.y = entry->get(0)->get(1)->asFloat(0);
                Vec2 vel;
                vel.x = entry->get(1)->get(0)->asFloat(0);
                vel.y = entry->get(1)->get(1)->asFloat(0);
                spawnAsteroid(pos, vel, generateRandomValuelowToHigh(1,3));
            }
        }
        _ship = shipParam;
        return true;
    }
    return false;
}

/**
 * Adds an asteroid to the active queue.
 *
 * All asteroids are added to a pending set; they do not appear in the current
 * set until {@link #update} is called. This makes it safe to add new asteroids
 * while still looping over the current asteroids.
 *
 * @param p     The asteroid position.
 * @param v     The asteroid velocity.
 * @param type  The asteroid type.
 */
void AsteroidSet::spawnAsteroid(Vec2 p, Vec2 v, int t)
{

    int index = generateRandomValuelowToHigh(0,getTotalTargets()-1);
    // Determine direction and velocity of the photon.
    std::shared_ptr<Asteroid> rock = std::make_shared<Asteroid>(p, v, t, index, _damage);
    if (_texture.size() > 0)
    {
        int rows = _framesize / _framecols;
        if (_framesize % _framecols != 0)
        {
            rows++;
        }
        rock->setSprite(_texture, rows,_framecols, _framesize, Vec2(_radius, _radius));
//        rock->setSprite(SpriteSheet::alloc(_texture, rows, _framecols, _framesize));
//        rock->getSprite()->setOrigin(Vec2(_radius, _radius));
    }
    _pending.emplace(rock);
}

/**
 * Moves all the asteroids in the active set.
 *
 * In addition, if any asteroids are in the pending set, they will appear
 * (unmoved) in the current set. The pending set will be cleared.
 *
 * This movement code supports "wrap around".  If the photon goes off one
 * edge of the screen, then it appears across the edge on the opposite
 * side. However, this method performs no collision detection. Collisions
 * are resolved afterwards.
 */
void AsteroidSet::update(Size size,float timestep)
{
    // Move asteroids, updating the animation frame
    for (auto it = current.begin(); it != current.end(); ++it)
    {
        (*it)->update(size,_target, _ship,_currentDecoys);
        auto sprite = (*it)->getSprite();
        int frame = sprite->getFrame() + 1;
        if (frame >= sprite->getSize())
        {
            frame = 0;
        }
        sprite->setFrame(frame);
    }
    auto itD = _currentDecoys.begin();
    bool decoyGone = false;
    while (itD != _currentDecoys.end()){
        std::shared_ptr<Decoy> curDecoy = *itD;
        curDecoy->update(timestep);
        CULog("time left %f", curDecoy->getTime());
        if (curDecoy->destroyed()){
            decoyGone = true;
            itD = _currentDecoys.erase(itD);
        }else {
            ++itD;
        }
    }
    if (decoyGone){
        targetAllToCloset();
    }
    // Move from pending to current
    for (auto it = _pending.begin(); it != _pending.end(); ++it)
    {
        current.emplace(*it);
    }
    _pending.clear();
    
    for ( auto it =  _pendingDecoys.begin(); it != _pendingDecoys.end(); it++){
        _currentDecoys.emplace_back(*it);
        retargetNewDecoy(*it);
    }
    _pendingDecoys.clear();
}

/**
 * Sets the image for a single photon; reused by all photons.
 *
 * This value should be loaded by the GameScene and set there. However,
 * we have to be prepared for this to be null at all times.  This
 * texture will be used to generate the sprite sheet for each
 * asteroid. Asteroids must have different sprite sheets because,
 * while they share a texture, they do not share the same animation
 * frame.
 *
 * The sprite sheet information (size, number of columns) should have
 * been set in the initial JSON. If not, this texture will be ignored.
 *
 * @param value the image for a single asteroid; reused by all asteroids.
 */
void AsteroidSet::setTexture(const std::vector<std::shared_ptr<cugl::Texture>>& value)
{
    if (_framecols > 0 && value.size() > 0)
    {
        int rows = _framesize / _framecols;
        if (_framesize % _framecols != 0)
        {
            rows++;
        }
        Size size = value[0]->getSize();
        size.width /= _framecols;
        size.height /= rows;

        _radius = std::max(size.width, size.height) / 2;
        _texture = value;

        // Update the sprite sheets of the asteroids as necessary
        for (auto it = current.begin(); it != current.end(); ++it)
        {
            std::shared_ptr<Asteroid> rock = (*it);
            rock->setSprite(_texture, rows,_framecols, _framesize, Vec2(_radius, _radius));
        }
        for (auto it = _pending.begin(); it != _pending.end(); ++it)
        {
            std::shared_ptr<Asteroid> rock = (*it);
            rock->setSprite(_texture, rows,_framecols, _framesize, Vec2(_radius, _radius));
        }
    }
    else
    {
        _radius = 0;
    }
}
void AsteroidSet::setDecoyTexture(const std::shared_ptr<cugl::Texture>& incomingDecoyTexture){
    _decoyTexture = incomingDecoyTexture;
}
/**
 * Draws all active asteroids to the sprite batch within the given bounds.
 *
 * This drawing code supports "wrap around". If a photon is partly off of
 * one edge, then it will also be drawn across the edge on the opposite
 * side.
 *
 * Pending asteroids are not drawn.
 *
 * @param batch     The sprite batch to draw to
 * @param size      The size of the window (for wrap around)
 */
void AsteroidSet::draw(const std::shared_ptr<SpriteBatch> &batch, Size size, std::shared_ptr<Font> font)
{
//    if (_texture)
    
    for (auto it = current.begin(); it != current.end(); ++it)
        {
            float scale = (*it)->getScale();
            Vec2 pos = (*it)->position;
            Vec2 origin(_radius, _radius);
            
            std::string hpMsg = strtool::format(std::to_string((*it)->getHealth()));
            std::shared_ptr<cugl::TextLayout> hptext = TextLayout::allocWithText(hpMsg, font);
            hptext->layout();

            Affine2 trans;
            trans.scale(scale);
            trans.translate(pos);
            auto sprite = (*it)->getSprite();
            
            sprite->draw(batch, trans);
            batch->drawText(hptext,trans);           
        }
    for (std::shared_ptr<Decoy> decoy: _currentDecoys){
        cugl::Vec2 pos = decoy->getPos();
        cugl::Vec2 origin(0, 0);
        cugl::Affine2 trans;
        float scale = 1;
        trans.scale(scale);
        trans.translate(pos);
        batch->draw(_decoyTexture, origin, trans);
    }
}


void AsteroidSet::createDecoy(){
    Vec2 curPos = _ship->getPosition();
    _pendingDecoys.emplace_back(std::make_shared<Decoy>(curPos));
}

void AsteroidSet::targetAllToCloset(){
    CULog("target all to Closet\n");
    Vec2 shipPos = _ship->getPosition();
    for (std::shared_ptr<Asteroid> enemy : current){
        Vec2 enemyPos = enemy->position;
        int index = 0;
        float dist = (enemyPos - shipPos).length();
        int sizeBases = (int) _target.size();
        for (int i = 0 ;i < sizeBases;i++){
            Vec2 basePos = _target[i];
            float curDist = (enemyPos -  basePos).length();
            if (curDist < dist){
                dist = curDist;
                index = i + 1;
            }
        }
        int sizeDecoys = (int)_currentDecoys.size();
        for (int i = 0 ;i < sizeDecoys; i++){
            Vec2 decoyPos = _currentDecoys[i]->getPos();
            float curDist = (enemyPos -  decoyPos).length();
            if (curDist < dist){
                dist = curDist;
                index = i + 1 + sizeBases;
            }
        }
        enemy->setTargetIndex(index);
    }
}
void AsteroidSet::retargetNewDecoy(const std::shared_ptr<Decoy>& curDecoy){
    CULog("retargeting Decoy \n");
    float retargetCutoff = 400.0;
    for (std::shared_ptr<Asteroid> enemy : current){
        Vec2 enemyPos = enemy->position;
        Vec2 norm = curDecoy->getPos() - enemyPos;
        float distance = norm.length();
        
        if (distance < retargetCutoff){
            CULog("total Targets %d\n", getTotalTargets()-1);
            enemy->setTargetIndex(getTotalTargets()-1); // newest one is always last
        }
    }
}
