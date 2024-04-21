//
//  DecoySet.hpp
//  Heavan
//
//  Created by Andrew Cheng on 3/22/24.
//

#ifndef DecoySet_hpp
#define DecoySet_hpp
#include "Decoy.h"
#include "SpriteAnimationNode.h"
#include <unordered_set>
#include <vector>
#include <cugl/cugl.h>
class DecoySet{

private:
    std::shared_ptr<cugl::scene2::SceneNode> decoySetNode;
    std::vector<std::shared_ptr<Decoy>> _currentDecoys;
    std::vector<std::shared_ptr<Decoy>> _removedDecoys;
    std::vector<std::shared_ptr<Decoy>> _pendingDecoys;
    std::shared_ptr<cugl::Texture> _texture;
    bool justAdded;
    std::shared_ptr<cugl::Texture> _explodeTexture;
    std::unordered_set<std::shared_ptr<SpriteAnimationNode>> _curAnimations;
public:
    
    std::shared_ptr<cugl::scene2::SceneNode> getDecoySetNode(){
        return decoySetNode;
    }
    std::vector<std::shared_ptr<Decoy>>& getCurrentDecoys(){
        return _currentDecoys;
    }
    std::vector<std::shared_ptr<Decoy>>& getPendingDecoys(){
        return _pendingDecoys;
    }
    std::vector<std::shared_ptr<Decoy>>& getRemovedDecoys(){
        return _removedDecoys;
    }
    bool addedNewDecoy(){
        return justAdded;
    }
    bool removedDecoy(){
        return _removedDecoys.size() != 0;
    }
    void init();
    void update(float timestep);
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
    void setTexture(const std::shared_ptr<cugl::Texture> &value);
    void setExplodeTexture(const std::shared_ptr<cugl::Texture> &value);
    void addNewDecoy(cugl::Vec2 pos);
    void postUpdate();
};
#endif /* DecoySet_hpp */
