#pragma once

#include "View.h"
#include <VirtualListView.h>
#include <SortedFilteredVector.h>
#include <QuickFindEdit.h>

class CImportsView :
	public CView<CImportsView>,
	public CVirtualListView<CImportsView> {
public:
	using CView::CView;

	CString GetColumnText(HWND, int row, int col) const;
	int GetRowImage(HWND, int row, int) const;
	void OnStateChanged(HWND h, int from, int to, DWORD oldState, DWORD newState);
	void DoSort(SortInfo const* si);
	bool IsSortable(HWND h, int col) const;

	BEGIN_MSG_MAP(CImportsView)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		NOTIFY_CODE_HANDLER(LVN_KEYDOWN, OnListViewKeyDown)
		COMMAND_CODE_HANDLER(EN_DELAYCHANGE, OnFilterChanged)
		COMMAND_CODE_HANDLER(ID_FILE_SAVE, OnExport)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CImportsView>)
		CHAIN_MSG_MAP(CVirtualListView<CImportsView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnFilterChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnExport(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnListViewKeyDown(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

private:
	void ApplyFilter(PCWSTR text);
	void BuildItems();
	void UpdateStatusText();

	CQuickFindEdit m_QuickFind;
	CListViewCtrl m_LibList;
	CListViewCtrl m_List;
	CSplitterWindow m_Splitter;
	std::vector<pe_import_library> m_ImportLibs;
	SortedFilteredVector<pe_import_function> m_Imports;
};

