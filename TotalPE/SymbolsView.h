#pragma once

#include "View.h"
#include <VirtualListView.h>
#include <SortedFilteredVector.h>
#include "DiaHelper.h"

class CSymbolsView :
	public CView<CSymbolsView>,
	public CVirtualListView<CSymbolsView> {
public:
	using CView::CView;

	CSymbolsView(IMainFrame* frame, SymbolTag tag) : CView(frame), m_SymbolTag(tag) {}

	void BuildItems(DiaSession const& session);

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);
	int GetRowImage(HWND, int row, int col) const;
	int GetSaveColumnRange(int& start) const;

	BEGIN_MSG_MAP(CSymbolsView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CSymbolsView>)
		CHAIN_MSG_MAP(CVirtualListView<CSymbolsView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	enum class ColumnType {
		Name, Id, UndecoratedName, Offset, Address, VA, Location, Type, Tag,
	};

	CListViewCtrl m_List;
	SortedFilteredVector<DiaSymbol> m_Symbols;
	SymbolTag m_SymbolTag;
};

#pragma once
