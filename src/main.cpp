#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

		cocos2d::CCSprite* userIcon = cocos2d::CCSprite::create("gdzoltex.nothing/button-image.png");

		cocos2d::CCSprite* frame = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
		if (!frame) frame = CCSprite::create("GJ_likeBtn_001.png");
		if (!frame) frame = CCSprite::createWithSpriteFrameName("GJ_button_01.png");
		if (!frame) frame = CCSprite::create("GJ_button_01.png");
		if (!frame) {
			auto menuNode = this->getChildByID("bottom-menu");
			if (menuNode) {
				std::function<cocos2d::CCSprite*(cocos2d::CCNode*)> findSprite;
				findSprite = [&](cocos2d::CCNode* node) -> cocos2d::CCSprite* {
					if (!node) return nullptr;
					auto spr = dynamic_cast<cocos2d::CCSprite*>(node);
					if (spr && spr->getTexture()) {
						float w = spr->getContentSize().width;
						float h = spr->getContentSize().height;
						if (w > 16.0f && h > 16.0f && fabs(w - h) < 8.0f) return spr;
					}
					auto children = node->getChildren();
					if (!children) return nullptr;
					for (int i = 0; i < (int)children->count(); ++i) {
						auto c = static_cast<cocos2d::CCNode*>(children->objectAtIndex(i));
						auto res = findSprite(c);
						if (res) return res;
					}
					return nullptr;
				};
				auto found = findSprite(menuNode);
				if (found) frame = CCSprite::createWithTexture(found->getTexture());
			}
		}
		if (frame) {
			frame->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.5f));
			frame->setVisible(true);
		}

		cocos2d::CCNode* finalSprite = nullptr;
		if (frame) {
			finalSprite = frame;
			if (userIcon) {
				userIcon->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.5f));
				frame->setScale(1.0f);
				float frameH = frame->getContentSize().height;
				float iconH = userIcon->getContentSize().height;
				float desiredRatio = 0.84f;
				float iconScale = 1.0f;
				if (iconH > 0.0f) iconScale = (frameH * desiredRatio) / iconH;
				if (iconScale > 2.0f) iconScale = 2.0f;
				if (iconScale < 0.08f) iconScale = 0.08f;
				userIcon->setScale(iconScale);
				userIcon->setPosition(cocos2d::CCPoint(frame->getContentSize().width * 0.5f, frame->getContentSize().height * 0.5f));
				frame->addChild(userIcon);
			}
		} else if (userIcon) {
			userIcon->setScale(0.08f);
			finalSprite = userIcon;
		}

		auto myButton = CCMenuItemSpriteExtra::create(
			dynamic_cast<cocos2d::CCSprite*>(finalSprite),
			this,
			menu_selector(MyMenuLayer::onMyButton)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);
		myButton->setID("my-button"_spr);
		menu->updateLayout();

		return true;
	}

	void onMyButton(CCObject*) {
		auto alert = FLAlertLayer::create(nullptr, "Nothing Mod v1", "\nwhy do you even click here?\n\n", "my bad", nullptr, 500.0f, false, 450.0f, 1.25f);
		if (!alert) return;

		auto bGit = ButtonSprite::create("GitHub");
		auto bKofi = ButtonSprite::create("Ko-Fi");
		auto bDisc = ButtonSprite::create("Discord");

		auto miGit = CCMenuItemSpriteExtra::create(bGit, this, menu_selector(MyMenuLayer::onLinkGitHub));
		auto miKofi = CCMenuItemSpriteExtra::create(bKofi, this, menu_selector(MyMenuLayer::onLinkKofi));
		auto miDisc = CCMenuItemSpriteExtra::create(bDisc, this, menu_selector(MyMenuLayer::onLinkDiscord));

		if (alert->m_buttonMenu) {
			alert->m_buttonMenu->addChild(miGit);
			alert->m_buttonMenu->addChild(miKofi);
			alert->m_buttonMenu->addChild(miDisc);
			alert->m_buttonMenu->alignItemsHorizontallyWithPadding(8.0f);
			cocos2d::CCSize alertSize = alert->getContentSize();
			alert->m_buttonMenu->setPosition(cocos2d::CCPoint(alertSize.width * 0.5f, alert->m_buttonMenu->getPositionY() + 6.0f));
		}

		alert->show();
	}

	void onLinkGitHub(CCObject*) { cocos2d::CCApplication::sharedApplication()->openURL("https://github.com/zoltex999"); }
	void onLinkKofi(CCObject*) { cocos2d::CCApplication::sharedApplication()->openURL("https://ko-fi.com/zoltex"); }
	void onLinkDiscord(CCObject*) { cocos2d::CCApplication::sharedApplication()->openURL("https://discord.gg/D9TCNr3VzE"); }
};
