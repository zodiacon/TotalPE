#pragma once

#include "View.h"

class CMenuView : 
	public CView<CMenuView>,
	public CScrollImpl<CMenuView> {
public:
	using CView::CView;

	bool SetData(std::vector<uint8_t> const& data);

	void DoPaint(CDCHandle dc);

	BEGIN_MSG_MAP(CMenuView)
		CHAIN_MSG_MAP(CScrollImpl<CMenuView>)
		CHAIN_MSG_MAP(CView<CMenuView>)
	END_MSG_MAP()

private:
	CMenu m_menu;
};

