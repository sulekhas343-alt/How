#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/binding/UILayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

namespace dc {
    struct Profile {
        std::string hardest;
        std::string favoriteMode = "Any";
        std::string style = "Balanced";
        std::vector<std::string> skills;
        int dailyEasy = 1;
        int dailyMedium = 1;
        int dailyHard = 1;
        int weeklyInsane = 1;
        int weeklyExtreme = 1;
        int monthlyList = 1;
        int minLikes = 4000;
    };

    static Profile profile;

    static void saveProfile() {
        auto& p = Mod::get()->getSavedValue<matjson::Value>("profile");
        p = matjson::Value({
            {"hardest", profile.hardest},
            {"favoriteMode", profile.favoriteMode},
            {"style", profile.style},
            {"skills", profile.skills},
            {"dailyEasy", profile.dailyEasy},
            {"dailyMedium", profile.dailyMedium},
            {"dailyHard", profile.dailyHard},
            {"weeklyInsane", profile.weeklyInsane},
            {"weeklyExtreme", profile.weeklyExtreme},
            {"monthlyList", profile.monthlyList},
            {"minLikes", profile.minLikes}
        });
    }

    static void loadProfile() {
        auto p = Mod::get()->getSavedValue<matjson::Value>("profile", matjson::Value());
        if (!p.isObject()) return;
        auto getS = [&](const char* k, std::string d) {
            auto v = p[k];
            return v.isString() ? v.asString().unwrapOr(d) : d;
        };
        profile.hardest = getS("hardest", "");
        profile.favoriteMode = getS("favoriteMode", "Any");
        profile.style = getS("style", "Balanced");
        if (p["skills"].isArray()) {
            profile.skills.clear();
            for (auto const& v : p["skills"]) if (v.isString()) profile.skills.push_back(v.asString().unwrapOr(""));
        }
        auto geti = [&](const char* k, int d) {
            auto v = p[k];
            return v.isNumber() ? static_cast<int>(v.asInt().unwrapOr(d)) : d;
        };
        profile.dailyEasy = geti("dailyEasy", 1);
        profile.dailyMedium = geti("dailyMedium", 1);
        profile.dailyHard = geti("dailyHard", 1);
        profile.weeklyInsane = geti("weeklyInsane", 1);
        profile.weeklyExtreme = geti("weeklyExtreme", 1);
        profile.monthlyList = geti("monthlyList", 1);
        profile.minLikes = geti("minLikes", 4000);
    }

    static void openTextPopup(std::string title, std::string text) {
        geode::createQuickPopup(
            title.c_str(), text.c_str(), "Close", nullptr
        );
    }

    static void showFirstRun() {
        auto p = CCNode::create();
        auto popup = geode::Popup::create(360.f, 260.f);
        popup->setTitle("Demon Coach — Setup");
        auto label = CCLabelBMFont::create(
            "Welcome! Open Mod Settings to configure your hardest,\n"
            "favorite gamemode, preferred level style, skills, and\n"
            "daily/weekly/monthly suggestion frequency.",
            "goldFont.fnt"
        );
        label->setScale(.42f);
        label->setPosition({180.f, 125.f});
        popup->addChild(label);
        popup->addButton("Open Settings", [] {
            geode::openSettingsPopup(Mod::get());
        });
        popup->addButton("Later", nullptr);
        popup->show();
    }

    static void fetchPointercratePreview() {
        web::WebRequest req;
        req.header("Accept", "application/json");
        req.get("https://pointercrate.com/api/v2/demons/listed/").listen(
            [](web::WebResponse res) {
                if (!res.ok()) return;
                log::info("Demon Coach: Pointercrate data reachable.");
            }
        );
    }

    static void fetchAredlPreview() {
        web::WebRequest req;
        req.header("Accept", "application/json");
        req.get("https://api.aredl.net/v2/").listen(
            [](web::WebResponse res) {
                if (!res.ok()) return;
                log::info("Demon Coach: AREDL API reachable.");
            }
        );
    }
}

class $modify(DemonCoachMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        dc::loadProfile();

        auto spr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        if (!spr) return true;
        spr->setScale(.65f);

        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(DemonCoachMenuLayer::onCoach)
        );
        auto menu = CCMenu::create();
        menu->addChild(btn);
        menu->setPosition({40.f, 40.f});
        this->addChild(menu, 10);

        if (dc::profile.hardest.empty())
            dc::showFirstRun();

        return true;
    }

    void onCoach(CCObject*) {
        dc::fetchPointercratePreview();
        dc::fetchAredlPreview();

        geode::createQuickPopup(
            "Demon Coach",
            "Your recommendation dashboard is ready in Mod Settings.\n\n"
            "Daily: Easy / Medium / Hard\n"
            "Weekly: Insane / Extreme\n"
            "Monthly: Pointercrate list demons\n\n"
            "Recommendations use your profile and configurable filters.",
            "Close", nullptr
        );
    }
};
