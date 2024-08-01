#include <Geode/Geode.hpp>
#include <Geode/modify/GJDifficultySprite.hpp>

#include "../ui/FireSprite.hpp"

using namespace geode::prelude;

class $modify(GJDifficultySprite) {
    void updateDifficultyFrame(int difficulty, GJDifficultyName name) {
        GJDifficultySprite::updateDifficultyFrame(difficulty, name);

        auto fireSprite = as<FireSprite*>(getChildByID("animated-fire-sprite"_spr));

        if (fireSprite) {
            auto y = getContentSize().height / 2;
            if (fireSprite->fireType == 1) {
                fireSprite->setPositionY(y + 16.875);
            } else {
                fireSprite->setPositionY(y + 15.875);
            }
        }
    }

    void updateFeatureState(GJFeatureState state) {
        auto fireSprite = as<FireSprite*>(getChildByID("animated-fire-sprite"_spr));

        if (fireSprite && state != GJFeatureState::None && state != GJFeatureState::Featured && (int)state - 1 != fireSprite->fireType) {
            fireSprite->retain();

            GJDifficultySprite::updateFeatureState(state);

            auto oldType = fireSprite->fireType;
            fireSprite->init((int)state - 1);
            addChild(fireSprite, -1);
            as<CCSprite*>(getChildren()->objectAtIndex(0))->setVisible(false);

            auto y = getContentSize().height / 2;
            if (state == GJFeatureState::Epic && oldType != 1) {
                fireSprite->setPositionY(y + 16.875);
            } else if (state != GJFeatureState::Epic && oldType == 1) {
                fireSprite->setPositionY(y + 15.875);
            }

            fireSprite->release();
        } else GJDifficultySprite::updateFeatureState(state);
    }
};
