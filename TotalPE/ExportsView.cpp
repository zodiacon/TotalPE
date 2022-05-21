#include "pch.h"
#include "ExportsView.h"
#include "resource.h"
#include <ToolbarHelper.h>
#include "ListViewhelper.h"
#include <SortHelper.h>
#include <ClipboardHelper.h>
#include "GenericWindow.h"
#include "AssemblyView.h"
#include <ThemeHelper.h>

CString CExportsView::GetColumnText(HWND, int row, int col) const {
	auto& exp = m_Exports[row];
	switch (col) {
		case 0: return exp.has_name() ? (CString)exp.get_func_name().c_str() : (CString)std::format(L"@{}", exp.get_ordinal()).c_str();
		case 1: return std::to_wstring(exp.get_ordinal()).c_str();
		case 2: return std::format(L"0x{:08X}", exp.get_rva()).c_str();
		case 3: return exp.is_forward() ? exp.get_forward_name().c_str() : "";
	}

	return CString();
}

int CExportsView::GetRowImage(HWND, int row, int) const {
	return m_Exports[row].is_forward() ? 1 : 0;
}

bool CExportsView::OnRightClickList(HWND, int row, int, CPoint const& pt) {
	if (row >= 0) {
		m_Selected = row;
		CMenu menu;
		menu.LoadMenu(IDR_CONTEXT);
		return Frame()->TrackPopupMenu(menu.GetSubMenu(2), 0, pt.x, pt.y);
	}
	return false;
}

void CExportsView::DoSort(SortInfo const* si) {
	auto asc = si->SortAscending;
	auto compare = [&](auto& exp1, auto& exp2) {
		switch (si->SortColumn) {
			case 0: return SortHelper::Sort(exp1.get_func_name(), exp2.get_func_name(), asc);
			case 1: return SortHelper::Sort(exp1.get_ordinal(), exp2.get_ordinal(), asc);
			case 2: return SortHelper::Sort(exp1.get_rva(), exp2.get_rva(), asc);
			case 3: return SortHelper::Sort(exp1.get_forward_name(), exp2.get_forward_name(), asc);
		}
		return false;
	};
	m_Exports.Sort(compare);
}

LRESULT CExportsView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	const UINT dummyId = 1;
	ToolBarButtonInfo const buttons[] = {
		{ ID_FILE_SAVE, IDI_SAVE, BTNS_BUTTON, L"Export" },
		{ 0 },
		{ dummyId, 0, BTNS_SEP | BTNS_SHOWTEXT },
	};
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	CToolBarCtrl tb = ToolbarHelper::CreateAndInitToolBar(m_hWnd, buttons, _countof(buttons), 16);
	tb.SetButtonInfo(dummyId, TBIF_SIZE, 0, 0, nullptr, -1, 200, 0, 0);
	tb.EnableButton(dummyId, FALSE);
	CRect rc(40, 0, 200, 20);
	tb.GetItemRect(2, &rc);

	m_QuickFind.Create(tb, rc, L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 123);
	m_QuickFind.SetLimitText(20);
	m_QuickFind.SetFont((HFONT)::SendMessage(m_hWndToolBar, WM_GETFONT, 0, 0));
	m_QuickFind.SetWatermark(L"Type to filter (Ctrl+Q)");
	m_QuickFind.SetHotKey(MOD_CONTROL, 'Q');

	AddSimpleReBarBand(tb);
	Frame()->AddToolBar(tb);
	CReBarCtrl rb(m_hWndToolBar);

	m_hWndClient = m_List.Create(*this, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA, WS_EX_CLIENTEDGE);
	auto cm = GetColumnManager(m_List);

	CImageList images;
	images.Create(16, 16, ILC_COLOR32 | ILC_MASK, 4, 4);
	UINT icons[] = {
		IDI_FUNCTION, IDI_FUNC_FORWARD
	};
	for (auto icon : icons)
		images.AddIcon(AtlLoadIconImage(icon, 0, 16, 16));
	m_List.SetImageList(images, LVSIL_SMALL);

	cm->AddColumn(L"Name", LVCFMT_LEFT, 250);
	cm->AddColumn(L"Ordinal", LVCFMT_RIGHT, 70);
	cm->AddColumn(L"Address", LVCFMT_RIGHT, 110);
	cm->AddColumn(L"Forwarded Name", LVCFMT_LEFT, 350);

	cm->UpdateColumns();

	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	BuildItems();

	return 0;
}

LRESULT CExportsView::OnShowWindow(UINT, WPARAM wp, LPARAM, BOOL&) {
	if (wp)
		UpdateStatusText();
	return 0;
}

LRESULT CExportsView::OnFilterChanged(WORD, WORD, HWND, BOOL&) {
	CString text;
	m_QuickFind.GetWindowText(text);
	ApplyFilter(text);

	return 0;
}

LRESULT CExportsView::OnExport(WORD, WORD, HWND, BOOL&) {
	CSimpleFileDialog dlg(FALSE, nullptr, nullptr, OFN_EXPLORER | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
		L"All Files\0*.*\0", m_hWnd);
	ThemeHelper::Suspend();
	if (IDOK == dlg.DoModal()) {
		ListViewHelper::SaveAll(dlg.m_szFileName, m_List, true);
	}
	ThemeHelper::Resume();
	return 0;
}

LRESULT CExportsView::OnCopy(WORD, WORD, HWND, BOOL&) {
	auto text = ListViewHelper::GetSelectedRowsAsString(m_List);
	if (!text.IsEmpty())
		ClipboardHelper::CopyText(m_hWnd, text);
	return 0;
}

LRESULT CExportsView::OnViewAssembly(WORD, WORD, HWND, BOOL&) {
	ATLASSERT(m_Selected >= 0);
	auto& exp = m_Exports[m_Selected];

	auto frame = new CGenericFrame<CAssemblyView>;
	frame->Create(nullptr, rcDefault, CString(exp.get_func_name().c_str()) + L" (Assembly)",
		WS_OVERLAPPEDWINDOW);
	auto hSmall = AtlLoadIconImage(IDI_CODE, 0, 16, 16);
	auto hBig = AtlLoadIconImage(IDI_CODE, 0, 32, 32);
	frame->SetIcon(hSmall, FALSE);
	frame->SetIcon(hBig, TRUE);
	auto view = new CAssemblyView(PE());
	view->Create(frame->m_hWnd, rcDefault, L"", WS_CHILD | WS_VISIBLE);
	pe_image_io io(PE().get_image());
	io.set_image_offset(exp.get_rva());
	std::vector<uint8_t> code;
	io.read(code, 0x400);		// hard-coded for now
	view->SetCode(exp.get_rva() + PE().get_image().get_image_base(), code);
	frame->SetClient(view);
	frame->ShowWindow(SW_SHOWDEFAULT);

	return 0;
}

void CExportsView::ApplyFilter(PCWSTR text) {
	CString filter(text);
	if (filter.IsEmpty())
		m_Exports.Filter(nullptr);
	else {
		filter.MakeLower();
		m_Exports.Filter([&](auto& item, int) {
			CString f(item.get_func_name().c_str());
			f.MakeLower();
			if (f.Find(filter) >= 0)
				return true;

			if (item.is_forward()) {
				f = item.get_forward_name().c_str();
				f.MakeLower();
				if (f.Find(filter) >= 0)
					return true;
			}
			return false;
			});
	}
	m_List.SetItemCountEx((int)m_Exports.size(), LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);
	m_List.RedrawItems(m_List.GetTopIndex(), m_List.GetTopIndex() + m_List.GetCountPerPage());
	UpdateStatusText();
}

void CExportsView::BuildItems() {
	auto& exports = PE().get_exports();
	m_Exports = exports.get_functions();
	m_List.SetItemCount((int)m_Exports.size());

	UpdateStatusText();
}

void CExportsView::UpdateStatusText() {
	Frame()->SetStatusText(2, std::format(L"{} Exports", m_Exports.size()).c_str());
	auto& exp = PE().get_exports();
	Frame()->SetStatusText(3, std::format(L"Names: {} Functions: {}", exp.get_number_of_names(), exp.get_number_of_functions()).c_str());
}
