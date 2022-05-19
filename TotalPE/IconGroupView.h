#pragma once

#include "View.h"

class CIconGroupView : 
	public CView<CIconGroupView>,
	public CScrollImpl<CIconGroupView> {
public:
	using CView::CView;

	void SetGroupIconData(std::vector<uint8_t> const& data);
	void SetIconData(std::vector<uint8_t> const& data, bool icon);
	void DoPaint(CDCHandle);

	BEGIN_MSG_MAP(CIconGroupView)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		COMMAND_ID_HANDLER(ID_ICON_EXPORT, OnExportIcon)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		//MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP(CScrollImpl<CIconGroupView>)
		CHAIN_MSG_MAP(CView)
	END_MSG_MAP()

private:
	struct IconData {
		CIconHandle Icon;
		int Size;
		UINT Id;
		UINT Colors;
		CRect Rect;
	};
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnExportIcon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CIconHandle m_Icon;
	int m_IconSize;
	std::vector<IconData> m_Icons;
	int m_SelectedIcon{ -1 };
};

