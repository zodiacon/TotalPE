#pragma once

#include "View.h"
#include <VirtualListView.h>

class CAcceleratorTableView :
	public CView<CAcceleratorTableView>,
	public CVirtualListView<CAcceleratorTableView> {
public:
	using CView::CView;

	void AddAccelTable(std::vector<uint8_t> const& data);

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);

	BEGIN_MSG_MAP(CAcceleratorTableView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CAcceleratorTableView>)
		CHAIN_MSG_MAP(CVirtualListView<CAcceleratorTableView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	static std::wstring KeyFlagsToString(DWORD flags);

	struct Accelerator {
		WORD Flags;
		WORD Key;
		WORD Id;
		WORD _padding;
	};

	CListViewCtrl m_List;
	std::vector<Accelerator> m_Items;
};


