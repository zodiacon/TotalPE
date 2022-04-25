#pragma once

#include "View.h"
#include <VirtualListView.h>
#include "ReadOnlyHexView.h"

class CSecurityView :
	public CView<CSecurityView>,
	public CVirtualListView<CSecurityView> {
public:
	CSecurityView(IMainFrame* frame, pe_image_full const& pe) : CView(frame, pe), m_HexView(frame) {}

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);
	bool OnDoubleClickList(HWND, int row, int, CPoint const& pt);
	void OnStateChanged(HWND h, int from, int to, DWORD oldState, DWORD newState);

	BEGIN_MSG_MAP(CSecurityView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CSecurityView>)
		CHAIN_MSG_MAP(CVirtualListView<CSecurityView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	CReadOnlyHexView m_HexView;
	CListViewCtrl m_List;
	CHorSplitterWindow m_Splitter;
	std::vector<pe_security_entry> m_Items;
};

