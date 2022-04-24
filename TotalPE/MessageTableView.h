#pragma once

#include "View.h"
#include <VirtualListView.h>
#include <SortedFilteredVector.h>

class CMessageTableView :
	public CView<CMessageTableView>,
	public CVirtualListView<CMessageTableView> {
public:
	using CView::CView;

	void SetData(uint8_t const* data);

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);

	BEGIN_MSG_MAP(CMessageTableView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CMessageTableView>)
		CHAIN_MSG_MAP(CVirtualListView<CMessageTableView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	struct Item {
		uint32_t Index;
		uint32_t Id;
		CString Text;
	};

	void BuildItems();

	CListViewCtrl m_List;
	SortedFilteredVector<Item> m_Items;
	uint8_t const* m_data{ nullptr };
};

