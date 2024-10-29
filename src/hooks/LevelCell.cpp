#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>
// #include <Geode/modify/DailyLevelNode.hpp>

using namespace geode::prelude;

class $modify(MyLevelCell, LevelCell) {
    void loadFromLevel(GJGameLevel* level) {
        LevelCell::loadFromLevel(level);

        auto difficultyNode = m_mainLayer->getChildByID("difficulty-container");
        if (!difficultyNode) {
            difficultyNode = m_mainLayer->getChildByID("grd-demon-icon-layer");
            if (!difficultyNode) return;
        }

        auto difficultySpr = static_cast<GJDifficultySprite*>(difficultyNode->getChildByID("difficulty-sprite"));
        if (!difficultySpr) return;

        if (auto fireSprite = difficultyNode->getChildByID("animated-fire-sprite"_spr)) {
            fireSprite->setPositionY(fireSprite->getPositionY() + (difficultySpr->m_featureState == GJFeatureState::Epic ? 16.875 : 15.875));
        }

        queueInMainThread([this, difficultyNode, difficultySpr] {
            if (!getParent() || !typeinfo_cast<DailyLevelNode*>(getParent()) || !Loader::get()->isModLoaded("cdc.level_thumbnails")) return;

            auto fireSprite = difficultyNode->getChildByID("animated-fire-sprite"_spr);
            if (!fireSprite) {
                fireSprite = difficultySpr->getChildByID("animated-fire-sprite"_spr);
                if (!fireSprite) return;
            }

            auto fireSpriteParent = fireSprite->getParent();
            fireSprite->retain();
            fireSprite->removeFromParentAndCleanup(false);

            auto clippingNode = CCClippingNode::create(CCLayerColor::create({0, 0, 0, 255}, fireSprite->getContentWidth(), fireSprite->getContentHeight()));
            clippingNode->setContentSize(fireSprite->getContentSize());
            clippingNode->setPosition(fireSprite->getPosition());
            clippingNode->setScale(fireSprite->getScale());

            if (auto bgNode = as<CCScale9Sprite*>(this->getParent()->getChildByID("background"))) {
                auto rect = CCLayerColor::create({0, 0, 0, 255}, fireSprite->getContentWidth(), fireSprite->getContentHeight() + (bgNode->getContentHeight() - ((difficultyNode->getPositionY() + clippingNode->getPositionY() + clippingNode->getContentHeight() / 2) + 6.625f)));
                clippingNode->setStencil(rect);
            }

            fireSprite->setPosition(fireSprite->getContentSize() / 2);
            clippingNode->setAnchorPoint({0.5, 0.5});
            clippingNode->setID("clipping-node"_spr);
            clippingNode->addChild(fireSprite);
            fireSpriteParent->addChild(clippingNode, -1);
            fireSprite->release();
        });
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
