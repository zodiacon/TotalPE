#pragma once

#include "View.h"

class CXMLView :
	public CView<CXMLView> {
public:
	CXMLView(IMainFrame* frame, TreeItemType type, pe_image_full const& pe, CString const& xml);

	BEGIN_MSG_MAP(CXMLView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CXMLView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	CEdit m_Edit;
	CString m_xml;
};


