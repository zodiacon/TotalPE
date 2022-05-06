#include "pch.h"
#include "MessageTableView.h"
#include "SortHelper.h"
#include "ClipboardHelper.h"

void CMessageTableView::SetMessageTableData(uint8_t const* data) {
	uint32_t index = 0;
	auto res = (MESSAGE_RESOURCE_DATA const*)data;
	for (DWORD i = 0; i < res->NumberOfBlocks; i++) {
		auto const& block = res->Blocks[i];
		auto entries = (MESSAGE_RESOURCE_ENTRY*)((PBYTE const)res + block.OffsetToEntries);
		for (DWORD id = block.LowId; id <= block.HighId; id++) {
			Item item;
			item.Index = ++index;
			item.Id = id;
			if (entries->Flags & 1)
				item.Text = (PCWSTR)CString((PCWSTR)entries->Text, entries->Length / sizeof(WCHAR));
			else
				item.Text = (PCWSTR)CString(CStringA((PCSTR)entries->Text, entries->Length));
			m_Items.push_back(std::move(item));
			entries = (MESSAGE_RESOURCE_ENTRY*)((PBYTE)entries + entries->Length);
		}
	}
	m_List.SetItemCount((int)m_Items.size());
}

void CMessageTableView::SetStringTableData(std::vector<uint8_t> const& data, UINT id) {
	UINT index = (UINT)m_Items.size();
	auto st = (WORD const*)data.data();
	id = (id - 1) * 16;
	int size = (int)data.size();
	while (size > 0) {
		while (*st == 0) {
			st++;
			id++;
			size -= 2;
		}
		if (size <= 0)
			break;
		std::wstring s((PCWSTR)(st + 1), *st);
		m_Items.push_back({ ++index, id++, std::move(s) });
		size -= (*st + 1) * 2;
		st += *st + 1;
	}
	m_List.SetItemCount((int)m_Items.size());
}

CString CMessageTableView::GetColumnText(HWND, int row, int col) const {
	auto& item = m_Items[row];
	switch (col) {
		case 0: return std::to_wstring(item.Index).c_str();
		case 1: return std::format(L"0x{:X}", item.Id).c_str();
		case 2: return item.Text.c_str();
	}
	return CString();
}

void CMessageTableView::DoSort(SortInfo const* si) {
	if (si == nullptr)
		return;

	auto compare = [&](auto& item1, auto& item2) {
		switch (si->SortColumn) {
			case 0: return SortHelper::Sort(item1.Index, item2.Index, si->SortAscending);
			case 1: return SortHelper::Sort(item1.Id, item2.Id, si->SortAscending);
			case 2: return SortHelper::Sort(item1.Text, item2.Text, si->SortAscending);
		}
		return false;
	};
	m_Items.Sort(compare);
}

LRESULT CMessageTableView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_List.Create(*this, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA, WS_EX_CLIENTEDGE);
	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	auto cm = GetColumnManager(m_List);
	cm->AddColumn(L"Dummy", LVCFMT_LEFT, 90);
	cm->AddColumn(L"Index", LVCFMT_RIGHT, 80);
	cm->AddColumn(L"ID", LVCFMT_RIGHT, 80);
	cm->AddColumn(L"Text", LVCFMT_LEFT, 600);
	cm->UpdateColumns();
	cm->DeleteColumn(0);

	m_Items.reserve(64);

	return 0;
}

LRESULT CMessageTableView::OnCopy(WORD, WORD, HWND, BOOL&) const {
	auto text = ListViewHelper::GetSelectedRowsAsString(m_List);
	if (!text.IsEmpty())
		ClipboardHelper::CopyText(m_hWnd, text);
	return 0;
}

