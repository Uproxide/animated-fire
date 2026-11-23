#include <Geode/Geode.hpp>
#include <Geode/modify/GJDifficultySprite.hpp>
#include <random>

using namespace geode::prelude;

class $modify(GJDifficultySprite) {
    void updateDifficultyFrame(int difficulty, GJDifficultyName name) {
        GJDifficultySprite::updateDifficultyFrame(difficulty, name);

        if (auto fireSpr = this->getChildByID("animated-fire-sprite"_spr)) {
            fireSpr->setPositionY(getContentHeight() / 2 + (m_featureState == GJFeatureState::Epic ? 16.875 : 15.875));
        }
    }

    void updateFeatureState(GJFeatureState state) {
        if (m_featureState == state) {
            GJDifficultySprite::updateFeatureState(state);
            return;
        }

        if (auto fireSpr = this->getChildByID("animated-fire-sprite"_spr)) {
            fireSpr->removeFromParent();
            fireSpr = nullptr;
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
        if (epicFire) epicFire->setVisible(false);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 8);
        auto startFrame = dis(gen);
        auto spriteFrameCache = CCSpriteFrameCache::get();
        auto spriteFrames = CCArray::create();
        
        auto i = startFrame;
        while (true) {
            spriteFrames->addObject(spriteFrameCache->spriteFrameByName(fmt::format("{}Frame_{:02}.png"_spr, firePrefix, i).c_str()));
            i = i == 8 ? 1 : i + 1;
            if (i == startFrame) break;
        }

        //log::info("Creating fire sprite with frame: {}", spriteFrameCache->spriteFrameByName(fmt::format("{}Fire.gif"_spr, firePrefix).c_str()));
        //fields->m_fireSpr = CCSprite::create(fmt::format("{}Fire.gif"_spr, firePrefix).c_str()); naomi, once you figure out how to make the gifs HD and SD uncomment this thx :3
        auto fireSpr = CCSprite::createWithSpriteFrame(static_cast<CCSpriteFrame*>(spriteFrames->objectAtIndex(0)));
        fireSpr->setPosition(getContentSize() / 2 + CCPoint { 0.f, state == GJFeatureState::Epic ? 16.875f : 15.875f });
        fireSpr->setScale(epicFire ? epicFire->getScale() : 1.f);
        fireSpr->setID("animated-fire-sprite"_spr);
        fireSpr->runAction(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(spriteFrames, 1/12.f))));
        addChild(fireSpr, -1);

         
    }
};
