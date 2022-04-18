#include "pch.h"
#include "XMLView.h"

CXMLView::CXMLView(IMainFrame* frame, TreeItemType type, pe_image_full const& pe, CString const& xml) 
    : CView(frame, type, pe), m_xml(xml) {
}

LRESULT CXMLView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
    m_hWndClient = m_Edit.Create(m_hWnd, rcDefault, m_xml, 
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_MULTILINE | ES_READONLY, WS_EX_CLIENTEDGE);

    CFont font;
    font.CreatePointFont(110, L"Consolas");
    m_Edit.SetFont(font.Detach());

    return 0;
}
