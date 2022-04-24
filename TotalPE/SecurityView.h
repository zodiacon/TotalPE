#pragma once

#include "View.h"
#include <VirtualListView.h>

class CSecurityView :
	public CView<CSecurityView>,
	public CVirtualListView<CSecurityView> {
public:
	using CView::CView;

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);
	bool OnDoubleClickList(HWND, int row, int, CPoint const& pt);

	BEGIN_MSG_MAP(CSecurityView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CSecurityView>)
		CHAIN_MSG_MAP(CVirtualListView<CSecurityView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	CListViewCtrl m_List;
	std::vector<pe_security_entry> m_Items;
};

