#include "pch.h"
#include "MenuView.h"

bool CMenuView::SetData(std::vector<uint8_t> const& data) {
	return m_menu.LoadMenuIndirect(data.data());
}

void CMenuView::DoPaint(CDCHandle dc) {
	if (m_menu) {
		auto count = m_menu.GetMenuItemCount();
		MENUITEMINFO mii{ sizeof(mii) };
		for (int i = 0; i < count; i++) {
			m_menu.GetMenuItemInfo(i, TRUE, &mii);
		}
	}
}
