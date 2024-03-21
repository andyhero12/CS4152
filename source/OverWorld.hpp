//
//  OverWorld.hpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//
#pragma once
#ifndef OverWorld_hpp
#define OverWorld_hpp
#include <cugl/cugl.h>
#include <unordered_set>
#include "Base.h"
#include "Dog.h"
#include "Animation.h"
#include "Decoy.hpp"
#include "Spawner.h"
#include "BaseSet.h"
#include "InputController.h"
#include <stdio.h>

#endif /* OverWorld_hpp */


class OverWorld{
private:
        std::shared_ptr<Dog> _dog;
        std::vector<std::shared_ptr<Decoy>> _decoys;
        std::vector<std::shared_ptr<Spawner>> _spawners;
        std::shared_ptr<BaseSet> _bases;
        
    
public:
    
    OverWorld(){
        
    }
    ~OverWorld(){
        
    }
    void dogUpdate(InputController& _input,cugl::Size totalSize);
    void update(InputController& input, cugl::Size totalSize);
    void reset(std::shared_ptr<cugl::JsonValue> _constants,
                          cugl::Size resetSize);
    
    std::shared_ptr<Dog> getDog(){
        return _dog;
    }
    std::vector<std::shared_ptr<Decoy>> getDecoys(){
        return _decoys;
    }
    
    void setDog(std::shared_ptr<Dog> m_dog){
        _dog = m_dog;
    }
};
