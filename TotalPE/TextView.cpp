#include "pch.h"
#include "TextView.h"
#include "..\External\tinyxml2.h"

#ifdef _DEBUG
    #pragma comment(lib, "../External/tinyxml2d")
#else
    #pragma comment(lib, "../External/tinyxml2")
#endif

void CTextView::SetText(PCWSTR text, TextFormat format) {
    m_text = text;
    FormatText(m_text, format);
    m_Edit.SetWindowText(m_text);
}

LRESULT CTextView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
    m_hWndClient = m_Edit.Create(m_hWnd, rcDefault, L"", 
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | WS_HSCROLL |
        ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | ES_AUTOHSCROLL, WS_EX_CLIENTEDGE);

    CFont font;
    font.CreatePointFont(110, L"Consolas");
    m_Edit.SetFont(font.Detach());
    m_Edit.SetTabStops(10);

    return 0;
}

LRESULT CTextView::OnCopy(WORD, WORD, HWND, BOOL&) {
    m_Edit.Copy();
    return 0;
}

void CTextView::FormatText(CString& text, TextFormat format) {
    switch (format) {
        case TextFormat::Xml:
            FormatXml(text);
            break;
    }
}

void CTextView::FormatXml(CString& xml) {
    tinyxml2::XMLDocument doc;
    doc.Parse(CStringA(xml));
    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);
    doc.Print();
    if (!doc.Error()) {
        xml = printer.CStr();
        xml.Replace(L"\n", L"\r\n");
    }
}
