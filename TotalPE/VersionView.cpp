#include "pch.h"
#include "VersionView.h"
#include "PEStrings.h"
#include "SortHelper.h"

void CVersionView::SetData(std::vector<uint8_t> const& data) {
	m_data = data.data();
	m_size = (uint32_t)data.size();

	BuildItems();
}

CString CVersionView::GetColumnText(HWND, int row, int col) const {
	auto& item = m_Items[row];
	switch (col) {
		case 0: return item.Name.c_str();
		case 1: return item.Value.c_str();
	}
	return CString();
}

void CVersionView::DoSort(SortInfo const* si) {
	if (si == nullptr)
		return;

	auto compare = [&](auto& item1, auto& item2) {
		return SortHelper::Sort(item1.Name, item2.Name, si->SortAscending);
	};
	std::sort(m_Items.begin(), m_Items.end(), compare);
}

bool CVersionView::IsSortable(HWND, int col) const {
	return col == 0;
}

LRESULT CVersionView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_List.Create(*this, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA);
	auto cm = GetColumnManager(m_List);

	cm->AddColumn(L"Name", LVCFMT_LEFT, 140);
	cm->AddColumn(L"Value", LVCFMT_LEFT, 330);
	cm->UpdateColumns();

	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	return 0;
}

void CVersionView::BuildItems() {
	pe_file_version_info info(m_data, m_size);
	if (!info.is_valid_info()) {
		WCHAR text[] = L"Version information is invalid";
		m_List.GetEmptyText(text, _countof(text));
		return;
	}

	m_Items.clear();
	m_Items.reserve(16);
	if (info.has_file_info()) {
		auto& fi = info.get_file_info();
		Item items[] = {
			{ L"Struct Version", std::format(L"{}.{}", HIWORD(fi.struct_version), LOWORD(fi.struct_version)) },
			{ L"File Version", std::format(L"{}.{}.{}.{}", HIWORD(fi.file_version_ms), LOWORD(fi.file_version_ms), HIWORD(fi.file_version_ls), LOWORD(fi.file_version_ls)) },
			{ L"Product Version", std::format(L"{}.{}.{}.{}", HIWORD(fi.product_version_ms), LOWORD(fi.product_version_ms), HIWORD(fi.product_version_ls), LOWORD(fi.product_version_ls)) },
			{ L"OS File", PEStrings::VersionFileOSToString(fi.file_os) },
			{ L"File Type", PEStrings::FileTypeToString(fi.file_type) },
			{ L"File Subtype", PEStrings::FileSubTypeToString(fi.file_type, fi.file_sub_type) },
			{ L"File Flags", PEStrings::FileFlagsToString(fi.file_flags & fi.file_flags_mask) },
		};
		m_Items.insert(m_Items.begin(), std::begin(items), std::end(items));
	}
	auto& st = info.get_string_table();
	for (auto& [t, sv] : st) {
		for(auto& s : sv)
			m_Items.push_back({ s.first, s.second });
	}
	
	m_List.SetItemCount((int)m_Items.size());
}
