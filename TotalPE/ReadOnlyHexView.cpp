#include "pch.h"
#include "ReadOnlyHexView.h"
#include "ToolbarHelper.h"

void CReadOnlyHexView::SetData(std::vector<uint8_t> const& data) {
	m_data.SetData(0, data.data(), (ULONG)data.size());
	m_Hex.SetBufferManager(&m_data);
	Frame()->SetStatusText(2, std::format(L"Size: {} bytes", m_data.GetSize()).c_str());
}

LRESULT CReadOnlyHexView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	ToolBarButtonInfo const buttons[] = {
		{ ID_DATASIZE_1BYTE, IDI_NUM1, BTNS_CHECKGROUP },
		{ ID_DATASIZE_2BYTES, IDI_NUM2, BTNS_CHECKGROUP },
		{ ID_DATASIZE_4BYTES, IDI_NUM4, BTNS_CHECKGROUP },
		{ ID_DATASIZE_8BYTES, IDI_NUM8, BTNS_CHECKGROUP },
		{ 0 },
		{ ID_FILE_SAVE, IDI_SAVE, BTNS_BUTTON, L"Export" },
	};
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	CToolBarCtrl tb = ToolbarHelper::CreateAndInitToolBar(m_hWnd, buttons, _countof(buttons), 16);

	AddSimpleReBarBand(tb);
	Frame()->AddToolBar(tb);

	m_hWndClient = m_Hex.Create(m_hWnd, rcDefault, nullptr,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | WS_HSCROLL,
		WS_EX_CLIENTEDGE);

	tb.CheckButton(ID_DATASIZE_1BYTE);

	return 0;
}

LRESULT CReadOnlyHexView::OnShowWindow(UINT, WPARAM wp, LPARAM, BOOL&) {
	if (wp) {
		Frame()->SetStatusText(2, std::format(L"Size: {} bytes", m_data.GetSize()).c_str());
	}
	return 0;
}

LRESULT CReadOnlyHexView::OnChangeDataSize(WORD, WORD id, HWND, BOOL&) {
	m_Hex.SetDataSize(1 << (id - ID_DATASIZE_1BYTE));
	return 0;
}

LRESULT CReadOnlyHexView::OnSave(WORD, WORD, HWND, BOOL&) {
	CSimpleFileDialog dlg(FALSE, nullptr, nullptr, OFN_EXPLORER | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
		L"All Files\0*.*\0", m_hWnd);
	if (IDOK == dlg.DoModal()) {
		HANDLE hFile = ::CreateFile(dlg.m_szFileName, GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, 0, nullptr);
		if (hFile == INVALID_HANDLE_VALUE) {
			AtlMessageBox(m_hWnd, L"Failed to create file", IDS_TITLE, MB_ICONERROR);
			return 0;
		}
		DWORD bytes;
		if (!::WriteFile(hFile, m_data.GetRawData(0), (ULONG)m_data.GetSize(), &bytes, nullptr)) {
			AtlMessageBox(m_hWnd, L"Failed to write data", IDS_TITLE, MB_ICONERROR);
		}
		::CloseHandle(hFile);
	}

	return 0;
}

LRESULT CReadOnlyHexView::OnSetFocus(UINT, WPARAM, LPARAM, BOOL&) {
	m_Hex.SetFocus();
	return 0;
}

