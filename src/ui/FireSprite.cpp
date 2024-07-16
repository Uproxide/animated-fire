#include <Geode/Geode.hpp>
#include <random>

#include "./FireSprite.hpp"

FireSprite* FireSprite::create(int p0) {
    auto ret = new FireSprite();
    if (ret && ret->initWithSpriteFrameName("EpicFrame_01.png"_spr) && ret->init(p0)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
};

bool FireSprite::init(int p0) {
    fireType = p0;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 7);
    m_currentFrame = dis(gen);

    this->schedule(schedule_selector(FireSprite::fireAnimation), (1/12.f));

    return true;
} 

void FireSprite::fireAnimation(float dt) {
    std::string type;

    switch (fireType) {
        case 1:
            type = "Epic";
            break;
        case 2:
            type = "Legendary";
            break;
        case 3:
            type = "Mythic";
            break;
        default:
            break;
    }

    if (m_currentFrame == 8) {
        m_currentFrame = 1;
    } else {
        m_currentFrame += 1;
    }

    std::string animationFrame = fmt::format("{}Frame_0{}.png"_spr, type, m_currentFrame);
    Ref<CCSpriteFrame> sprframe = CCSpriteFrameCache::get()->spriteFrameByName(animationFrame.c_str());
    this->setDisplayFrame(sprframe);
}