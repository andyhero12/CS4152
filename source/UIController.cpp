//
//  UIController.cpp
//  Heavan
//
//  Created by Colin Wu on 3/28/24.
//

#include "UIController.h"
using namespace cugl;
#define ANIM_FREQ 5

bool UIController::init(std::shared_ptr<cugl::scene2::SceneNode> node, const std::shared_ptr<cugl::AssetManager>& assets, cugl::Size screenSize){
//    _childOffset = -1;
//    // Get gameplay ui elements
    _screenSize = screenSize;
    UInode = node;
    _healthframe = cugl::scene2::PolygonNode::allocWithTexture(assets->get<Texture>("healthframe"));
    _healthfill = SubTextureNode::allocWithTexture(assets->get<Texture>("healthfill"));
    _sizeframe = cugl::scene2::PolygonNode::allocWithTexture(assets->get<Texture>("sizeframe"));
    _sizefill = SubTextureNode::allocWithTexture(assets->get<Texture>("sizefill"));
    _bombtoggle = cugl::scene2::PolygonNode::allocWithTexture(assets->get<Texture>("bombtoggle"));
    _shoottoggle = cugl::scene2::PolygonNode::allocWithTexture(assets->get<Texture>("shoottoggle"));
    _baittoggle = cugl::scene2::PolygonNode::allocWithTexture(assets->get<Texture>("baittoggle"));
    _hometoggle = cugl::scene2::PolygonNode::allocWithTexture(assets->get<Texture>("hometoggle"));
    _initialFlash = SpriteAnimationNode::allocWithSheet(assets->get<Texture>("initialflash"), 1, 5, 5, 6);
    _toggleFlash = SpriteAnimationNode::allocWithSheet(assets->get<Texture>("flash"), 1, 5, 5, 10);
    
    
    // set the scale
    _healthframe->setScale(UI_SCALE);
    _healthfill->setScale(UI_SCALE);
    _sizeframe->setScale(UI_SCALE);
    _sizefill->setScale(UI_SCALE);
    _bombtoggle->setScale(UI_SCALE);
    _shoottoggle->setScale(UI_SCALE);
    _baittoggle->setScale(UI_SCALE);
    _hometoggle->setScale(UI_SCALE);
    _initialFlash->setScale(UI_SCALE);
    _toggleFlash->setScale(UI_SCALE);
    
    // set the position
    _healthframe->setAnchor(Vec2::ANCHOR_CENTER);
    _healthfill->setAnchor(Vec2::ANCHOR_CENTER);
    _sizeframe->setAnchor(Vec2::ANCHOR_CENTER);
    _sizefill->setAnchor(Vec2::ANCHOR_CENTER);
    _bombtoggle->setAnchor(Vec2::ANCHOR_CENTER);
    _shoottoggle->setAnchor(Vec2::ANCHOR_CENTER);
    _baittoggle->setAnchor(Vec2::ANCHOR_CENTER);
    _hometoggle->setAnchor(Vec2::ANCHOR_CENTER);
    _initialFlash->setAnchor(Vec2::ANCHOR_CENTER);
    _toggleFlash->setAnchor(Vec2::ANCHOR_CENTER);
    
    x =0;
    y =0;
    

    float healthx = x + UI_SCALE * _healthframe->getTexture()->getWidth()/2;
    float healthy = y + screenSize.height - UI_SCALE * _healthframe->getTexture()->getHeight()/2;
    
    _healthframe->setPosition(healthx, healthy);
    
    float healthfillx = x + UI_SCALE * _healthfill->getTexture()->getWidth()/2;
    
    _healthfill->setPosition(healthfillx, healthy);
    
    float sizex = x + UI_SCALE * _sizeframe->getTexture()->getWidth()/2;
    float sizey = y + UI_SCALE * _sizeframe->getTexture()->getHeight()/2;
    
    float sizefillx = x + UI_SCALE * _sizefill->getTexture()->getWidth()/2;
    float sizefilly = y + UI_SCALE * _sizefill->getTexture()->getHeight()/2;
    
    _sizeframe->setPosition(sizex , sizey);
    _sizefill->setPosition(sizefillx , sizefilly);
    
    float togglex = x + screenSize.width - UI_SCALE * _baittoggle->getTexture()->getWidth()/2;
    float toggley = y + screenSize.height - UI_SCALE * _baittoggle->getTexture()->getHeight()/2;
    
    _bombtoggle->setPosition(togglex, toggley);
    _shoottoggle->setPosition(togglex, toggley);
    _baittoggle->setPosition(togglex, toggley);
    _hometoggle->setPosition(togglex, toggley);
    _initialFlash->setPosition(togglex, toggley);
    _toggleFlash->setPosition(togglex, toggley);
    
    _shoottoggle->setVisible(true);
    _bombtoggle->setVisible(false);
    _baittoggle->setVisible(false);
    _hometoggle->setVisible(false);
    _initialFlash->setVisible(false);
    _toggleFlash->setVisible(true);
    
    node->addChild(_healthframe);
    node->addChild(_sizeframe);
    node->addChild(_bombtoggle);
    node->addChild(_shoottoggle);
    node->addChild(_baittoggle);
    node->addChild(_hometoggle);
    node->addChild(_healthfill);
    node->addChild(_sizefill);
    node->addChild(_toggleFlash);
    node->addChild(_initialFlash);
    
    return true;
}

void UIController::setHealthBarTexture(float percentage){
    CUAssert(0 <= percentage <= 1);
    // The percentage of the health bar that is empty space, needed to adjust how fast the health bar decreases
    GLfloat emptyPercent = 15.0/_healthfill->getWidth();
    
    GLfloat minS = 0;
    GLfloat maxS = emptyPercent + (1.0 - emptyPercent) * percentage;
    GLfloat minT = 0;
    GLfloat maxT = 1;
    
    _healthfill->setSubtexture(minS, maxS, minT, maxT);
    
    float healthy = y + _screenSize.height - UI_SCALE * _healthframe->getTexture()->getHeight()/2;
    float healthfillx = x + UI_SCALE * _healthfill->getTexture()->getWidth()/2;
    _healthfill->setAnchor(Vec2::ANCHOR_CENTER);
    _healthfill->setPosition(healthfillx, healthy);
}

void UIController::setSizeBarTexture(float percentage){
    CUAssert(0 <= percentage <= 1);
    // The percentage of the size bar that is empty space, needed to adjust how fast the health bar decreases
    GLfloat emptyPercent = 3.2/_sizefill->getWidth();
    
    GLfloat minS = 0;
    GLfloat maxS = 1;
    GLfloat minT = 1 - (emptyPercent + (1.0 - emptyPercent) * percentage);
    GLfloat maxT = 1;

    _sizefill->setSubtexture(minS, maxS, minT, maxT);
    
    float sizefillx = x + UI_SCALE * _sizefill->getTexture()->getWidth()/2;
    float sizefilly = y + UI_SCALE * _sizefill->getTexture()->getHeight()/2;

    _sizefill->setPosition(sizefillx , sizefilly);
}

void UIController::setToggle(std::string mode){
    if (mode == "SHOOT") {
        _bombtoggle->setVisible(false);
        _shoottoggle->setVisible(true);
        _baittoggle->setVisible(false);
        _hometoggle->setVisible(false);
    }
    else if (mode == "BAIT"){
        _bombtoggle->setVisible(false);
        _shoottoggle->setVisible(false);
        _baittoggle->setVisible(true);
        _hometoggle->setVisible(false);
    }
    else if (mode == "BOMB"){
        _bombtoggle->setVisible(true);
        _shoottoggle->setVisible(false);
        _baittoggle->setVisible(false);
        _hometoggle->setVisible(false);
    }
    else if (mode == "NOTHING"){
        _bombtoggle->setVisible(false);
        _shoottoggle->setVisible(false);
        _baittoggle->setVisible(false);
        _hometoggle->setVisible(false);
    }else if (mode == "RECALL"){
        _bombtoggle->setVisible(false);
        _shoottoggle->setVisible(false);
        _baittoggle->setVisible(false);
        _hometoggle->setVisible(true);
    }else{
        CUAssertLog(false, "wrong mode string");
    }
}

void UIController::animateFlash(int absorb){
    if(absorb > 10){
        if(!_flashAnimated){
            _initialFlash->setVisible(true);
            _initialFlash->update();
            
            if(_initialFlash->getFrame() == _initialFlash->getSpan() - 1){
                _flashAnimated = true;
            }
        } else{
            _toggleFlash->update();
        }
        
    } else{
        _initialFlash->setVisible(false);
        _flashAnimated = false;
        _initialFlash->setFrame(0);
        _toggleFlash->setFrame(3);
    }
}
