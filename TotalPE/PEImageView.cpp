#include "pch.h"
#include "PEImageView.h"
#include "PEStrings.h"

CString CPEImageView::GetColumnText(HWND, int row, int col) const {
	auto& item = m_Items[row];
	switch (col) {
		case 0: return item.Name.c_str();
		case 1: return item.Value.c_str();
		case 2: return item.Details.c_str();
	}
	return CString();
}

LRESULT CPEImageView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	m_hWndClient = m_List.Create(*this, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA, WS_EX_CLIENTEDGE);
	auto cm = GetColumnManager(m_List);

	cm->AddColumn(L"Name", LVCFMT_LEFT, 150);
	cm->AddColumn(L"Value", LVCFMT_LEFT, 200);
	cm->AddColumn(L"Details", LVCFMT_LEFT, 400);

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
	};

	//m_Items.push_back(Item(L"Magic", PEStrings::ToDecAndHex(magic), PEStrings::MagicToString((OptionalHeaderMagic)magic)));
	//m_Items.push_back(Item(L"Machine", PEStrings::ToDecAndHex(fh.Machine), PEStrings::MachineTypeToString((MachineType)fh.Machine)));
	//m_Items.push_back(Item(L"Image Characteristics", PEStrings::ToDecAndHex(fh.Characteristics), PEStrings::CharacteristicsToString((ImageCharacteristics)fh.Characteristics)));
	//m_Items.push_back(Item(L"DLL Characteristics", PEStrings::ToDecAndHex(dllChar), PEStrings::DllCharacteristicsToString((DllCharacteristics)dllChar)));
	//m_Items.push_back(Item(L"Is Managed", _parser->IsManaged() ? L"Yes" : L"No"));
	//m_Items.push_back(Item(L"Image Base", PEStrings::ToHex(pe64 ? oh64.ImageBase : oh32.ImageBase)));
	//m_Items.push_back(Item(L"Heap Commit", PEStrings::ToMemorySize(pe64 ? oh64.SizeOfHeapCommit : oh32.SizeOfHeapCommit)));
	//m_Items.push_back(Item(L"Heap Reserve", PEStrings::ToMemorySize(pe64 ? oh64.SizeOfHeapReserve : oh32.SizeOfHeapReserve)));
	//m_Items.push_back(Item(L"Stack Commit", PEStrings::ToMemorySize(pe64 ? oh64.SizeOfStackCommit : oh32.SizeOfStackCommit)));
	//m_Items.push_back(Item(L"Stack Reserve", PEStrings::ToMemorySize(pe64 ? oh64.SizeOfStackReserve : oh32.SizeOfStackReserve)));
	//m_Items.push_back(Item(L"Code Size", PEStrings::ToMemorySize(pe64 ? oh64.SizeOfCode : oh32.SizeOfCode)));
	//m_Items.push_back(Item(L"Entry Point", PEStrings::ToHex(pe64 ? oh64.AddressOfEntryPoint : oh32.AddressOfEntryPoint)));
	//m_Items.push_back(Item(L"Image Size", PEStrings::ToMemorySize(pe64 ? oh64.SizeOfImage : oh32.SizeOfImage)));
	//m_Items.push_back(Item(L"OS Version", (std::to_wstring(pe64 ? oh64.MajorOperatingSystemVersion : oh32.MajorOperatingSystemVersion) + L"." +
	//	std::to_wstring(pe64 ? oh64.MinorOperatingSystemVersion : oh32.MinorOperatingSystemVersion)).c_str()));
	//_items.push_back(Item(L"Image Version", (std::to_wstring(pe64 ? oh64.MajorImageVersion : oh32.MajorImageVersion) + L"." +
	//	std::to_wstring(pe64 ? oh64.MinorImageVersion : oh32.MinorImageVersion)).c_str()));
	//_items.push_back(Item(L"Linker Version", (std::to_wstring(pe64 ? oh64.MajorLinkerVersion : oh32.MajorLinkerVersion) + L"." +
	//	std::to_wstring(pe64 ? oh64.MinorLinkerVersion : oh32.MinorLinkerVersion)).c_str()));
	//_items.push_back(Item(L"Loader Flags", PEStrings::ToDecAndHex(pe64 ? oh64.LoaderFlags : oh32.LoaderFlags)));

	m_List.SetItemCount((int)m_Items.size());
}

