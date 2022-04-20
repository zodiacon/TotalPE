#include "pch.h"
#include "DirectoriesView.h"
#include "PEStrings.h"
#include "SortHelper.h"

CString CDirectoriesView::GetColumnText(HWND, int row, int col) const {
	auto& dir = m_Directories[row];
	switch (col) {
		case 0: return dir.Name.c_str();
		case 1: return std::to_wstring(dir.Index).c_str();
		case 2: return std::format(L"0x{:X}", dir.Address).c_str();
		case 3: return std::format(L"0x{:X}", dir.Size).c_str();
	}
	return CString();
}

void CDirectoriesView::DoSort(SortInfo const* si) {
	if (si == nullptr)
		return;

	auto asc = si->SortAscending;
	auto compare = [&](auto& d1, auto& d2) {
		switch (si->SortColumn) {
			case 0: return SortHelper::Sort(d1.Name, d2.Name, asc);
			case 1: return SortHelper::Sort(d1.Index, d2.Index, asc);
			case 2: return SortHelper::Sort(d1.Address, d2.Address, asc);
			case 3: return SortHelper::Sort(d1.Size, d2.Size, asc);
		}
		return false;
	};
	std::sort(m_Directories.begin(), m_Directories.end(), compare);

}

LRESULT CDirectoriesView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_List.Create(*this, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA, WS_EX_CLIENTEDGE);
	auto cm = GetColumnManager(m_List);

	cm->AddColumn(L"Name", LVCFMT_LEFT, 150);
	cm->AddColumn(L"Index", LVCFMT_RIGHT, 70);
	cm->AddColumn(L"Address", LVCFMT_RIGHT, 110);
	cm->AddColumn(L"Size", LVCFMT_RIGHT, 110);
	cm->UpdateColumns();

	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	auto& image = PE().get_image();
	for (int i = 0; i < 16; i++) {
		if (!image.has_directory(i))
			continue;

		Item item;
		item.Name = PEStrings::GetDataDirectoryName(i);
		item.Index = i;
		item.Address = image.get_directory_virtual_address(i);
		item.Size = image.get_directory_virtual_size(i);
		m_Directories.push_back(std::move(item));
	}
	m_List.SetItemCount((int)m_Directories.size());

	return 0;
}
