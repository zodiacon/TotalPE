#include "pch.h"
#include "SymbolsView.h"
#include "resource.h"
#include "PEStrings.h"
#include <SortHelper.h>

int CSymbolsView::GetRowImage(HWND, int row, int col) const {
	return 0;
}

int CSymbolsView::GetSaveColumnRange(int& start) const {
	return -1;
}

LRESULT CSymbolsView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_List.Create(*this, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA);
	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	CImageList images;
	images.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);
	images.AddIcon(AtlLoadIconImage(IDI_SYMBOLS, 0, 16, 16));
	m_List.SetImageList(images, LVSIL_SMALL);

	auto cm = GetColumnManager(m_List);

	cm->AddColumn(L"Name", LVCFMT_LEFT, 350, ColumnType::Name);
	cm->AddColumn(L"Type", LVCFMT_LEFT, 100, ColumnType::Type);
	cm->AddColumn(L"Undecorated Name", LVCFMT_LEFT, 300, ColumnType::UndecoratedName);
	cm->AddColumn(L"Id", LVCFMT_RIGHT, 70, ColumnType::Id);
//	cm->AddColumn(L"Location", LVCFMT_LEFT, 90, ColumnType::Location);
	cm->UpdateColumns();

	return 0;
}

void CSymbolsView::BuildItems(DiaSession const& session, SymbolTag tag) {
	m_Symbols = session.FindChildren(session.GlobalScope(), tag);
	m_List.SetItemCount((int)m_Symbols.size());
}

CString CSymbolsView::GetColumnText(HWND h, int row, int col) const {
	auto const& sym = m_Symbols[row];
	switch (GetColumnManager(h)->GetColumnTag<ColumnType>(col)) {
		case ColumnType::Name: return sym.Name().c_str();
		case ColumnType::Type: return PEStrings::SymbolTagToString(sym.Tag());
		case ColumnType::UndecoratedName: return PEStrings::UndecorateName(sym.Name().c_str()).c_str();
		case ColumnType::Id: return std::to_wstring(sym.Id()).c_str();
		case ColumnType::Offset: return std::format(L"0x{:X}", sym.Offset()).c_str();
		case ColumnType::Location: return PEStrings::SymbolLocationToString(sym.Location());
	}
	return L"";
}

void CSymbolsView::DoSort(SortInfo const* si) {
	auto col = GetColumnManager(si->hWnd)->GetColumnTag<ColumnType>(si->SortColumn);
	auto asc = si->SortAscending;

	auto compare = [&](auto const& s1, auto const& s2) -> bool {
		switch (col) {
			case ColumnType::Name: return SortHelper::Sort(s1.Name(), s2.Name(), asc);
			case ColumnType::UndecoratedName: return SortHelper::Sort(PEStrings::UndecorateName(s1.Name().c_str()), PEStrings::UndecorateName(s2.Name().c_str()), asc);
			case ColumnType::Type: return SortHelper::Sort(s1.Tag(), s2.Tag(), asc);
			case ColumnType::Location: return SortHelper::Sort(s1.Location(), s2.Location(), asc);
			case ColumnType::Id: return SortHelper::Sort(s1.Id(), s2.Id(), asc);
		};
		return false;
	};
	CWaitCursor wait(false);

	if (m_Symbols.size() > 5000 && (col == ColumnType::Name || col == ColumnType::UndecoratedName))
		wait.Set();
	m_Symbols.Sort(compare);
}

