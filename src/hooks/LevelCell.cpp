#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/DailyLevelNode.hpp>

using namespace geode::prelude;

#include "../ui/FireSprite.hpp"

class $modify(LevelCell) {
    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);

        auto difficultyNode = m_mainLayer->getChildByID("difficulty-container");

		if (difficultyNode) {
			GJDifficultySprite* difficultySpr = as<GJDifficultySprite*>(difficultyNode->getChildByID("difficulty-sprite"));

            if (level->m_isEpic != 0) {
                auto epicFire = as<CCSprite*>(difficultySpr->getChildren()->objectAtIndex(0));
                epicFire->setVisible(false);

                auto fireSpr = FireSprite::create(level->m_isEpic);
                difficultySpr->addChild(fireSpr);
                fireSpr->setPosition(epicFire->getPosition());
                fireSpr->setScale(epicFire->getScale());
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

class $modify(DailyLevelNode) {
    static void onModify(auto& self) {
        if (!self.setHookPriority("DailyLevelNode::init", 2000000)) {
            log::warn("uh oh");
        }
    }

    bool init(GJGameLevel* level, DailyLevelPage* page, bool idk) {
        if (!DailyLevelNode::init(level, page, idk)) return false;

        auto remindMeToNameThis = cocos2d::extension::CCScale9Sprite::create("square02b_small.png");
        remindMeToNameThis->setID("hide-spr"_spr);

        switch(page->m_type) {
            case GJTimedLevelType::Daily:
                remindMeToNameThis->setColor(ccc3(153, 85, 51));
                break;
            case GJTimedLevelType::Weekly:
                remindMeToNameThis->setColor(ccc3(84, 84, 84));
                break;
            // case GJTimedLevelType::Event:
                // break;
            default:
                break;
        }

        if (Loader::get()->isModLoaded("cdc.level_thumbnails")) {
            this->addChild(remindMeToNameThis);
            remindMeToNameThis->setPositionY(remindMeToNameThis->getPositionY() + 78);
            remindMeToNameThis->setPositionX(this->getPositionX() - 156);
            remindMeToNameThis->setZOrder(5);
            remindMeToNameThis->setContentWidth(60);
        }
        
        return true;
    }
};
