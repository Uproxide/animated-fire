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

        if (fireSprite) {
            if (level->m_isEpic == 1) {
                fireSprite->setPositionY(fireSprite->getPositionY() + 16.875);
            } else {
                fireSprite->setPositionY(fireSprite->getPositionY() + 15.875);
            }
        } else {
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
        }
    }
};
