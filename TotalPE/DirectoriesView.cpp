#include "pch.h"
#include "DirectoriesView.h"
#include "PEStrings.h"
#include "SortHelper.h"
#include "ListViewhelper.h"

CString CDirectoriesView::GetColumnText(HWND, int row, int col) const {
	auto& dir = m_Directories[row];
	switch (col) {
		case 0: return dir.Name.c_str();
		case 1: return std::to_wstring(dir.Index).c_str();
		case 2: return std::format(L"0x{:X}", dir.Address).c_str();
		case 3: return std::format(L"0x{:X}", dir.Size).c_str();
		case 4: return dir.Section.c_str();
	}
	return CString();
}

void CDirectoriesView::PreSort(HWND) {
	auto index = m_List.GetSelectedIndex();
	if (index >= 0) {
		m_Selected = m_Directories[index];
	}
}

void CDirectoriesView::PostSort(HWND) {
	auto index = m_List.GetSelectedIndex();
	if (index >= 0) {
		index = ListViewHelper::FindItem(m_List, m_Selected.Name.c_str(), false);
		ATLASSERT(index >= 0);
		m_List.SelectItem(index);
	}
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
			case 4: return SortHelper::Sort(d1.Section, d2.Section, asc);
		}
		return false;
	};
	std::sort(m_Directories.begin(), m_Directories.end(), compare);

}

void CDirectoriesView::OnStateChanged(HWND h, int from, int to, DWORD oldState, DWORD newState) {
	if (newState & LVIS_SELECTED) {
		int index = m_List.GetSelectedIndex();
		m_HexView.ClearData();
		if (index >= 0) {
			auto& image = PE().get_image();
			pe_image_io io(image);
			auto& dir = m_Directories[index];
			io.set_image_offset(dir.Address);
			std::vector<uint8_t> data;
			io.read(data, dir.Size);
			m_HexView.SetData(std::move(data));
		}
	}
}

LRESULT CDirectoriesView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_Splitter.Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_List.Create(m_Splitter, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA | LVS_SINGLESEL | LVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	m_HexView.Create(m_Splitter, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_HexView.SetDynamicAlloc(false);

	auto cm = GetColumnManager(m_List);
	cm->AddColumn(L"Name", LVCFMT_LEFT, 150);
	cm->AddColumn(L"Index", LVCFMT_RIGHT, 70);
	cm->AddColumn(L"Address", LVCFMT_RIGHT, 110);
	cm->AddColumn(L"Size", LVCFMT_RIGHT, 110);
	cm->AddColumn(L"Section", LVCFMT_LEFT, 120);
	cm->UpdateColumns();

	auto& image = PE().get_image();
	for (int i = 0; i < 16; i++) {
		if (!image.has_directory(i))
			continue;

		Item item;
		item.Name = PEStrings::GetDataDirectoryName(i);
		item.Index = i;
		item.Address = image.get_directory_virtual_address(i);
		item.Size = image.get_directory_virtual_size(i);
		auto section = image.get_section_by_rva(item.Address);
		if (section)
			item.Section = section->get_section_name();
		m_Directories.push_back(std::move(item));
	}
	m_List.SetItemCount((int)m_Directories.size());

	m_Splitter.SetSplitterPanes(m_List, m_HexView);
	m_Splitter.SetSplitterPosPct(30);

	return 0;
}
