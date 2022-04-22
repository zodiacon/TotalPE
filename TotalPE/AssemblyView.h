#pragma once

#include "View.h"

class CAssemblyView : public CView<CAssemblyView> {
public:
	CAssemblyView(pe_image_full const& pe) : CView(nullptr, pe) {}

	bool SetCode(uint64_t startAddress, std::vector<uint8_t> const& code);

	BEGIN_MSG_MAP(CTextView)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnCopy)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CAssemblyView>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	CEdit m_Edit;
};

