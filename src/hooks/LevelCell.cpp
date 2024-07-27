#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/DailyLevelNode.hpp>

using namespace geode::prelude;

#include "../ui/FireSprite.hpp"

class $modify(MyLevelCell, LevelCell) {
    struct Fields {
        SEL_SCHEDULE m_dailyNodeCheck;
        FireSprite* m_fireSpr;
    };

    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);
        m_fields->m_dailyNodeCheck = schedule_selector(MyLevelCell::checkDailyNode);
        schedule(m_fields->m_dailyNodeCheck);
    }

    void checkDailyNode(float dt) {
        if(CCNode* node = getParent()){
            if(typeinfo_cast<DailyLevelNode*>(node) && Loader::get()->isModLoaded("cdc.level_thumbnails")) {
                addWithClippingMask();
            } else {
                add();
            }
            unschedule(m_fields->m_dailyNodeCheck);
        }
    }

    void addWithClippingMask() {
        auto difficultyNode = m_mainLayer->getChildByID("difficulty-container");

		if (difficultyNode) {
			GJDifficultySprite* difficultySpr = as<GJDifficultySprite*>(difficultyNode->getChildByID("difficulty-sprite"));

            auto state = difficultySpr->m_featureState;
            if (state != GJFeatureState::None && state != GJFeatureState::Featured) {
                auto epicFire = as<CCSprite*>(difficultySpr->getChildren()->objectAtIndex(0));
                epicFire->setVisible(false);

                m_fields->m_fireSpr = FireSprite::create((int)state - 1);
                m_fields->m_fireSpr->setID("animated-fire-sprite"_spr);
                m_fields->m_fireSpr->setAnchorPoint({0, 0});

                auto clippingNode = CCClippingNode::create(CCLayerColor::create({0, 0, 0, 255}, m_fields->m_fireSpr->getContentWidth(), m_fields->m_fireSpr->getContentHeight()));
                clippingNode->setContentSize(m_fields->m_fireSpr->getContentSize());
                clippingNode->setPosition(epicFire->getPosition());
                clippingNode->setScale(epicFire->getScale());

                if (state == GJFeatureState::Epic) {
                    clippingNode->setPositionY(clippingNode->getPositionY() + 16.875);
                } else {
                    clippingNode->setPositionY(clippingNode->getPositionY() + 15.875);
                }
                
                if(auto bgNode = as<CCScale9Sprite*>(this->getParent()->getChildByID("background"))) {
                    auto rect = CCLayerColor::create({0, 0, 0, 255}, m_fields->m_fireSpr->getContentWidth(), m_fields->m_fireSpr->getContentHeight() + (bgNode->getContentHeight() - ((difficultyNode->getPositionY() + clippingNode->getPositionY() + clippingNode->getContentHeight() / 2) + 11.f + 2.625f)));
                    clippingNode->setStencil(rect);
                }

                clippingNode->setZOrder(-1);
                clippingNode->setAnchorPoint({0.5, 0.5});
                clippingNode->setID("clipping-node"_spr);
                clippingNode->addChild(m_fields->m_fireSpr, 0);
                difficultySpr->addChild(clippingNode, -1);
            }
        }
    }

    void add() {
        auto difficultyNode = m_mainLayer->getChildByID("difficulty-container");

		if (difficultyNode) {
			GJDifficultySprite* difficultySpr = as<GJDifficultySprite*>(difficultyNode->getChildByID("difficulty-sprite"));

            auto state = difficultySpr->m_featureState;
            if (state != GJFeatureState::None && state != GJFeatureState::Featured) {
                auto epicFire = as<CCSprite*>(difficultySpr->getChildren()->objectAtIndex(0));
                epicFire->setVisible(false);

                m_fields->m_fireSpr = FireSprite::create((int)state - 1);
                difficultySpr->addChild(m_fields->m_fireSpr);
                m_fields->m_fireSpr->setPosition(epicFire->getPosition());
                m_fields->m_fireSpr->setScale(epicFire->getScale());
                if (state == GJFeatureState::Epic) {
                    m_fields->m_fireSpr->setPositionY(m_fields->m_fireSpr->getPositionY() + 16.875);
                } else {
                    m_fields->m_fireSpr->setPositionY(m_fields->m_fireSpr->getPositionY() + 15.875);
                }
                m_fields->m_fireSpr->setZOrder(-1);
                m_fields->m_fireSpr->setID("animated-fire-sprite"_spr);
            }
        }
    }
};

// class $modify(DailyLevelNode) {
//     static void onModify(auto& self) {
//         if (!self.setHookPriority("DailyLevelNode::init", 2000000)) {
//             log::warn("uh oh");
//         }
//     }

//     bool init(GJGameLevel* level, DailyLevelPage* page, bool idk) {
//         if (!DailyLevelNode::init(level, page, idk)) return false;

//         auto remindMeToNameThis = cocos2d::extension::CCScale9Sprite::create("square02b_small.png");
//         remindMeToNameThis->setID("hide-spr"_spr);

//         switch(page->m_type) {
//             case GJTimedLevelType::Daily:
//                 remindMeToNameThis->setColor(ccc3(153, 85, 51));
//                 break;
//             case GJTimedLevelType::Weekly:
//                 remindMeToNameThis->setColor(ccc3(84, 84, 84));
//                 break;
//             // case GJTimedLevelType::Event:
//                 // break;
//             default:
//                 break;
//         }

//         if (Loader::get()->isModLoaded("cdc.level_thumbnails")) {
//             this->addChild(remindMeToNameThis);
//             remindMeToNameThis->setPositionY(remindMeToNameThis->getPositionY() + 78);
//             remindMeToNameThis->setPositionX(this->getPositionX() - 156);
//             remindMeToNameThis->setZOrder(5);
//             remindMeToNameThis->setContentWidth(60);
//         }
        
//         return true;
//     }
// };
