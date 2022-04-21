#include "pch.h"
#include "DebugView.h"
#include "PEStrings.h"
#include "SortHelper.h"

CString CDebugView::GetColumnText(HWND, int row, int col) const {
	auto& item = m_Items[row];
	switch (col) {
		case 0: return PEStrings::DebugTypeToString(item.get_type());
		case 1: return std::format(L"0x{:08X}", item.get_timestamp()).c_str();
		case 2: return std::format(L"{}.{}", item.get_major_version(), item.get_minor_version()).c_str();
		case 3: return std::format(L"0x{:X}", item.get_address_of_raw_data()).c_str();
		case 4: return PEStrings::ToMemorySize(item.get_size_of_data()).c_str();
		case 5: return std::format(L"0x{:X}", item.get_pointer_to_raw_data()).c_str();
	}
	return CString();
}

void CDebugView::DoSort(SortInfo const* si) {
}

void CDebugView::OnStateChanged(HWND h, int from, int to, DWORD oldState, DWORD newState) {
	if (newState & LVIS_SELECTED) {
		int index = m_List.GetSelectedIndex();
		ATLASSERT(index == from);
		if (index >= 0) {
			auto& item = m_Items[index];
			m_HexView.SetData(item.get_item_data());
		}
		else {
			m_HexView.ClearData();
		}
	}
}

LRESULT CDebugView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_Splitter.Create(m_hWnd, rcDefault, nullptr, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	m_List.Create(m_hWndClient, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA | LVS_SINGLESEL | LVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
	m_HexView.Create(m_Splitter, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	m_HexView.SetDynamicAlloc(false);

	auto cm = GetColumnManager(m_List);
	cm->AddColumn(L"Type", LVCFMT_LEFT, 170);
	cm->AddColumn(L"Time Stamp", LVCFMT_RIGHT, 130);
	cm->AddColumn(L"Version", LVCFMT_RIGHT, 110);
	cm->AddColumn(L"Address", LVCFMT_RIGHT, 110);
	cm->AddColumn(L"Size", LVCFMT_RIGHT, 130);
	cm->AddColumn(L"Pointer to Raw Data", LVCFMT_RIGHT, 130);
	cm->UpdateColumns();

	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	m_Splitter.SetSplitterPosPct(30);
	m_Splitter.SetSplitterPanes(m_List, m_HexView);

	BuildItems();
	return 0;
}

void CDebugView::BuildItems() {
	m_Items = PE().get_debug().get_entries();
	m_List.SetItemCount((int)m_Items.size());
}
