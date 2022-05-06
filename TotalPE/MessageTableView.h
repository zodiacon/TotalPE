#pragma once

#include "View.h"
#include <VirtualListView.h>
#include <SortedFilteredVector.h>

class CMessageTableView :
	public CView<CMessageTableView>,
	public CVirtualListView<CMessageTableView> {
public:
	using CView::CView;

	void SetMessageTableData(uint8_t const* data);
	void SetStringTableData(std::vector<uint8_t> const& data, UINT id);

	CString GetColumnText(HWND, int row, int col) const;
	void DoSort(SortInfo const* si);

	BEGIN_MSG_MAP(CMessageTableView)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnCopy)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CMessageTableView>)
		CHAIN_MSG_MAP(CVirtualListView<CMessageTableView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) const;

private:
	struct Item {
		uint32_t Index;
		uint32_t Id;
		std::wstring Text;
	};

	CListViewCtrl m_List;
	SortedFilteredVector<Item> m_Items;
};

