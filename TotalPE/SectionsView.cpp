#include "pch.h"
#include "SectionsView.h"
#include "PEStrings.h"
#include "SortHelper.h"
#include "resource.h"

CString CSectionsView::GetColumnText(HWND, int row, int col) const {
	auto& section = m_Sections[row];
	switch (col) {
		case 0: return section->get_section_name().c_str();
		case 1: return PEStrings::ToMemorySize(section->get_virtual_size()).c_str();
		case 2: return std::format(L"0x{:X}", section->get_virtual_address()).c_str();
		case 3: return std::format(L"0x{:X}", section->get_pointer_to_raw()).c_str();
		case 4: return PEStrings::ToMemorySize(section->get_size_of_raw_data()).c_str();
		case 5: return std::format(L"0x{:08X} ({})", section->get_characteristics(), 
			PEStrings::SectionCharacteristicsToString(section->get_characteristics())).c_str();
	}
	return CString();
}

void CSectionsView::DoSort(SortInfo const* si) {
	if (si == nullptr)
		return;

	auto asc = si->SortAscending;
	auto compare = [&](auto& s1, auto& s2) {
		switch (si->SortColumn) {
			case 0: return SortHelper::Sort(s1->get_section_name(), s2->get_section_name(), asc);
			case 1: return SortHelper::Sort(s1->get_virtual_size(), s2->get_virtual_size(), asc);
			case 2: return SortHelper::Sort(s1->get_virtual_address(), s2->get_virtual_address(), asc);
			case 3: return SortHelper::Sort(s1->get_pointer_to_raw(), s2->get_pointer_to_raw(), asc);
			case 4: return SortHelper::Sort(s1->get_size_of_raw_data(), s2->get_size_of_raw_data(), asc);
			case 5: return SortHelper::Sort(s1->get_characteristics(), s2->get_characteristics(), asc);
		}
		return false;
	};
	m_Sections.Sort(compare);
}

int CSectionsView::GetRowImage(HWND, int row, int col) const {
	return 0;
}

LRESULT CSectionsView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_List.Create(*this, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA);
	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	CImageList images;
	images.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);
	images.AddIcon(AtlLoadIconImage(IDI_SECTION, 0, 16, 16));
	m_List.SetImageList(images, LVSIL_SMALL);

	auto cm = GetColumnManager(m_List);

	cm->AddColumn(L"Name", LVCFMT_LEFT, 90);
	cm->AddColumn(L"Size", LVCFMT_RIGHT, 130);
	cm->AddColumn(L"Address", LVCFMT_RIGHT, 110);
	cm->AddColumn(L"Ptr to Raw Data", LVCFMT_RIGHT, 110);
	cm->AddColumn(L"Raw Data Size", LVCFMT_RIGHT, 130);
	cm->AddColumn(L"Characteristics", LVCFMT_LEFT, 300);
	cm->UpdateColumns();

	BuildItems();
	
	return 0;
}

void CSectionsView::BuildItems() {
	for (auto& section : PE().get_image().get_sections()) {
		m_Sections.push_back(section);
	}
	m_List.SetItemCount((int)m_Sections.size());
}
