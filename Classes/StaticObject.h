#ifndef __STATIC_OBJECT_INCLUDED__
#define __STATIC_OBJECT_INCLUDED__

#include "WorldObject.h"

namespace rtm {

    class StaticObject abstract
        : public WorldObject
    {
    public:
        StaticObject();
        StaticObject(float x, float y, float a, cocos2d::Sprite* sprite = nullptr);
        StaticObject(float x, float y, float a, std::string const& filename);

        virtual ~StaticObject() = default;
    };

}

#endif // __STATIC_OBJECT_INCLUDED__