//
//  UIController.cpp
//  Heavan
//
//  Created by Colin Wu on 3/28/24.
//

#include "UIController.h"
using namespace cugl;

bool UIController::init(const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size screenSize, std::shared_ptr<Dog> dog){
    
    // Get gameplay ui elements
    _screenSize = screenSize;
    _healthframe = assets->get<Texture>("healthframe");
    _healthfill = assets->get<Texture>("healthfill");
    _sizeframe = assets->get<Texture>("sizeframe");
    _sizefill = assets->get<Texture>("sizefill");
    _bombtoggle = assets->get<Texture>("bombtoggle");
    _shoottoggle = assets->get<Texture>("shoottoggle");
    _baittoggle = assets->get<Texture>("baittoggle");
    _dog = dog;
    
    return true;
}

std::shared_ptr<cugl::Texture> UIController::getHealthBarTexture(float health){
    // The percentage of the health bar that is empty space, needed to adjust how fast the health bar decreases
    GLfloat emptyPercent = 15.0/_healthfill->getWidth();
    
    GLfloat minS = 0;
    GLfloat maxS = emptyPercent + (1.0 - emptyPercent) * health/100.0;
    GLfloat minT = 0;
    GLfloat maxT = 1;
    
    return _healthfill->getSubTexture(minS, maxS, minT, maxT);
}

std::shared_ptr<cugl::Texture> UIController::getSizeBarTexture(float size){
    // The percentage of the size bar that is empty space, needed to adjust how fast the health bar decreases
    GLfloat emptyPercent = 3.2/_sizefill->getWidth();
    
    GLfloat minS = 0;
    GLfloat maxS = 1;
    GLfloat minT = 1 - (emptyPercent + (1.0 - emptyPercent) * size/MAX_ABSORB);
    GLfloat maxT = 1;
    
    return _sizefill->getSubTexture(minS, maxS, minT, maxT);
}

void UIController::draw(const std::shared_ptr<cugl::SpriteBatch>& batch){
    
    //Toggle
    cugl::Affine2 toggletrans;
    toggletrans.scale(UI_SCALE);
    toggletrans.translate(_screenSize.width - _bombtoggle->getWidth() * UI_SCALE, _screenSize.height - _bombtoggle->getHeight() * UI_SCALE);
   
    if(_dog->getMode() == "BOMB"){
        batch->draw(_bombtoggle, origin, toggletrans);
    } else if (_dog->getMode() == "SHOOT"){
        batch->draw(_shoottoggle, origin, toggletrans);
    } else if (_dog->getMode() == "BAIT"){
        batch->draw(_baittoggle, origin, toggletrans);
    }
    
    // Health bar
    cugl::Affine2 healthframetrans;
    healthframetrans.scale(UI_SCALE);
    healthframetrans.translate(0, _screenSize.height - _healthframe->getHeight() * UI_SCALE);
    
    cugl::Affine2 healthfilltrans;
    healthfilltrans.scale(UI_SCALE);
    healthfilltrans.translate(-1 * UI_SCALE, _screenSize.height - (_healthfill->getHeight() - 11) * UI_SCALE);
    
    batch->draw(getHealthBarTexture(_dog->getHealth()), origin, healthfilltrans);
    batch->draw(_healthframe, origin, healthframetrans);
    
    
    // Size bar
    cugl::Affine2 sizetrans;
    sizetrans.scale(UI_SCALE);
    sizetrans.translate(0, 0);
    batch->draw(_sizeframe, origin, sizetrans);
    batch->draw(getSizeBarTexture(_dog->getAbsorb()), origin, sizetrans);
}
