//
//  MonsterController.h
//  Heavan
//
//  Created by Andrew Cheng on 3/21/24.
//

#ifndef MonsterController_h
#define MonsterController_h

#include "AbstractEnemy.h"
#include <unordered_set>

class MonsterController{
private:
    std::unordered_set<std::shared_ptr<AbstractEnemy>> _enemies;
public:
        
    
    MonsterController();
    
    void update(cugl::Size size, float timestep);
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size size,  std::shared_ptr<cugl::Font> font);
    
    std::unordered_set<std::shared_ptr<AbstractEnemy>>& getEnemies(){
        return _enemies;
    }
};

#endif /* MonsterController_h */
