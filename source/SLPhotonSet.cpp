//
//  PhotonSet.cpp
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
#include "SLPhotonSet.h"
using namespace cugl;
#pragma mark Photon


PhotonSet::Photon::Photon(const cugl::Vec2 p, const cugl::Vec2 v, int mx)
    : position{p}
    , velocity{v}
    , _scale{1.5f}
    , _age{1}
    , _maxage{mx}
    {}

void PhotonSet::Photon::update(cugl::Size size){
    position += velocity;
    while (position.x > size.width) {
        position.x -= size.width;
    }
    while (position.x < 0) {
        position.x += size.width;
    }
    while (position.y > size.height) {
        position.y -= size.height;
    }
    while (position.y < 0) {
        position.y += size.height;
    }
    _age++;
    _scale = 1.5f - 1.5*_age/_maxage;
}
#pragma mark Photon Set
/**
 * Creates an photon set with the default values.
 *
 * To properly initialize the photon set, you should call the init
 * method with the JSON value. We cannot do that in this constructor
 * because the JSON value does not exist at the time the constructor
 * is called (because we do not create this object dynamically).
 */
PhotonSet::PhotonSet()
    : _speed(0.0f)
    , _mass(0.0f)
    , _radius(0.0f)
    , _maxage(0)
    {}

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
bool PhotonSet::init(std::shared_ptr<cugl::JsonValue> data){
    if (data) {
        // Reset all data
        current.clear();
        _speed = data->getFloat("speed",0);
        _mass = data->getFloat("mass",0);
        _maxage = data->getInt("max age",0);
        return true;
    }
    return false;
}


void PhotonSet::setTexture(const std::shared_ptr<cugl::Texture>& value) {
    if (value){
        cugl::Size size = value->getSize();
        _radius = std::max(size.width,size.height)/2;
        _texture = value;
    }else {
        _radius = 0;
        _texture = nullptr;
    }
}
/**
 * Moves all the photons in the active set.
 */
void PhotonSet::update(cugl::Size size){
    auto it = current.begin();
    while (it != current.end()){
        (*it)->update(size);
        auto curr = it++;
        if ((*curr)->getAge() == _maxage){
            current.erase(curr);
        }
    }
}

/**
 * Spawns a Photon and includes it into the photon set
 */
void PhotonSet::spawnPhoton(cugl::Vec2 p, cugl::Vec2 v){
    std::shared_ptr<Photon> myPhoton = std::make_shared<Photon>(p,v, _maxage);
    current.emplace(myPhoton);
}
/**
 * Draws all active Photons to the sprite batch within the given bounds.
 *
 * This drawing code supports "wrap around". If a photon is partly off of
 * one edge, then it will also be drawn across the edge on the opposite
 * side.
 *
 * @param size      The size of the window (for wrap around)
 */
void PhotonSet::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size){
    if (_texture){
        for (auto it = current.begin(); it != current.end(); ++it){
            float scale = (*it)->getScale();
            cugl::Vec2 pos = (*it)->position;
            cugl::Vec2 origin(_radius, _radius);
            cugl::Affine2 trans;
            trans.scale(scale);
            trans.translate(pos);
            batch->draw(_texture, origin, trans);
            float r = getRadius() * scale;
            if (pos.x+r > size.width) {
                trans.translate(-size.width,0);
                batch->draw(_texture, origin, trans);
                trans.translate(size.width,0);
            } else if (pos.x-r < 0) {
                trans.translate(size.width,0);
                batch->draw(_texture, origin, trans);
                trans.translate(-size.width,0);
            }
            if (pos.y+r > size.height) {
                trans.translate(0,-size.height);
                batch->draw(_texture, origin, trans);
                trans.translate(0,size.height);
            } else if (pos.y-r < 0) {
                trans.translate(0,size.height);
                batch->draw(_texture, origin, trans);
                trans.translate(0,-size.height);
            }
        }
    }
}
