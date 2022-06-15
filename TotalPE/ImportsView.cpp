#include "pch.h"
#include "ImportsView.h"
#include "resource.h"
#include "PEStrings.h"
#include "SortHelper.h"

CString CImportsView::GetColumnText(HWND h, int row, int col) const {
	if (h == m_LibList) {
		auto& lib = m_ImportLibs[row];
		switch (col) {
			case 0: return lib.get_library_name().c_str();
			case 1: return std::to_wstring(lib.get_functions().size()).c_str();
			case 2: return lib.is_bound_library() ? L"Yes" : L"No";
		}
	}
	else {
		auto& fun = m_Imports[row];
		switch (col) {
			case 0: return fun.get_func_name().c_str();
			case 1: return std::to_wstring(fun.get_hint()).c_str();
			case 2: return PEStrings::UndecorateName(CString(fun.get_func_name().c_str())).c_str();
			case 3: return std::to_wstring(fun.get_ordinal()).c_str();
		}
	}
	return CString();
}

int CImportsView::GetRowImage(HWND h, int row, int) const {
	if (h == m_LibList) {
		return m_ImportLibs[row].get_library_name().starts_with("api-ms-") ? 1 : 0;
	}
	return 0;
}

void CImportsView::OnStateChanged(HWND h, int from, int to, DWORD oldState, DWORD newState) {
	if (h == m_LibList) {
		int n = m_LibList.GetSelectedIndex();
		if (n >= 0) {
			m_Imports = m_ImportLibs[n].get_functions();
			Sort(GetSortInfo(m_List));
			m_List.SetItemCount((int)m_Imports.size());
		}
		UpdateStatusText();
	}
}

void CImportsView::DoSort(SortInfo const* si) {
	if (si == nullptr)
		return;

	auto asc = si->SortAscending;
	if (si->hWnd == m_LibList) {
		auto compare = [&](auto& lib1, auto& lib2) {
			switch (si->SortColumn) {
				case 0: return SortHelper::Sort(lib1.get_library_name(), lib2.get_library_name(), asc);
				case 1: return SortHelper::Sort(lib1.get_functions().size(), lib2.get_functions().size(), asc);
				case 2: return SortHelper::Sort(lib1.is_bound_library(), lib2.is_bound_library(), asc);
			}
			return false;
		};
		std::sort(m_ImportLibs.begin(), m_ImportLibs.end(), compare);
	}
	else {
		auto compare = [&](auto& f1, auto& f2) {
			switch (si->SortColumn) {
				case 0: return SortHelper::Sort(f1.get_func_name(), f2.get_func_name(), asc);
				case 1: return SortHelper::Sort(f1.get_hint(), f2.get_hint(), asc);
				case 3: return SortHelper::Sort(f1.get_ordinal(), f2.get_ordinal(), asc);
			}
			return false;
		};
		m_Imports.Sort(compare);
	}
}

bool CImportsView::IsSortable(HWND h, int col) const {
	if (h == m_LibList)
		return true;

	return col != 2;
}

LRESULT CImportsView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
    m_hWndClient = m_Splitter.Create(m_hWnd, rcDefault, nullptr, WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE);

	m_List.Create(m_Splitter, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA | LVS_SHOWSELALWAYS);
	auto cm = GetColumnManager(m_List);

	CImageList images;
	images.Create(16, 16, ILC_COLOR32 | ILC_MASK, 4, 4);
	UINT icons[] = {
		IDI_FUNC_IMPORT, 
	};
	for (auto icon : icons)
		images.AddIcon(AtlLoadIconImage(icon, 0, 16, 16));
	m_List.SetImageList(images, LVSIL_SMALL);

	cm->AddColumn(L"Name", LVCFMT_LEFT, 300);
	cm->AddColumn(L"Hint", LVCFMT_RIGHT, 70);
	cm->AddColumn(L"Undecorated Name", LVCFMT_LEFT, 300);
	cm->AddColumn(L"Ordinal", LVCFMT_RIGHT, 70);
	cm->UpdateColumns();

	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	m_LibList.Create(m_Splitter, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA | LVS_SHOWSELALWAYS | LVS_SINGLESEL);
	cm = GetColumnManager(m_LibList);

	images.Detach();
	images.Create(16, 16, ILC_COLOR32 | ILC_MASK, 4, 4);
	UINT icons2[] = {
		IDI_IMPORTLIB, IDI_INTERFACE,
	};
	for (auto icon : icons2)
		images.AddIcon(AtlLoadIconImage(icon, 0, 16, 16));
	m_LibList.SetImageList(images, LVSIL_SMALL);

	cm->AddColumn(L"Name", LVCFMT_LEFT, 250);
	cm->AddColumn(L"Count", LVCFMT_RIGHT, 70);
	cm->AddColumn(L"Bound?", LVCFMT_LEFT, 70);
	cm->UpdateColumns();

	m_LibList.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

	BuildItems();

	m_Splitter.SetSplitterPanes(m_LibList, m_List);
	m_Splitter.SetSplitterPosPct(30);

    return 0;
}

LRESULT CImportsView::OnShowWindow(UINT, WPARAM, LPARAM, BOOL&) {
	UpdateStatusText();
	return 0;
}

LRESULT CImportsView::OnFilterChanged(WORD, WORD, HWND, BOOL&) {
	return LRESULT();
}

LRESULT CImportsView::OnExport(WORD, WORD, HWND, BOOL&) {
	return LRESULT();
}

LRESULT CImportsView::OnSetFocus(UINT, WPARAM, LPARAM, BOOL&) {
	m_LibList.SetFocus();
	return 0;
}

void CImportsView::BuildItems() {
	auto& imports = PE().get_imports();

	m_ImportLibs = imports.get_libraries();
	m_LibList.SetItemCount((int)m_ImportLibs.size());
}

void CImportsView::UpdateStatusText() {
	Frame()->SetStatusText(2, std::format(L"Libraries: {}", m_ImportLibs.size()).c_str());
	if(m_LibList.GetSelectedIndex() >= 0)
		Frame()->SetStatusText(3, std::format(L"Imported: {}", m_Imports.size()).c_str());
}

LRESULT CImportsView::OnListViewKeyDown(int /*idCtrl*/, LPNMHDR hdr, BOOL& /*bHandled*/) {
	auto lv = (NMLVKEYDOWN*)hdr;
	if (lv->wVKey == VK_TAB) {
		if (hdr->hwndFrom == m_LibList)
			m_List.SetFocus();
		else
			m_LibList.SetFocus();
	}
	return 0;
}

