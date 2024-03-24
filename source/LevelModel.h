//
//  JSLevelModel.hpp
//  JSONDemo
//
//  This module provides a level that can be loaded from a JSON file.  It reads the JSON
//  file and creates the objects in the level.  Objects are one of four categories: the
//  rocket, the exit, the walls, and the crates.
//
//  This class is a subclass of Asset so that we can use a GenericLoader to manage the
//  the loading.  Even though this class defines a load method, there is the question of
//  when the method is called. The GenericLoader allows this object to be loaded in a
//  separate thread along with all of the other assets.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White and Anthony Perello
//  Version: 3/12/17
//
#ifndef __TD_LEVEL_MODEL_H__
#define __TD_LEVEL_MODEL_H__
#include <cugl/cugl.h>
#include <cugl/physics2/CUObstacleWorld.h>
#include <vector>
#include <cugl/assets/CUAsset.h>
#include <cugl/io/CUJsonReader.h>
