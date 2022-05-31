#include "pch.h"
#include "ExceptionsView.h"
#include "SortHelper.h"

CString CExceptionsView::GetColumnText(HWND, int row, int col) const {
	auto& item = m_Items[row];

	switch (col) {
		case 0: return std::format(L"0x{:08X}", item.get_begin_address()).c_str();
		case 1: return std::format(L"0x{:08X}", item.get_end_address()).c_str();
		case 2: return std::format(L"0x{:08X}", item.get_unwind_data_address()).c_str();
	}
	return CString();
}

void CExceptionsView::DoSort(SortInfo const* si) {
	if (si == nullptr)
		return;

	auto compare = [&](auto& item1, auto& item2) {
		switch (si->SortColumn) {
			case 0: return SortHelper::Sort(item1.get_begin_address(), item2.get_begin_address(), si->SortAscending);
			case 1: return SortHelper::Sort(item1.get_end_address(), item2.get_end_address(), si->SortAscending);
			case 2: return SortHelper::Sort(item1.get_unwind_data_address(), item2.get_unwind_data_address(), si->SortAscending);
		}
		return false;
	};
	std::ranges::sort(m_Items, compare);
}

LRESULT CExceptionsView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_List.Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA | LVS_SINGLESEL | LVS_SHOWSELALWAYS);
	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	auto cm = GetColumnManager(m_List);
	cm->AddColumn(L"Dummy", 0, 10);
	cm->AddColumn(L"Begin Address", LVCFMT_RIGHT, 120);
	cm->AddColumn(L"End Address", LVCFMT_RIGHT, 120);
	cm->AddColumn(L"Unwind Address", LVCFMT_RIGHT, 120);
	cm->UpdateColumns();
	cm->DeleteColumn(0);

	m_Items = PE().get_exceptions().get_exception_entries();
	m_List.SetItemCount((int)m_Items.size());

	return 0;
}
