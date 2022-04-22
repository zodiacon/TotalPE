#pragma once

#include "View.h"
#include <VirtualListView.h>
#include "ReadOnlyHexView.h"

class CDirectoriesView :
	public CView<CDirectoriesView>,
	public CVirtualListView<CDirectoriesView> {
public:
	CDirectoriesView(IMainFrame* frame, pe_image_full const& pe) : CView(frame, pe), m_HexView(frame) {}

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);
	void OnStateChanged(HWND h, int from, int to, DWORD oldState, DWORD newState);

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
		std::string Section;
	};

	CListViewCtrl m_List;
	CHorSplitterWindow m_Splitter;
	CReadOnlyHexView m_HexView;
	std::vector<Item> m_Directories;
	Item m_Selected;
};

