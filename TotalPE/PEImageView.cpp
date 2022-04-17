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
	auto cm = GetColumnManager(m_List);

	cm->AddColumn(L"Name", LVCFMT_LEFT, 150);
	cm->AddColumn(L"Value", LVCFMT_LEFT, 200);
	cm->AddColumn(L"Details", LVCFMT_LEFT, 450);

	cm->UpdateColumns();

	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	BuildItems();

	return 0;
}

void CPEImageView::BuildItems() {
	auto& header = PE().header();
	auto& oh = PE().optional_header();
	auto& path = Frame()->GetPEPath();

	m_Items = std::vector<DataItem>{
		{ L"File Name", (PCWSTR)path.Mid(path.ReverseFind(L'\\') + 1), (PCWSTR)path },
		{ L"Time/Date Stamp", std::format(L"0x{:08X}", header.time_date_stamp()) },
		{ L"Machine", std::format(L"{} (0x{:X})", (uint32_t)header.machine(), (uint32_t)header.machine()), std::to_wstring(header.machine()) },
		{ L"Subsystem", std::to_wstring((uint32_t)oh.subsystem()), std::to_wstring(oh.subsystem()) },
		{ L"Sections", std::to_wstring(header.numberof_sections()) },
		{ L"Characteristics", std::format(L"0x{:08X}", (uint32_t)header.characteristics()), std::to_wstring(header.characteristics()) },
		{ L"Magic", std::format(L"{} (0x{:X})", (uint32_t)oh.magic(), (uint32_t)oh.magic()), std::to_wstring(oh.magic()) },
		{ L"DLL Characteristics", std::format(L"0x{:04X}", (uint32_t)oh.dll_characteristics()), PEStrings::DllCharacteristicsToString(oh.dll_characteristics()) },
		{ L"Image Base", std::format(L"0x{:X}", oh.imagebase()) },
		{ L"Image Size", PEStrings::ToMemorySize(oh.sizeof_image()) },
		{ L"Stack Reserve", PEStrings::ToMemorySize(oh.sizeof_stack_reserve()) },
		{ L"Stack Commit", PEStrings::ToMemorySize(oh.sizeof_stack_commit()) },
		{ L"Heap Reserve", PEStrings::ToMemorySize(oh.sizeof_heap_reserve()) },
		{ L"Heap Commit", PEStrings::ToMemorySize(oh.sizeof_heap_commit()) },
		{ L"Is Managed?", PE().data_directory(LIEF::PE::DATA_DIRECTORY::CLR_RUNTIME_HEADER).has_section() ? L"Yes" : L"No" },
		{ L"Code Size", PEStrings::ToMemorySize(oh.sizeof_code()) },
		{ L"Entry Point", std::format(L"0x{:X}", oh.addressof_entrypoint()) },
		{ L"OS Version", std::format(L"{}.{}", oh.major_operating_system_version(), oh.minor_operating_system_version()) },
		{ L"Image Version", std::format(L"{}.{}", oh.major_image_version(), oh.minor_image_version()) },
		{ L"Linker Version", std::format(L"{}.{}", oh.major_linker_version(), oh.minor_linker_version()) },
		{ L"Loader Flags", std::format(L"0x{:X}", oh.loader_flags()) },
	};

	m_List.SetItemCount((int)m_Items.size());
}

