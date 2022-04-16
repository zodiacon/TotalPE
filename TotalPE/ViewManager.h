#pragma once

#include "Interfaces.h"

class ViewManager {
public:
	ViewManager(IMainFrame* frame);
	HWND CreateOrGetView(TreeItemType type, HWND hParent, LIEF::PE::Binary const& pe);
	void Clear();

private:
	std::unordered_map<TreeItemType, HWND> m_views;
	IMainFrame* m_pFrame;
};

