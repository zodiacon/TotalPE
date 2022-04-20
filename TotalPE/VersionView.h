#pragma once

#include "View.h"
#include <VirtualListView.h>

class CVersionView :
	public CView<CVersionView>,
	public CVirtualListView<CVersionView> {
public:
	using CView::CView;

	void SetData(std::vector<uint8_t> const& data);

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);

	BEGIN_MSG_MAP(CVersionView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CVersionView>)
		CHAIN_MSG_MAP(CVirtualListView<CVersionView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	struct Item {
		std::wstring Name;
		std::wstring Value;
	};

	void BuildItems();

	uint8_t const* m_data;
	uint32_t m_size;
	CListViewCtrl m_List;
	std::vector<Item> m_Items;
};

