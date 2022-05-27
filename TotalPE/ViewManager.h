#pragma once

#include "Interfaces.h"

class ViewManager {
public:
	ViewManager(IMainFrame* frame);
	HWND CreateOrGetView(TreeItemType type, HWND hParent, pe_image_full const& pe);
	void Clear();

	static HWND CreateAssemblyView(pe_image_full const& pe, PCWSTR title, std::vector<uint8_t> const& code, uint64_t address = 0);

private:
	std::unordered_map<TreeItemType, HWND> m_views;
	IMainFrame* m_pFrame;
};

