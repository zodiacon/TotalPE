#pragma once

#include "View.h"
#include <VirtualListView.h>

class CDirectoriesView :
	public CView<CDirectoriesView>,
	public CVirtualListView<CDirectoriesView> {
public:
	using CView::CView;

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);

	BEGIN_MSG_MAP(CDirectoriesView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CDirectoriesView>)
		CHAIN_MSG_MAP(CVirtualListView<CDirectoriesView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	struct Item {
		std::wstring Name;
		int Index;
		uint32_t Address;
		uint32_t Size;
	};

	CListViewCtrl m_List;
	std::vector<Item> m_Directories;
};

