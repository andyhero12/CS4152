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
    _dog = dog;
    
    return true;
}

std::shared_ptr<cugl::Texture> UIController::getHealthBarTexture(float health){
    
    GLfloat minS = 0;
    GLfloat maxS = health/100.0;
    GLfloat minT = 0;
    GLfloat maxT = 1;
    
    return _healthfill->getSubTexture(minS, maxS, minT, maxT);
}

std::shared_ptr<cugl::Texture> UIController::getSizeBarTexture(float size){
    
    GLfloat minS = 0;
    GLfloat maxS = 1;
    GLfloat minT = 1 - size/MAX_ABSORB;
    GLfloat maxT = 1;
    
    return _sizefill->getSubTexture(minS, maxS, minT, maxT);
}

void UIController::draw(const std::shared_ptr<cugl::SpriteBatch>& batch){
    Vec2 origin = Vec2(0, 0);
    
    //Toggle
    cugl::Affine2 bombtrans;
    bombtrans.scale(UI_SCALE);
    bombtrans.translate(_screenSize.width - _bombtoggle->getWidth() * UI_SCALE, _screenSize.height - _bombtoggle->getHeight() * UI_SCALE);
    batch->draw(_bombtoggle, origin, bombtrans);
    
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
