#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        repositionFireSprite();

        return true;
    }

    void levelDownloadFinished(GJGameLevel* level) {
        LevelInfoLayer::levelDownloadFinished(level);

        repositionFireSprite();
    }

    void likedItem(LikeItemType p0, int p1, bool p2) {
        LevelInfoLayer::likedItem(p0, p1, p2);

        repositionFireSprite();
    }

    void repositionFireSprite() {
        if (auto fireSprite = getChildByID("animated-fire-sprite"_spr)) {
            fireSprite->setPositionY(fireSprite->getPositionY() + (m_difficultySprite->m_featureState == GJFeatureState::Epic ? 16.875 : 15.875));
        }
    }
};
