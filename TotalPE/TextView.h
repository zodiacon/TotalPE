#pragma once

#include "View.h"

class CTextView : public CView<CTextView> {
public:
	using CView::CView;

	void SetText(PCWSTR text, TextFormat format = TextFormat::Plain);

	BEGIN_MSG_MAP(CTextView)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnCopy)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CTextView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	static void FormatText(CString& text, TextFormat format);
	static void FormatXml(CString& xml);

private:
	CEdit m_Edit;
	CString m_text;
};


