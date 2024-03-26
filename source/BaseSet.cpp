//
//  BaseSet.cpp
//
//  Created by Andrew Cheng on 2/26/24.
//
#include "BaseSet.h"

BaseSet::BaseSet()
{
}
BaseSet::~BaseSet()
{
}

void BaseSet::update()
{
    for (auto &base : _bases)
    {
        base->update();
    }
}
int BaseSet::getFirstHealth()
{
    return (*_bases.begin())->getHealth();
}
bool BaseSet::init(std::shared_ptr<cugl::JsonValue> data)
{
    if (data)
    {
        _bases.clear();
        if (data->get("start"))
        {
            auto baseValues = data->get("start")->children();
            for (auto it = baseValues.begin(); it != baseValues.end(); it++)
            {
                std::shared_ptr<cugl::JsonValue> entry = (*it);
                cugl::Vec2 pos;
                // TEMPORARY MAGIC NUMBERS
                pos.x = entry->get(0)->get(0)->asFloat(0) / 64;
                pos.y = entry->get(0)->get(1)->asFloat(0) / 64;
                int health = entry->get(1)->asInt(0);
                _bases.emplace_back(std::make_shared<Base>(health, pos));
            }
        }
        return true;
    }
    return false;
}

void BaseSet::setTexture(const std::shared_ptr<cugl::Texture> &value)
{
    _texture = value;
}

void BaseSet::draw(const std::shared_ptr<cugl::SpriteBatch> &batch, cugl::Size size)
{
    for (auto &base : _bases)
    {
        cugl::Vec2 pos = base->getPos();
        cugl::Vec2 origin(0, 0);
        cugl::Affine2 trans;
        float scale = 1;
        trans.scale(scale / _texture->getHeight());
        trans.translate(pos);
        batch->draw(_texture, origin, trans);
    }
}
bool BaseSet::baseLost()
{
    for (auto &base : _bases)
    {
        if (base->lost())
        {
            return true;
        }
    }
    return false;
}
