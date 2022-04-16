#pragma once

#include "View.h"
#include <VirtualListView.h>
#include <SortedFilteredVector.h>

class CPEImageView : 
	public CView<CPEImageView>,
	public CVirtualListView<CPEImageView> {
public:
	using CView::CView;

	CString GetColumnText(HWND, int row, int col) const;

	BEGIN_MSG_MAP(CPEImageView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CPEImageView>)
		CHAIN_MSG_MAP(CVirtualListView<CPEImageView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	void BuildItems();

	struct DataItem {
		std::wstring Name, Value, Details;
	};

	CListViewCtrl m_List;
	SortedFilteredVector<DataItem> m_Items;
};

