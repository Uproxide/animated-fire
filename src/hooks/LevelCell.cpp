#include "Geode/cocos/cocoa/CCObject.h"
#include "Geode/cocos/misc_nodes/CCClippingNode.h"
#include "Geode/ui/Notification.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/DailyLevelNode.hpp>
#include <fmt/format.h>

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
            // float x, y, z;
            // log::info("x: {}, y: {}, z: {}", x, y, z);

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
                    auto rect = CCLayerColor::create({0, 0, 0, 255}, m_fields->m_fireSpr->getContentWidth(), m_fields->m_fireSpr->getContentHeight() + bgNode->getContentHeight() - (difficultyNode->getPositionY() + difficultySpr->getPositionY() + (clippingNode->getPositionY() - difficultySpr->getContentHeight() / 2) + clippingNode->getContentHeight() / 2 + 11.f));
                    log::info("rect-height: {}", rect->getContentHeight());
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