#include "pch.h"
#include "ReadOnlyHexView.h"

void CReadOnlyHexView::SetData(std::vector<uint8_t> const& data) {
	m_data.SetData(0, data.data(), (ULONG)data.size());
	m_Hex.SetBufferManager(&m_data);
}

LRESULT CReadOnlyHexView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_Hex.Create(m_hWnd, rcDefault, nullptr,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | WS_HSCROLL,
		WS_EX_CLIENTEDGE);

	return 0;
}

