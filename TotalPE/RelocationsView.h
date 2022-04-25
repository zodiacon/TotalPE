#pragma once

#include "View.h"
#include <VirtualListView.h>

class CRelocationsView :
	public CView<CRelocationsView>,
	public CVirtualListView<CRelocationsView> {
public:
	using CView::CView;

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);

	BEGIN_MSG_MAP(CRelocationsView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CRelocationsView>)
		CHAIN_MSG_MAP(CVirtualListView<CRelocationsView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	CListViewCtrl m_List;
	std::vector<pe_relocation_entry> m_Items;
};

