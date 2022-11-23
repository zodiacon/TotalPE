#include "pch.h"
#include "ResourcesView.h"
#include "PEStrings.h"
#include "SortHelper.h"

LRESULT CResourcesView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_Splitter.Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_List.Create(m_Splitter, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS, 0);
	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	m_HexView.Create(m_Splitter, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_HexView.SetDynamicAlloc(false);
	m_HexView.GetHexControl().SetBytesPerLine(24);

	m_List.SetImageList(Frame()->GetTreeImageList(), LVSIL_SMALL);

	auto cm = GetColumnManager(m_List);
	cm->AddColumn(L"Type", LVCFMT_LEFT, 130);
	cm->AddColumn(L"Name", LVCFMT_LEFT, 200);
	cm->AddColumn(L"Language", LVCFMT_LEFT, 110);
	cm->AddColumn(L"Size", LVCFMT_RIGHT, 110);
	cm->UpdateColumns();

	m_Splitter.SetSplitterPanes(m_List, m_HexView);
	m_Splitter.SetSplitterPosPct(40);

	BuildItems();

	return 0;
}

LRESULT CResourcesView::OnGoToResource(WORD, WORD, HWND, BOOL&) {
	int selected = m_List.GetSelectedIndex();
	ATLASSERT(selected >= 0);
	if (selected) {
		auto& item = m_Items[selected];
		Frame()->GotoTreeItemResource((item.Type + L"\\" + item.Name + L"\\" + item.Language).c_str());
	}
	return 0;
}

CString CResourcesView::GetColumnText(HWND, int row, int col) const {
	auto& item = m_Items[row];
	switch (col) {
		case 0: return item.Type.c_str();
		case 1: return item.Name.c_str();
		case 2: return item.Language.c_str();
		case 3: return PEStrings::ToMemorySize(item.Data.get_data().size()).c_str();
	}
	return CString();
}

int CResourcesView::GetRowImage(HWND, int row, int) const {
	return Frame()->GetResourceIconIndex(m_Items[row].TypeIndex);
}

void CResourcesView::DoSort(SortInfo const* si) {
	if (si == nullptr)
		return;

	auto asc = si->SortAscending;
	auto compare = [&](auto& r1, auto& r2) {
		switch (si->SortColumn) {
			case 0: return SortHelper::Sort(r1.Type, r2.Type, asc);
			case 1: return SortHelper::Sort(r1.Name, r2.Name, asc);
			case 2: return SortHelper::Sort(r1.Language, r2.Language, asc);
			case 3: return SortHelper::Sort(r1.Data.get_data().size(), r2.Data.get_data().size(), asc);
		}
		return false;
	};
	std::sort(m_Items.begin(), m_Items.end(), compare);

}

void CResourcesView::OnStateChanged(HWND h, int from, int to, DWORD oldState, DWORD newState) {
	if (newState & LVIS_SELECTED) {
		int index = m_List.GetSelectedIndex();
		if (index >= 0) {
			m_HexView.ClearData();
			m_HexView.SetData(m_Items[index].Data.get_data());
		}
	}
}

bool CResourcesView::OnRightClickList(HWND, int row, int col, CPoint const& pt) {
	if (row >= 0) {
		CMenu menu;
		menu.LoadMenu(IDR_CONTEXT);
		return Frame()->TrackPopupMenu(menu.GetSubMenu(1), 0, pt.x, pt.y);
	}
	return false;
}

void CResourcesView::BuildItems() {
	auto& res = PE().get_resources();
	m_Items.clear();
	m_Items.reserve(32);
	for (auto& dir : res.get_entry_list()) {
		ParseResourceDir(dir, 0);
	}
	m_List.SetItemCount((int)m_Items.size());
}

void CResourcesView::ParseResourceDir(pe_resource_directory_entry const& dir, int level) {
	std::wstring name;
	switch (level) {
		case 0:		// type of resource
			if (dir.is_named())
				name = dir.get_name();
			else
				name = PEStrings::ResourceTypeToString(dir.get_id());
			if (name.empty())
				name = std::format(L"#{}", dir.get_id());
			m_CurrentResource.TypeIndex = dir.is_named() ? 0xffff : dir.get_id();
			m_CurrentResource.Type = std::move(name);
			break;

		case 1:		// resource ID/name
			if (dir.is_named())
				name = dir.get_name();
			else
				name = std::format(L"#{}", dir.get_id());
			m_CurrentResource.Name = std::move(name);
			break;

		case 2:		// language
			name = PEStrings::LanguageToString(dir.get_id());
			if (name.empty())
				name = std::to_wstring(dir.get_id());
			m_CurrentResource.Language = std::move(name);
			m_CurrentResource.Data = dir.get_data_entry();
			m_Items.push_back(m_CurrentResource);
			break;
	}
	if (!dir.is_includes_data()) {
		for (auto& d : dir.get_resource_directory().get_entry_list()) {
			ParseResourceDir(d, level + 1);
		}
	}
}
