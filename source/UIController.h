//
//  UIController.hpp
//  Heavan
//
//  Created by Colin Wu on 3/28/24.
//

#ifndef UIController_h
#define UIController_h

#include <cugl/cugl.h>
#include <stdio.h>
#include "Dog.h"

#define UI_SCALE 4.5

class UIController{
    
private:
    
    /** Game UI Elements **/
    cugl::Size _screenSize;
    std::shared_ptr<Dog> _dog;
    std::shared_ptr<cugl::Texture> _healthframe;
    std::shared_ptr<cugl::Texture> _healthfill;
    std::shared_ptr<cugl::Texture> _sizeframe;
    std::shared_ptr<cugl::Texture> _sizefill;
    std::shared_ptr<cugl::Texture> _bombtoggle;
    std::shared_ptr<cugl::Texture> _shoottoggle;
    std::shared_ptr<cugl::Texture> _baittoggle;
    
    std::shared_ptr<cugl::Texture> getHealthBarTexture(float health);
    std::shared_ptr<cugl::Texture> getSizeBarTexture(float size);
    
public:
        
    UIController(){}
    
    ~UIController(){}
    
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size screenSize, std::shared_ptr<Dog> dog);
    
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
    
};

#endif /* UIController_hpp */
