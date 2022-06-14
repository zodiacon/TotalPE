#include "pch.h"
#include "PEImageView.h"
#include "PEStrings.h"
#include "SortHelper.h"
#include "ClipboardHelper.h"
#include "AssemblyView.h"
#include "ViewManager.h"

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
	m_hWndClient = m_List.Create(*this, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LVS_REPORT | LVS_OWNERDATA);
	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	auto cm = GetColumnManager(m_List);

	cm->AddColumn(L"Name", LVCFMT_LEFT, 150);
	cm->AddColumn(L"Value", LVCFMT_LEFT, 200);
	cm->AddColumn(L"Details", LVCFMT_LEFT, 450);

	cm->UpdateColumns();


	BuildItems();

	return 0;
}

void CPEImageView::OnStateChanged(HWND, int from, int to, DWORD oldState, DWORD newState) {
	Frame()->GetUI().UIEnable(ID_EDIT_COPY, m_List.GetSelectedCount() > 0);
}

void CPEImageView::BuildItems() {
	auto& header = PE().get_image();
	auto& path = Frame()->GetPEPath();
	DWORD fileSize = 0;
	HANDLE hFile = ::CreateFile(path, FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) {
		fileSize = ::GetFileSize(hFile, nullptr);
		::CloseHandle(hFile);
	}

	m_Items = std::vector<DataItem> {
		{ L"File Name", (PCWSTR)path.Mid(path.ReverseFind(L'\\') + 1), (PCWSTR)path },
		{ L"File Size", PEStrings::ToMemorySize(fileSize) },
		{ L"Alignment", std::format(L"0x{:X}", header.get_file_align()) },
		{ L"Checksum", std::format(L"0x{:X}", header.get_checksum()) },
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

bool CPEImageView::OnRightClickList(HWND lv, int row, int col, CPoint const& pt) {
	if (row < 0)
		return false;

	CMenu menu;
	menu.LoadMenu(IDR_CONTEXT);
	return Frame()->TrackPopupMenu(menu.GetSubMenu(4), 0, pt.x, pt.y);
}

LRESULT CPEImageView::OnCopy(WORD, WORD, HWND, BOOL&) const {
	auto text = ListViewHelper::GetSelectedRowsAsString(m_List);
	if (!text.IsEmpty())
		ClipboardHelper::CopyText(m_hWnd, text);
	return 0;
}

LRESULT CPEImageView::OnEntryPointAssembly(WORD, WORD, HWND, BOOL&) const {
	auto const& image = PE().get_image();
	pe_image_io io(image);
	io.set_image_offset(image.get_entry_point());
	std::vector<uint8_t> code;
	io.read(code, 0x1000);
	ViewManager::CreateAssemblyView(PE(), Frame()->GetTreeItemText(0) + L": Entry Point", code, image.get_image_base() + image.get_entry_point());

	return 0;
}

void CPEImageView::OnActivate(bool activate) {
	if (activate)
		Frame()->GetUI().UIEnable(ID_MAIN_ENTRYPOINTASSEMBLY, PE().get_image().get_entry_point() > 0);
}
