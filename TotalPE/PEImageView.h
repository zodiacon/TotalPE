#pragma once

#include "View.h"
#include <VirtualListView.h>
#include <SortedFilteredVector.h>
#include "resource.h"

class CPEImageView : 
	public CView<CPEImageView>,
	public CVirtualListView<CPEImageView> {
public:
	using CView::CView;

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);
	bool IsSortable(HWND, int col) const;
	bool OnRightClickList(HWND lv, int row, int col, CPoint const& pt);
	void OnStateChanged(HWND, int from, int to, DWORD oldState, DWORD newState);
	void OnActivate(bool activate);

	BEGIN_MSG_MAP(CPEImageView)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnCopy)
		COMMAND_ID_HANDLER(ID_MAIN_ENTRYPOINTASSEMBLY, OnEntryPointAssembly)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CPEImageView>)
		CHAIN_MSG_MAP(CVirtualListView<CPEImageView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCopy(WORD, WORD, HWND, BOOL&) const;
	LRESULT OnEntryPointAssembly(WORD, WORD, HWND, BOOL&) const;

private:
	void BuildItems();

	struct DataItem {
		std::wstring Name, Value, Details;
	};

	CListViewCtrl m_List;
	SortedFilteredVector<DataItem> m_Items;
};

