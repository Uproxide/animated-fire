#include <Geode/Geode.hpp>
#include <Geode/modify/GJDifficultySprite.hpp>
#include <random>

using namespace geode::prelude;

class $modify(GJDifficultySprite) {
    struct Fields {
        CCSprite* m_fireSpr;
    };

    void updateDifficultyFrame(int difficulty, GJDifficultyName name) {
        GJDifficultySprite::updateDifficultyFrame(difficulty, name);

        auto fields = m_fields.self();
        if (fields->m_fireSpr) {
            fields->m_fireSpr->setPositionY(getContentHeight() / 2 + (m_featureState == GJFeatureState::Epic ? 16.875 : 15.875));
        }
    }

    void updateFeatureState(GJFeatureState state) {
        if (m_featureState == state) {
            GJDifficultySprite::updateFeatureState(state);
            return;
        }

        auto fields = m_fields.self();
        if (fields->m_fireSpr) {
            fields->m_fireSpr->removeFromParent();
            fields->m_fireSpr = nullptr;
        }

        GJDifficultySprite::updateFeatureState(state);

        if (state == GJFeatureState::None || state == GJFeatureState::Featured) return;

        auto epicFrame = "";
        auto firePrefix = "";
        switch (state) {
            case GJFeatureState::Epic:
                epicFrame = "GJ_epicCoin_001.png";
                firePrefix = "Epic";
                break;
            case GJFeatureState::Legendary:
                epicFrame = "GJ_epicCoin2_001.png";
                firePrefix = "Legendary";
                break;
            case GJFeatureState::Mythic:
                epicFrame = "GJ_epicCoin3_001.png";
                firePrefix = "Mythic";
                break;
            default:
                return;
        }

        auto epicFire = getChildBySpriteFrameName(this, epicFrame);
        if (epicFire) {
            epicFire->setVisible(false);
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 8);
        auto startFrame = dis(gen);

        auto spriteFrames = CCArray::create();
        auto i = startFrame;
        auto spriteFrameCache = CCSpriteFrameCache::get();
        while (true) {
            if (i == 8) {
                i = 1;
            } else {
                i++;
            }
            spriteFrames->addObject(spriteFrameCache->spriteFrameByName(fmt::format("{}Frame_{:02}.png"_spr, firePrefix, i).c_str()));
            if (i == startFrame) break;
        }

        fields->m_fireSpr = CCSprite::createWithSpriteFrame(static_cast<CCSpriteFrame*>(spriteFrames->objectAtIndex(0)));
        fields->m_fireSpr->setPosition(getContentSize() / 2 + CCPoint { 0.f, m_featureState == GJFeatureState::Epic ? 16.875f : 15.875f });
        fields->m_fireSpr->setScale(epicFire ? epicFire->getScale() : 1.f);
        fields->m_fireSpr->setID("animated-fire-sprite"_spr);
        addChild(fields->m_fireSpr, -1);
        fields->m_fireSpr->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(spriteFrames, 1/12.f))));
    }
};
