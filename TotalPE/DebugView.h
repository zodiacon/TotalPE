#pragma once

#include "View.h"
#include <VirtualListView.h>
#include "ReadOnlyHexView.h"

class CDebugView :
	public CView<CDebugView>,
	public CVirtualListView<CDebugView> {
public:
	CDebugView(IMainFrame* frame, pe_image_full const& pe) : CView(frame, pe), m_HexView(frame) {}

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);
	void OnStateChanged(HWND h, int from, int to, DWORD oldState, DWORD newState);

	BEGIN_MSG_MAP(CDebugView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP_MEMBER(m_HexView)
		CHAIN_MSG_MAP(CView<CDebugView>)
		CHAIN_MSG_MAP(CVirtualListView<CDebugView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	void BuildItems();

	CListViewCtrl m_List;
	CHorSplitterWindow m_Splitter;
	CReadOnlyHexView m_HexView;
	std::unique_ptr<IBufferManager> m_Buffer;
	int m_SelectedIndex{ -1 };
	std::vector<pe_debug_entry> m_Items;
};

