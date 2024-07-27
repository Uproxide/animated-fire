#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

#include "../ui/FireSprite.hpp"

class $modify(LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        if (level->m_isEpic != 0) {
            auto epicFire = as<CCSprite*>(m_difficultySprite->getChildren()->objectAtIndex(0));
            epicFire->setVisible(false);

            auto fireSpr = FireSprite::create(level->m_isEpic);
            m_difficultySprite->addChild(fireSpr);
            fireSpr->setPosition(epicFire->getPosition());
            if (level->m_isEpic == 1) {
                fireSpr->setPositionY(fireSpr->getPositionY() + 16.875);
            } else {
                fireSpr->setPositionY(fireSpr->getPositionY() + 15.875);
            }
            fireSpr->setZOrder(-1);
            fireSpr->setID("animated-fire-sprite"_spr);
        }

        return true;
    }

    void levelDownloadFinished(GJGameLevel* level) {
        LevelInfoLayer::levelDownloadFinished(level);

        auto fireSprite = m_difficultySprite->getChildByID("animated-fire-sprite"_spr);

        if (!fireSprite) {
            auto state = m_difficultySprite->m_featureState;
            if (state != GJFeatureState::None && state != GJFeatureState::Featured) {
                auto epicFire = as<CCSprite*>(m_difficultySprite->getChildren()->objectAtIndex(0));
                epicFire->setVisible(false);

                auto fireSpr = FireSprite::create((int)state - 1);
                m_difficultySprite->addChild(fireSpr);
                fireSpr->setPosition(epicFire->getPosition());
                if (state == GJFeatureState::Epic) {
                    fireSpr->setPositionY(fireSpr->getPositionY() + 16.875);
                } else {
                    fireSpr->setPositionY(fireSpr->getPositionY() + 15.875);
                }
                fireSpr->setZOrder(-1);
                fireSpr->setID("animated-fire-sprite"_spr);
            }
        }
    }

    void likedItem(LikeItemType p0, int p1, bool p2) {
        LevelInfoLayer::likedItem(p0, p1, p2);

        auto fireSprite = m_difficultySprite->getChildByID("animated-fire-sprite"_spr);

        if (!fireSprite) {
            auto state = m_difficultySprite->m_featureState;
            if (state != GJFeatureState::None && state != GJFeatureState::Featured) {
                auto epicFire = as<CCSprite*>(m_difficultySprite->getChildren()->objectAtIndex(0));
                epicFire->setVisible(false);

                auto fireSpr = FireSprite::create((int)state - 1);
                m_difficultySprite->addChild(fireSpr);
                fireSpr->setPosition(epicFire->getPosition());
                if (state == GJFeatureState::Epic) {
                    fireSpr->setPositionY(fireSpr->getPositionY() + 16.875);
                } else {
                    fireSpr->setPositionY(fireSpr->getPositionY() + 15.875);
                }
                fireSpr->setZOrder(-1);
                fireSpr->setID("animated-fire-sprite"_spr);
            }
        }
    }
};
