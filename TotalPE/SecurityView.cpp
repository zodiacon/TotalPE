#include "pch.h"
#include "SecurityView.h"
#include "PEStrings.h"
#include <SortHelper.h>
#include <cryptuiapi.h>

typedef struct _WIN_CERTIFICATE {
	DWORD       dwLength;
	WORD        wRevision;
	WORD        wCertificateType;   // WIN_CERT_TYPE_xxx
	BYTE        bCertificate[ANYSIZE_ARRAY];
} WIN_CERTIFICATE, * LPWIN_CERTIFICATE;

#include <ImageHlp.h>

#pragma comment(lib, "Cryptui")
#pragma comment(lib, "Crypt32")
#pragma comment(lib, "imagehlp")

CString CSecurityView::GetColumnText(HWND, int row, int col) const {
	auto& item = m_Items[row];

	switch (col) {
		case 0: return PEStrings::CertificateTypeToString(item.get_certificate_type());
		case 1: return std::format(L"0x{:X}", item.get_revision()).c_str();
		case 2: return std::format(L"0x{:X} ({})", item.get_certificate_data().size(), item.get_certificate_data().size()).c_str();
	}
	return CString();
}

void CSecurityView::DoSort(SortInfo const* si) {
}

bool CSecurityView::OnDoubleClickList(HWND, int row, int, CPoint const& pt) {
	if (row < 0)
		return false;

	auto& item = m_Items[row];
	auto ctx = ::CertCreateCertificateContext(PKCS_7_ASN_ENCODING | X509_ASN_ENCODING, item.get_certificate_data().data(), (DWORD)item.get_certificate_data().size());
	if (ctx) {
		::CryptUIDlgViewContext(CERT_STORE_CERTIFICATE_CONTEXT, ctx, m_hWnd, nullptr, 0, nullptr);
		::CertFreeCertificateContext(ctx);
	}
	return true;
}

LRESULT CSecurityView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	m_hWndClient = m_List.Create(*this, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA, WS_EX_CLIENTEDGE);
	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	auto cm = GetColumnManager(m_List);
	cm->AddColumn(L"Certificate Type", LVCFMT_LEFT, 140);
	cm->AddColumn(L"Revision", LVCFMT_RIGHT, 80);
	cm->AddColumn(L"Data Size", LVCFMT_RIGHT, 100);
	cm->UpdateColumns();

	m_Items = PE().get_security().get_certificates();
	ATLASSERT(PE().get_security().get_certificates_count() == (int)m_Items.size());
	m_List.SetItemCount((int)m_Items.size());

	return 0;
}
