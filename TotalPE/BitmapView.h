#pragma once

#include "View.h"

class CBitmapView :
	public CView<CBitmapView>,
	public CZoomScrollImpl<CBitmapView> {
public:
	using CView::CView;

	bool SetData(std::vector<uint8_t> const& data);
	void DoPaint(CDCHandle);

	BEGIN_MSG_MAP(CBitmapView)
//		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CZoomScrollImpl<CBitmapView>)
		CHAIN_MSG_MAP(CView)
	END_MSG_MAP()

private:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	UINT m_Width, m_Height;
	CBitmap m_bmp;
};


