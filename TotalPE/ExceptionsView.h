#pragma once

#include "View.h"
#include <VirtualListView.h>

class CExceptionsView :
	public CView<CExceptionsView>,
	public CVirtualListView<CExceptionsView> {
public:
	using CView::CView;

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);

	BEGIN_MSG_MAP(CExceptionsView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CExceptionsView>)
		CHAIN_MSG_MAP(CVirtualListView<CExceptionsView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	CListViewCtrl m_List;
	std::vector<pe_exception_entry> m_Items;
};

