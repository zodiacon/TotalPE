#pragma once

#include "View.h"
#include "ReadOnlyHexView.h"
#include <VirtualListView.h>

class CResourcesView : 
	public CView<CResourcesView>,
	public CVirtualListView<CResourcesView> {
public:
	CResourcesView(IMainFrame* frame, pe_image_full const& pe) : CView(frame, pe), m_HexView(frame) {}

	CString GetColumnText(HWND, int row, int col) const;
	int GetRowImage(HWND, int row, int) const;
	void DoSort(SortInfo const* si);
	void OnStateChanged(HWND h, int from, int to, DWORD oldState, DWORD newState);
	bool OnRightClickList(HWND, int row, int col, CPoint const& pt);

	BEGIN_MSG_MAP(CResourcesView)
		COMMAND_ID_HANDLER(ID_RESOURCES_GOTORESOURCE, OnGoToResource)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CVirtualListView<CResourcesView>)
		CHAIN_MSG_MAP(CView<CResourcesView>)
	END_MSG_MAP()

private:
	void BuildItems();
	void ParseResourceDir(pe_resource_directory_entry const& dir, int level);

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnGoToResource(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	struct ResourceItem {
		std::wstring Type;
		std::wstring Name;
		std::wstring Language;
		pe_resource_data_entry Data;
		WORD TypeIndex;
	};

	CListViewCtrl m_List;
	std::vector<ResourceItem> m_Items;
	CSplitterWindow m_Splitter;
	CReadOnlyHexView m_HexView;
	ResourceItem m_CurrentResource;
};

