//
//  Devil.cpp
//  Heavan
//
//  Created by Andrew Cheng on 3/20/24.
//

#include "Devil.h"

Devil::Devil(std::shared_ptr<Dog> inc_dog, const cugl::Vec2& pos, std::shared_ptr<cugl::JsonValue> data) {
    _pos = pos;
    _radius = 0;
    dog = inc_dog;
    // Sprite sheet information
    _framecols = data->getInt("sprite cols",0);
    _framesize = data->getInt("sprite size",0);
    _frameflat = data->getInt("sprite frame",0);
}
void Devil::setRunTexture(const std::vector<std::shared_ptr<cugl::Texture>>& texture){
    if (_framecols > 0) {
        int rows = _framesize/_framecols;
        if (_framesize % _framecols != 0) {
            rows++;
        }
        std::shared_ptr<cugl::SpriteSheet> _sprite ;
        std::vector<std::shared_ptr<cugl::SpriteSheet>> anims;
        for(auto& text : texture) {
            _sprite = cugl::SpriteSheet::alloc(text, rows, _framecols, _framesize);
            anims.push_back(_sprite);
//
        }
        runAnimation = Animation( anims, 10, _frameflat);
        cugl::Vec2 origin(runAnimation.getSprite()->getFrameSize()/2);
        runAnimation.setOrigin(origin);
        _radius = std::max(runAnimation.getSprite()->getFrameSize().width, _sprite->getFrameSize().height)/2;
    }
}
void Devil::move(cugl::Size size){
    
    cugl::Vec2 direction = dog->getPosition()- _pos ;
    if (direction.length() < 50){
        return;
    }
    _pos += direction.normalize();
    while (_pos.x > size.width) {
        _pos.x = size.width;
    }
    while (_pos.x < 0) {
        _pos.x = 0;
    }
    while (_pos.y > size.height) {
        _pos.y = size.height;
    }
    while (_pos.y < 0) {
        _pos.y = 0;
    }
}

void Devil::draw(const std::shared_ptr<cugl::SpriteBatch>& batch, cugl::Size bounds){
    // Don't draw if sprite not set
    if (runAnimation.getSprite()) {
        // Transform to place the ship
        cugl::Affine2 shiptrans;
        // super duper magic number
        shiptrans.scale(1.0 * 0.75f);
//        shiptrans.rotate(_ang*M_PI/180);
        shiptrans.translate(_pos);
        // Transform to place the shadow, and its color
//        Affine2 shadtrans = shiptrans;
//        shadtrans.translate(_shadows,-_shadows);
//        Color4f shadow(0,0,0,0.5f);
        
        runAnimation.getSprite()->draw(batch,shiptrans);
    }
}
