#include <Geode/Geode.hpp>
#include <Geode/modify/LevelCell.hpp>

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

        if (!Loader::get()->isModLoaded("cdc.level_thumbnails")) return;

        queueInMainThread([this, difficultyNode, difficultySpr, level] {
            if (!getParent() || !typeinfo_cast<DailyLevelNode*>(getParent())) return;

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

            if (auto bgNode = static_cast<CCScale9Sprite*>(getParent()->getChildByID("background"))) {
                clippingNode->setStencil(CCLayerColor::create(
                    {0, 0, 0, 255},
                    fireSprite->getContentWidth(),
                    fireSprite->getContentHeight() / 2
                        + bgNode->getContentHeight()
                        - difficultyNode->getPositionY()
                        - clippingNode->getPositionY()
                        - 6.0f - (level->m_coins > 0 ? 9.0f : 0.0f)
                ));
            }

            fireSprite->setPosition(clippingNode->getContentSize() / 2);
            clippingNode->setAnchorPoint({0.5, 0.5});
            clippingNode->setID("clipping-node"_spr);
            clippingNode->addChild(fireSprite);
            fireSpriteParent->addChild(clippingNode, -1);
            fireSprite->release();
        });
    }
};
