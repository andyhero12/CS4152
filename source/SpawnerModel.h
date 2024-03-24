//
//  JSExitModel.h
//  JSONDemo
//
//  This encapsulates all of the information for representing an exit door. Since the door
//  will never move, we normally would not need a separate class (because we could just use
//  an instance of BoxObstacle).  However, this changes when we do dynamic level loading.
//
//  The problem is that we need to initialize an physics object when a level is loaded,
//  but NOT attach any assets yet.  That is because the physics object is itself an asset
//  and it is not safe to assume the order of asset loading.  So instead, we just store
//  the keys of the assets in this class, and they attach the assets later when we
//  initialize the scene graph.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White and Anthony Perello
//  Version: 3/12/17
//
#ifndef __TD_EXIT_MODEL_H__
#define __TD_EXIT_MODEL_H__

#include <cugl/physics2/CUBoxObstacle.h>

using namespace cugl;


#pragma mark -
#pragma mark Spawner Model

/**
* An exit door in the rocket lander game.
*
* Note that this class does keeps track of its own assets via keys, so that
* they can be attached later as part of the scene graph.
*/
class SpawnerModel {

protected:
	/** The texture filmstrip for the exit door */
	std::string _exitTexture;
	
	const std::string& getTextureKey() const { return _exitTexture; }

};

#endif /* defined(__JS_EXIT_MODEL_H__) */
