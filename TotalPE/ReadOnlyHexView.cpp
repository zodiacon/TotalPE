#include "pch.h"
#include "ReadOnlyHexView.h"

void CReadOnlyHexView::SetData(std::vector<uint8_t> data) {
	m_data = std::move(data);
	DisplayData();
}

LRESULT CReadOnlyHexView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_Edit.Create(m_hWnd, rcDefault, L"",
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_READONLY | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE,
		WS_EX_CLIENTEDGE);

	CFont font;
	font.CreatePointFont(110, L"Consolas");
	m_Edit.SetFont(font.Detach());

	return 0;
}

void CReadOnlyHexView::DisplayData() {
	CHARFORMAT fmt = { sizeof(fmt) };
	fmt.dwMask = CFM_COLOR;
	fmt.crTextColor = RGB(0, 0, 0);
	m_Edit.SetDefaultCharFormat(fmt);

	CWaitCursor wait;
	std::wstring text;
	int addrLen = m_data.size() < 0x10000 ? 4 : 8;
	auto size = (ULONG)m_data.size();
	for (ULONG i = 0; i < size; i += m_BytesPerLine) {
		text += addrLen == 4 ? std::format( L"{:04X} ", i) : std::format(L"{:08X} ", i);

		for (int x = 0; x < m_BytesPerLine; x += m_DataSize) {
			if (i + x + m_DataSize > m_data.size())
				text += std::wstring(m_DataSize * 2 + 1, L' ');
			else {
				switch (m_DataSize) {
					case 1: text += std::format(L"{:02X} ", m_data[i + x]); break;
					case 2: text += std::format(L"{:04X} ", *(WORD*)&m_data[i + x]); break;
					case 4: text += std::format(L"{:08X} ", *(DWORD*)&m_data[i + x]); break;
					case 8: text += std::format(L"{:016X} ", *(DWORD64*)&m_data[i + x]); break;
					default: ATLASSERT(false);
				}
			}
		}
		for (int x = 0; x < m_BytesPerLine; x++) {
			if (i + x + 1 > m_data.size())
				break;
			text += isprint(m_data[i + x]) ? (WCHAR)m_data[i + x] : L'.';
		}
		text += L"\n";
	}
	m_Edit.SetRedraw(FALSE);
	m_Edit.AppendText(text.c_str());
	m_Edit.HideSelection(true);
	int lines = m_Edit.GetLineCount();
	for (int i = 0; i < lines; i++) {
		auto ch = m_Edit.LineIndex(i);
		m_Edit.SetSel(ch, ch + addrLen);
		fmt.crTextColor = RGB(0, 0, 128);
		m_Edit.SetSelectionCharFormat(fmt);

		auto ch2 = ch + addrLen + 1 + (1 + m_DataSize * 2) * m_BytesPerLine / m_DataSize;
		m_Edit.SetSel(ch2, ch2 + m_BytesPerLine);
		fmt.crTextColor = RGB(0, 0, 255);
		m_Edit.SetSelectionCharFormat(fmt);
	}
	m_Edit.SetSelNone();
	m_Edit.HideSelection(false);
	m_Edit.SetRedraw();
}
