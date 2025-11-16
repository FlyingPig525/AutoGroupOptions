#include <Geode/Geode.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

#define OBJ_POS CCPoint(0, -500)
#define OBJ_TYPE CameraTriggerGameObject
#define OBJ_ID 2016

#include <Geode/modify/LevelSettingsLayer.hpp>
struct AutoOptionsButton : Modify<AutoOptionsButton, LevelSettingsLayer> {
    bool init(LevelSettingsObject* settings, LevelEditorLayer* layer) {
        if (!LevelSettingsLayer::init(settings, layer)) return false;

        auto sprite = ButtonSprite::create("Auto Object Options");
        sprite->setScale(0.65f);
        auto button = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(AutoOptionsButton::onAutoOptions));
        button->setPosition(CCPoint(-126.0f, -130.0f));
        m_buttonMenu->addChild(button);
        return true;
    }

    void onAutoOptions(CCObject *sender) {
        log::debug("Auto Object Options button pressed");
        GameObject *optObject = getAndCreateOptionsObject();
        if (optObject == nullptr) return;
        optObject->m_isHide = false;
        auto popup = SetupObjectOptionsPopup::create(optObject, nullptr, nullptr);
        if (popup == nullptr) return;
        popup->show();
    }

    static OBJ_TYPE *getOptionsObject() {
        auto editor = LevelEditorLayer::get();
        if (editor == nullptr) return nullptr;
        auto objs = editor->objectsAtPosition(OBJ_POS);
        if (objs == nullptr) {
            return nullptr;
        }
        OBJ_TYPE *optObject = nullptr;
        if (objs->count() < 1) {
            return nullptr;
        } else {
            optObject = typeinfo_cast<OBJ_TYPE*>(objs->objectAtIndex(0));
            if (optObject == nullptr) {
                log::error("Could not get options object from CCArray");
                return nullptr;
            }
        }
        return optObject;
    }

    static OBJ_TYPE *getAndCreateOptionsObject() {
        auto editor = LevelEditorLayer::get();
        if (editor == nullptr) return nullptr;
        auto opt = getOptionsObject();
        if (opt == nullptr) {
            log::info("Options object not found in level, creating");
            auto obj = editor->createObject(OBJ_ID, OBJ_POS, false);
            if (obj == nullptr) return nullptr;
            return typeinfo_cast<OBJ_TYPE*>(obj);
        }
        return opt;
    }
};

#include <Geode/modify/SetupObjectOptionsPopup.hpp>
struct ObjectOptionsPopupSetHide : Modify<ObjectOptionsPopupSetHide, SetupObjectOptionsPopup> {
    void onClose(cocos2d::CCObject* sender) override {
        SetupObjectOptionsPopup::onClose(sender);
        if (m_gameObject != nullptr && m_gameObject->getPosition().equals(OBJ_POS)) {
            if (auto camera = typeinfo_cast<OBJ_TYPE *>(m_gameObject); camera != nullptr) {
                log::debug("Popup closed was for settings object, hiding");
                m_gameObject->m_isHide = true;
            }
        }
    };
};

#include <Geode/modify/GameObject.hpp>
struct GameObjectConstructor : Modify<GameObjectConstructor, GameObject> {
    void customSetup() override {
        GameObject::customSetup();
        auto opt = AutoOptionsButton::getOptionsObject();
        if (opt == nullptr) return;
        m_isDontFade           = opt->m_isDontFade;
        m_isDontEnter          = opt->m_isDontEnter;
        m_hasNoEffects         = opt->m_hasNoEffects;
        m_hasGroupParent       = opt->m_hasGroupParent;
        m_hasAreaParent        = opt->m_hasAreaParent;
        m_isDontBoostY         = opt->m_isDontBoostY;
        m_isDontBoostX         = opt->m_isDontBoostX;
        m_isHighDetail         = opt->m_isHighDetail;
        m_isNoTouch            = opt->m_isNoTouch;
        m_isPassable           = opt->m_isPassable;
        m_isNonStickX          = opt->m_isNonStickX;
        m_isNonStickY          = opt->m_isNonStickY;
        m_isExtraSticky        = opt->m_isExtraSticky;
        m_hasExtendedCollision = opt->m_hasExtendedCollision;
        m_isIceBlock           = opt->m_isIceBlock;
        m_isGripSlope          = opt->m_isGripSlope;
        m_hasNoGlow            = opt->m_hasNoGlow;
        m_hasNoParticles       = opt->m_hasNoParticles;
        m_isScaleStick         = opt->m_isScaleStick;
        m_hasNoAudioScale      = opt->m_hasNoAudioScale;
    }
};