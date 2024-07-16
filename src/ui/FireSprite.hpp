#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class FireSprite : public CCSprite {
protected:
    int m_currentFrame = 1;
public:
    static FireSprite* create(int fireType);
    bool init(int fireType);
    int fireType;
    void fireAnimation(float dt);
}; // Thanks Cooper! :D