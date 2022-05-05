#include "pch.h"
#include "PEImageView.h"
#include "PEStrings.h"
#include "SortHelper.h"

CString CPEImageView::GetColumnText(HWND, int row, int col) const {
	auto& item = m_Items[row];
	switch (col) {
		case 0: return item.Name.c_str();
		case 1: return item.Value.c_str();
		case 2: return item.Details.c_str();
	}
	return CString();
}

void CPEImageView::DoSort(SortInfo const* si) {
	if (si == nullptr)
		return;

	auto compare = [&](auto& item1, auto& item2) {
		return SortHelper::Sort(item1.Name, item2.Name, si->SortAscending);
	};

	m_Items.Sort(compare);
}

bool CPEImageView::IsSortable(HWND, int col) const {
	return col == 0;		// sort on Name column only
}

LRESULT CPEImageView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	m_hWndClient = m_List.Create(*this, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA, WS_EX_CLIENTEDGE);
	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	auto cm = GetColumnManager(m_List);

	cm->AddColumn(L"Name", LVCFMT_LEFT, 150);
	cm->AddColumn(L"Value", LVCFMT_LEFT, 200);
	cm->AddColumn(L"Details", LVCFMT_LEFT, 450);

	cm->UpdateColumns();


	BuildItems();

	return 0;
}

void CPEImageView::BuildItems() {
	auto& header = PE().get_image();
	auto& path = Frame()->GetPEPath();

	m_Items = std::vector<DataItem>{
		{ L"File Name", (PCWSTR)path.Mid(path.ReverseFind(L'\\') + 1), (PCWSTR)path },
		{ L"Time/Date Stamp", std::format(L"0x{:08X}", header.get_timestamp()) },
		{ L"Machine", std::format(L"{} (0x{:X})", header.get_machine(), header.get_machine()), PEStrings::MachineTypeToString(header.get_machine()) },
		{ L"Subsystem", std::to_wstring(header.get_sub_system()), PEStrings::SubsystemToString(header.get_sub_system()) },
		{ L"Sections", std::to_wstring(header.get_sections_number()) },
		{ L"Characteristics", std::format(L"0x{:08X}", header.get_characteristics()), PEStrings::CharacteristicsToString(header.get_characteristics()) },
		{ L"Magic", std::format(L"{} (0x{:X})", header.get_magic(), header.get_magic()), PEStrings::MagicToString(header.get_magic()) },
		{ L"DLL Characteristics", std::format(L"0x{:04X}", header.get_characteristics_dll()), PEStrings::DllCharacteristicsToString(header.get_characteristics_dll()) },
		{ L"Image Base", std::format(L"0x{:X}", header.get_image_base()) },
		{ L"Image Size", PEStrings::ToMemorySize(header.get_image_size()) },
		{ L"Stack Reserve", PEStrings::ToMemorySize(header.get_stack_reserve_size()) },
		{ L"Stack Commit", PEStrings::ToMemorySize(header.get_stack_commit_size()) },
		{ L"Heap Reserve", PEStrings::ToMemorySize(header.get_heap_reserve_size()) },
		{ L"Heap Commit", PEStrings::ToMemorySize(header.get_heap_commit_size()) },
		{ L"Is Managed?", header.has_directory(IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR) ? L"Yes" : L"No" },
		{ L"Code Size", PEStrings::ToMemorySize(header.get_size_of_code()) },
		{ L"Entry Point", std::format(L"0x{:X}", header.get_entry_point()) },
		{ L"OS Version", std::format(L"{}.{}", header.get_os_ver_major(), header.get_os_ver_minor()) },
		{ L"Image Version", std::format(L"{}.{}", header.get_image_ver_major(), header.get_image_ver_minor()) },
		{ L"Linker Version", std::format(L"{}.{}", header.get_major_linker(), header.get_minor_linker()) },
		{ L"Loader Flags", std::format(L"0x{:X}", header.get_loader_flags()) },
	};

	m_List.SetItemCount((int)m_Items.size());
}

