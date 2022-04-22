#pragma once

#include <Settings.h>

class AppSettings : public Settings {
public:
	BEGIN_SETTINGS(AppSettings)
		SETTING(MainWindowPlacement, WINDOWPLACEMENT{}, SettingType::Binary);
		SETTING(Font, LOGFONT{}, SettingType::Binary);
		SETTING(AlwaysOnTop, 0, SettingType::Bool);
		SETTING(ViewToolBar, 1, SettingType::Bool);
		SETTING(ViewStatusBar, 1, SettingType::Bool);
		SETTING(TreeIconSize, 0, SettingType::Int32);
		SETTING(DarkMode, 0, SettingType::Bool);
	END_SETTINGS

	DEF_SETTING(AlwaysOnTop, int)
	DEF_SETTING(MainWindowPlacement, WINDOWPLACEMENT)
	DEF_SETTING(Font, LOGFONT)
	DEF_SETTING(TreeIconSize, int)
	DEF_SETTING(DarkMode, int)
	DEF_SETTING(ViewToolBar, int)
	DEF_SETTING(ViewStatusBar, int)
	DEF_SETTING_MULTI(RecentFiles)
};

