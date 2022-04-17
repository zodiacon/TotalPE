#include "pch.h"
#include "SectionsView.h"
#include "PEStrings.h"
#include "SortHelper.h"

CString CSectionsView::GetColumnText(HWND, int row, int col) const {
	auto& section = m_Sections[row];
	switch (col) {
		case 0: return section.name().c_str();
		case 1: return PEStrings::ToMemorySize(section.virtual_size()).c_str();
		case 2: return std::format(L"0x{:X}", section.virtual_address()).c_str();
		case 3: return std::format(L"0x{:X}", section.pointerto_raw_data()).c_str();
		case 4: return PEStrings::ToMemorySize(section.sizeof_raw_data()).c_str();
		case 5: return std::to_wstring(section.entropy()).c_str();
		case 6: return std::format(L"0x{:08X} ({})", section.characteristics(), PEStrings::SectionCharacteristicsToString(section.characteristics())).c_str();
	}
	return CString();
}

void CSectionsView::DoSort(SortInfo const* si) {
	if (si == nullptr)
		return;

	auto asc = si->SortAscending;
	auto compare = [&](auto& s1, auto& s2) {
		switch (si->SortColumn) {
			case 0: return SortHelper::Sort(s1.name(), s2.name(), asc);
			case 1: return SortHelper::Sort(s1.virtual_size(), s2.virtual_size(), asc);
			case 2: return SortHelper::Sort(s1.virtual_address(), s2.virtual_address(), asc);
			case 3: return SortHelper::Sort(s1.pointerto_raw_data(), s2.pointerto_raw_data(), asc);
			case 4: return SortHelper::Sort(s1.sizeof_raw_data(), s2.sizeof_raw_data(), asc);
			case 5: return SortHelper::Sort(s1.entropy(), s2.entropy(), asc);
			case 6: return SortHelper::Sort(s1.characteristics(), s2.characteristics(), asc);
		}
		return false;
	};
	m_Sections.Sort(compare);
}

LRESULT CSectionsView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_List.Create(*this, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA, WS_EX_CLIENTEDGE);
	auto cm = GetColumnManager(m_List);

	cm->AddColumn(L"Name", LVCFMT_LEFT, 90);
	cm->AddColumn(L"Size", LVCFMT_RIGHT, 130);
	cm->AddColumn(L"Address", LVCFMT_RIGHT, 110);
	cm->AddColumn(L"Ptr to Raw Data", LVCFMT_RIGHT, 110);
	cm->AddColumn(L"Raw Data Size", LVCFMT_RIGHT, 130);
	cm->AddColumn(L"Entropy", LVCFMT_RIGHT, 100);
	cm->AddColumn(L"Characteristics", LVCFMT_LEFT, 300);
	
	cm->UpdateColumns();

	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	BuildItems();
	
	return 0;
}

void CSectionsView::BuildItems() {
	for (auto& section : PE().sections()) {
		m_Sections.push_back(section);
	}
	m_List.SetItemCount((int)m_Sections.size());
}
