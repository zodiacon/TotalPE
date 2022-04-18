#pragma once

#include "View.h"
#include <VirtualListView.h>
#include <SortedFilteredVector.h>

class CSectionsView :
	public CView<CSectionsView>,
	public CVirtualListView<CSectionsView> {
public:
	using CView::CView;

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);

	BEGIN_MSG_MAP(CSectionsView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CSectionsView>)
		CHAIN_MSG_MAP(CVirtualListView<CSectionsView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	void BuildItems();

	CListViewCtrl m_List;
	SortedFilteredVector<pe_section*> m_Sections;
};

