#include "pch.h"
#include "RelocationsView.h"
#include "PEStrings.h"
#include "SortHelper.h"

CString CRelocationsView::GetColumnText(HWND, int row, int col) const {
	auto& item = m_Items[row];
	switch (col) {
		case 0: return PEStrings::x64RelocationTypeToString(item.type);
		case 1: return std::format(L"0x{:X}", item.relative_virtual_address).c_str();
		case 2: return std::to_wstring(item.relocation_id).c_str();
		case 3: return std::format(L"0x{:X}", item.data).c_str();
	}
	return CString();
}

void CRelocationsView::DoSort(SortInfo const* si) {
	if (si == nullptr)
		return;

	auto compare = [&](auto& item1, auto& item2) {
		switch (si->SortColumn) {
			case 0: return SortHelper::Sort(item1.type, item2.type, si->SortAscending);
			case 1: return SortHelper::Sort(item1.relative_virtual_address, item2.relative_virtual_address, si->SortAscending);
			case 2: return SortHelper::Sort(item1.relocation_id, item2.relocation_id, si->SortAscending);
			case 3: return SortHelper::Sort(item1.data, item2.data, si->SortAscending);
		}
		return false;
	};
	std::ranges::sort(m_Items, compare);
}

LRESULT CRelocationsView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_List.Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA | LVS_SINGLESEL | LVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	auto cm = GetColumnManager(m_List);
	cm->AddColumn(L"Type", LVCFMT_LEFT, 200);
	cm->AddColumn(L"RVA", LVCFMT_RIGHT, 80);
	cm->AddColumn(L"ID", LVCFMT_RIGHT, 100);
	cm->AddColumn(L"Data", LVCFMT_RIGHT, 140);
	cm->UpdateColumns();

	m_Items = PE().get_relocations().get_entries();
	m_List.SetItemCount((int)m_Items.size());

	return 0;
}

