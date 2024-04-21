//
//  NLBiteEvent.cpp
//  Networked Physics Lab
//
//  This class represents an event of creating an extra-large crate
//
//  Created by Andrew Cheng.
//

#include "NLBiteEvent.h"

/**
 * This method is used by the NetEventController to create a new event of using a
 * reference of the same type.
 *
 * Not that this method is not static, it differs from the static alloc() method
 * and all methods must implement this method.
 */
std::shared_ptr<NetEvent> BiteEvent::newEvent(){
    return std::make_shared<BiteEvent>();
}

std::shared_ptr<NetEvent> BiteEvent::allocBiteEvent(Vec2 pos, float ang, bool m_isHost){
    //TODO: make a new shared copy of the event and set its _pos to pos.
#pragma mark BEGIN SOLUTION
    auto event = std::make_shared<BiteEvent>();
    event->_pos = pos;
    event->_ang = ang;
    event->_isHost = m_isHost;
    return event;
#pragma mark END SOLUTION
}

/**
 * Serialize any paramater that the event contains to a vector of bytes.
 */
std::vector<std::byte> BiteEvent::serialize(){
    //TODO: serialize _pos
#pragma mark BEGIN SOLUTION
    _serializer.reset();
    _serializer.writeFloat(_pos.x);
    _serializer.writeFloat(_pos.y);
    _serializer.writeFloat(_ang);
    _serializer.writeBool(_isHost);
    return _serializer.serialize();
#pragma mark END SOLUTION
}
/**
 * Deserialize a vector of bytes and set the corresponding parameters.
 *
 * @param data  a byte vector packed by serialize()
 *
 * This function should be the "reverse" of the serialize() function: it
 * should be able to recreate a serialized event entirely, setting all the
 * useful parameters of this class.
 */
void BiteEvent::deserialize(const std::vector<std::byte>& data){
    //TODO: deserialize data and set _pos
    //NOTE: You might be tempted to write Vec2(_deserializer.readFloat(),_deserializer.readFloat()), however, C++ doesn't specify the order in which function arguments are evaluated, so you might end up with <y,x> instead of <x,y>.
    
#pragma mark BEGIN SOLUTION
    _deserializer.reset();
    _deserializer.receive(data);
    float x = _deserializer.readFloat();
    float y = _deserializer.readFloat();
    _pos = Vec2(x,y);
    float ang = _deserializer.readFloat();
    _ang = ang;
    bool host = _deserializer.readBool();
    _isHost = host;
#pragma mark END SOLUTION
}
