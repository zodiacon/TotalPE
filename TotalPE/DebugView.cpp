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
		case 6: return GetDetails(row).c_str();
	}
	return CString();
}

void CDebugView::DoSort(SortInfo const* si) {
	if (si == nullptr)
		return;

	auto asc = si->SortAscending;
	auto compare = [&](auto& d1, auto& d2) {
		switch (si->SortColumn) {
			case 0: return SortHelper::Sort(PEStrings::DebugTypeToString(d1.get_type()), PEStrings::DebugTypeToString(d2.get_type()), asc);
			case 1: return SortHelper::Sort(d1.get_timestamp(), d2.get_timestamp(), asc);
			case 2: return SortHelper::Sort((d1.get_major_version() << 16) | d1.get_minor_version(), (d2.get_major_version() << 16) | d2.get_minor_version(), asc);
			case 3: return SortHelper::Sort(d1.get_address_of_raw_data(), d2.get_address_of_raw_data(), asc);
			case 4: return SortHelper::Sort(d1.get_size_of_data(), d2.get_size_of_data(), asc);
			case 5: return SortHelper::Sort(d1.get_pointer_to_raw_data(), d2.get_pointer_to_raw_data(), asc);
		}
		return false;
	};
	std::sort(m_Items.begin(), m_Items.end(), compare);
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

std::wstring CDebugView::GetDetails(int row) const {
	struct CodeView {
		char format[4];
		GUID guid;
		ULONG count;
		char pdb[64];
	};
	auto& item = m_Items[row];
	switch (item.get_type()) {
		case IMAGE_DEBUG_TYPE_CODEVIEW:	
			auto data = (CodeView*)item.get_item_data().data();
			return std::format(L"Format: {}{}{}{} GUID: {} Pdb: {}",
				data->format[0], data->format[1], data->format[2], data->format[3],
				PEStrings::GuidToString(data->guid), (PCWSTR)CString(data->pdb));
	}
	return std::wstring();
}

LRESULT CDebugView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_Splitter.Create(m_hWnd, rcDefault, nullptr, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	m_List.Create(m_hWndClient, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA | LVS_SINGLESEL | LVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
	m_HexView.Create(m_Splitter, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
	m_HexView.SetDynamicAlloc(false);

	auto cm = GetColumnManager(m_List);
	cm->AddColumn(L"Type", LVCFMT_LEFT, 170);
	cm->AddColumn(L"Time Stamp", LVCFMT_RIGHT, 130);
	cm->AddColumn(L"Version", LVCFMT_RIGHT, 110);
	cm->AddColumn(L"Address", LVCFMT_RIGHT, 110);
	cm->AddColumn(L"Size", LVCFMT_RIGHT, 130);
	cm->AddColumn(L"Pointer to Raw Data", LVCFMT_RIGHT, 130);
	cm->AddColumn(L"Details", LVCFMT_LEFT, 300);
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
