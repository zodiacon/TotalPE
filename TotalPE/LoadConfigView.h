#pragma once

#include "View.h"
#include <VirtualListView.h>
#include <SortedFilteredVector.h>
#include "SymbolsHandler.h"

class CLoadConfigView :
	public CView<CLoadConfigView>,
	public CVirtualListView<CLoadConfigView> {
public:
	using CView::CView;

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);
	bool IsSortable(HWND, int col) const;
	int GetRowImage(HWND hList, int row, int col) const;

	BEGIN_MSG_MAP(CLoadConfigView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CLoadConfigView>)
		CHAIN_MSG_MAP(CVirtualListView<CLoadConfigView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	void BuildItems();

	struct DataItem {
		std::wstring Name, Value, Details;
	};
	struct CfgFunction {
		std::string Name;
		uint32_t Rva;
		bool Export;
	};
	CListViewCtrl m_List;
	CSplitterWindow m_Splitter;
	CListViewCtrl m_CfgList;
	std::vector<DataItem> m_Items;
	std::vector<CfgFunction> m_CfgFunctions;
	SymbolsHandler m_symbols;
};

