#pragma once

#include "View.h"
#include "SimpleHexControl.h"
#include "MemoryBuffer.h"
#include "resource.h"

class CReadOnlyHexView : public CView<CReadOnlyHexView> {
public:
	CReadOnlyHexView(IMainFrame* frame) : CView(frame) {}

	void SetData(std::vector<uint8_t> const& data);
	void ClearData();
	void SetAddress(uint32_t address);
	CHexControl& GetHexControl();
	CHexControl const& GetHexControl() const;
	//void OnActivate(bool active);

	BEGIN_MSG_MAP(CreadOnlyHexView)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
		MESSAGE_HANDLER(::RegisterWindowMessage(L"WTLHelperUpdateTheme"), OnUpdateTheme)
		NOTIFY_CODE_HANDLER(TBN_DROPDOWN, OnDropDown)
		COMMAND_RANGE_HANDLER(ID_DATASIZE_1BYTE, ID_DATASIZE_8BYTES, OnChangeDataSize)
		COMMAND_RANGE_HANDLER(ID_BYTESPERLINE_8, ID_BYTESPERLINE_128, OnChangeBytesPerLine)
		COMMAND_ID_HANDLER(ID_FILE_SAVE, OnSave)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CView<CReadOnlyHexView>)
	END_MSG_MAP()

private:
	void UpdateColors();
	void UpdateStatusText();

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnUpdateTheme(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChangeDataSize(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnChangeBytesPerLine(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDropDown(int /*idCtrl*/, LPNMHDR hdr, BOOL& /*bHandled*/);

private:
	CHexControl m_Hex;
	MemoryBuffer m_data;
	int m_DataSize{ 1 };
	int m_BytesPerLine{ 32 };
};

