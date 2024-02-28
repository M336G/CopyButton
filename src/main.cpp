#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(CopyButtonLayer, LevelInfoLayer) {
	// I define songID here so that it can be used in onCopyButton
	int songID;

	bool init(GJGameLevel* level, bool p1) {
		if (!LevelInfoLayer::init(level, false))
			return false;
		try {
			// I define everything here
			auto customSongWidget = this->getChildByID("custom-songs-widget");
			CCMenu* customSongWidgetMenu = reinterpret_cast<CCMenu*>(customSongWidget->getChildren()->objectAtIndex(1));
			CCMenuItemSpriteExtra* moreButtonSprite = reinterpret_cast<CCMenuItemSpriteExtra*>(customSongWidgetMenu->getChildren()->objectAtIndex(6));
			ButtonSprite* moreButton = reinterpret_cast<ButtonSprite*>(customSongWidgetMenu->getChildren()->objectAtIndex(0));
			
			// And the song ID previously defined
			songID = level->m_songID;

			// I create the button
			ButtonSprite* myCopyButton = ButtonSprite::create("Copy", "bigFont.fnt", "GJ_button_04.png", moreButton->getScale());
			myCopyButton->setScaleX(moreButtonSprite->getScaleX());
			myCopyButton->setScaleY(moreButtonSprite->getScaleY());
			myCopyButton->setScale(0.5);
			auto copyButton = CCMenuItemSpriteExtra::create(
				myCopyButton,
				this,
				menu_selector(CopyButtonLayer::onCopyButton)
			);

			// Here I set the button's position, depending on the More button position
			copyButton->setPositionX(moreButtonSprite->getPositionX() + 55);
			copyButton->setPositionY(moreButtonSprite->getPositionY());

			// And I add it to the custom song widget menu
			customSongWidgetMenu->addChild(copyButton);
			// I also set its ID here
			copyButton->setID("copy-button"_spr);
			// And I also update the layout
			customSongWidget->updateLayout();
			geode::log::debug("Successfully displayed the copy button!");
		}
		catch (...) {
			// If there is an error when the mod tries to add the button, it throws an error in-game and in the logs
			Notification::create("Failed to display the copy button", NotificationIcon::Error, 1)->show();
			geode::log::error("Failed to display the copy button");
		}

		return true;
	}

	void onCopyButton(CCObject*) {
		try {
			// The mod writes the song ID to the clipboard and show a success message in-game and in the logs
			std::string clipboardSongID = fmt::format("{}", songID).c_str();
			clipboard::write(clipboardSongID);
			Notification::create("Successfully copied the song ID to the clipboard!", NotificationIcon::Success, 1)->show();
			geode::log::debug("Successfully copied the song ID to the clipboard!");
		}
		catch (...) {
			// If there is an error when the mod tries to write into the clipboard, it throws an error in-game and in the logs
			Notification::create("Failed to copy the song ID to the clipboard", NotificationIcon::Error, 1)->show();
			geode::log::error("Failed to copy the song ID to the clipboard");
		}
	}
};