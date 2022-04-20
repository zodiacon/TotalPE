#pragma once

#include "View.h"
#include <VirtualListView.h>
#include <SortedFilteredVector.h>
#include <QuickFindEdit.h>

class CExportsView :
	public CView<CExportsView>,
	public CVirtualListView<CExportsView> {
public:
	using CView::CView;

	CString GetColumnText(HWND, int row, int col) const;
	int GetRowImage(HWND, int row, int) const;

	void DoSort(SortInfo const* si);

	BEGIN_MSG_MAP(CExportsView)
		COMMAND_CODE_HANDLER(EN_DELAYCHANGE, OnFilterChanged)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CExportsView>)
		CHAIN_MSG_MAP(CVirtualListView<CExportsView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnFilterChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	void ApplyFilter(PCWSTR text);
	void BuildItems();
	void UpdateStatusText();

	CQuickFindEdit m_QuickFind;
	CListViewCtrl m_List;
	SortedFilteredVector<pe_export_entry> m_Exports;
};

