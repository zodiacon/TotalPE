#include "pch.h"
#include "SymbolsView.h"
#include "resource.h"
#include "PEStrings.h"
#include <SortHelper.h>

int CSymbolsView::GetRowImage(HWND, int row, int col) const {
	return 0;
}

int CSymbolsView::GetSaveColumnRange(HWND, int& start) const {
	return -1;
}

CString CSymbolsView::GetName(DiaSymbol const& sym) const {
	return CString();
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
	cm->AddColumn(L"Tag", LVCFMT_LEFT, 100, ColumnType::Tag);
	cm->AddColumn(L"Id", LVCFMT_RIGHT, 70, ColumnType::Id);
	if (m_SymbolTag == SymbolTag::Function || m_SymbolTag == SymbolTag::Null || m_SymbolTag == SymbolTag::PublicSymbol) {
		cm->AddColumn(L"Undecorated Name", LVCFMT_LEFT, 300, ColumnType::UndecoratedName);
	}
//	cm->AddColumn(L"Location", LVCFMT_LEFT, 90, ColumnType::Location);
	cm->UpdateColumns();

	return 0;
}

void CSymbolsView::BuildItems(DiaSession const& session) {
	m_Symbols = session.FindChildren(session.GlobalScope(), nullptr, m_SymbolTag);
	m_List.SetItemCount((int)m_Symbols.size());
}

CString CSymbolsView::GetColumnText(HWND h, int row, int col) const {
	auto const& sym = m_Symbols[row];
	switch (GetColumnManager(h)->GetColumnTag<ColumnType>(col)) {
		case ColumnType::Name: return sym.Name().c_str();
		case ColumnType::Tag: return PEStrings::SymbolTagToString(sym.Tag());
		case ColumnType::UndecoratedName: return sym.UndecoratedName().c_str();
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
			case ColumnType::UndecoratedName: return SortHelper::Sort(s1.UndecoratedName(), s2.UndecoratedName(), asc);
			case ColumnType::Tag: return SortHelper::Sort(s1.Tag(), s2.Tag(), asc);
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

