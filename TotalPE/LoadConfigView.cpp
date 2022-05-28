#include "pch.h"
#include "LoadConfigView.h"
#include "PEStrings.h"
#include "SortHelper.h"
#include "resource.h"

CString CLoadConfigView::GetColumnText(HWND h, int row, int col) const {
	if (h == m_List) {
		auto& item = m_Items[row];
		switch (col) {
			case 0: return item.Name.c_str();
			case 1: return item.Value.c_str();
			case 2: return item.Details.c_str();
		}
	}
	else {
		auto& item = m_CfgFunctions[row];
		switch (col) {
			case 0: return item.Name.c_str();
			case 1: return std::format(L"0x{:08X}", item.Rva).c_str();
		}
	}
	return L"";
}

void CLoadConfigView::DoSort(SortInfo const* si) {
	if (si == nullptr)
		return;

	if (si->hWnd == m_List) {
		auto compare = [&](auto& item1, auto& item2) {
			return SortHelper::Sort(item1.Name, item2.Name, si->SortAscending);
		};
		std::ranges::sort(m_Items, compare);
	}
	else {
		auto compare = [&](auto& f1, auto& f2) {
			switch (si->SortColumn) {
				case 0:	return SortHelper::Sort(f1.Name, f2.Name, si->SortAscending);
				case 1:	return SortHelper::Sort(f1.Rva, f2.Rva, si->SortAscending);
			}
			return false;
		};
		std::ranges::sort(m_CfgFunctions, compare);
	}
}

bool CLoadConfigView::IsSortable(HWND h, int col) const {
	if (h == m_List)
		return col == 0;		// sort on Name column only
	return true;
}

int CLoadConfigView::GetRowImage(HWND hList, int row, int col) const {
	if (hList == m_List)
		return -1;

	return m_CfgFunctions[row].Export ? 1 : 0;
}

LRESULT CLoadConfigView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	m_hWndClient = m_Splitter.Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS);
	m_List.Create(m_Splitter, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA, WS_EX_CLIENTEDGE);
	m_List.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	auto cm = GetColumnManager(m_List);
	cm->AddColumn(L"Name", LVCFMT_LEFT, 180);
	cm->AddColumn(L"Value", LVCFMT_LEFT, 200);
	cm->AddColumn(L"Details", LVCFMT_LEFT, 550);
	cm->UpdateColumns();

	m_CfgList.Create(m_Splitter, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
		LVS_REPORT | LVS_OWNERDATA, WS_EX_CLIENTEDGE);
	m_CfgList.SetExtendedListViewStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	cm = GetColumnManager(m_CfgList);
	cm->AddColumn(L"Function Name", LVCFMT_LEFT, 220);
	cm->AddColumn(L"Address", LVCFMT_RIGHT, 100);
	cm->UpdateColumns();
	CImageList images;
	images.Create(16, 16, ILC_COLOR32 | ILC_MASK, 2, 2);
	images.AddIcon(AtlLoadIconImage(IDI_FUNCTION));
	images.AddIcon(AtlLoadIconImage(IDI_FUNC_IMPORT));
	m_CfgList.SetImageList(images, LVSIL_SMALL);

	BuildItems();
	m_Splitter.SetSplitterPanes(m_List, m_CfgList);
	if (m_CfgFunctions.empty())
		m_Splitter.SetSinglePaneMode(0);
	else
		m_Splitter.SetSplitterPosPct(70);

	return 0;
}

void CLoadConfigView::BuildItems() {
	auto baseAddress = m_symbols.LoadSymbolsForModule(CStringA(Frame()->GetPEPath()));
	auto& lc = PE().get_load_config();
	auto cfg = lc.get_code_integrity();

	m_Items = std::vector<DataItem>{
		{ L"Time Stamp", std::format(L"0x{:X}", lc.get_timestamp()) },
		{ L"Size", std::format(L"0x{:X}", lc.get_size()) },
		{ L"Version", std::format(L"{}.{}", lc.get_major_version(), lc.get_minor_version()) },
		{ L"Process Affinity Mask", std::format(L"0x{:X}", lc.get_process_affinity_mask()) },
		{ L"Guard Flags", std::format(L"0x{:X}", lc.get_guard_flags()), PEStrings::CFGFlagsToString(lc.get_guard_flags()) },
		{ L"Heap Flags", std::format(L"0x{:X}", lc.get_process_heap_flags()) },
		{ L"Virtual Memory Threshold", std::format(L"0x{:X}", lc.get_virtual_memory_threshold()) },
		{ L"Global Flags Set", std::format(L"0x{:X}", lc.get_global_flagsset()) },
		{ L"Global Flags Clear", std::format(L"0x{:X}", lc.get_global_flagsclear()) },
		{ L"Lock Prefix Table", std::format(L"0x{:X}", lc.get_lock_prefix_table()) },
		{ L"Default CS Timeout", std::format(L"0x{:X}", lc.get_criticalsection_default_timeout()) },
		{ L"Dependent Load Flags", std::format(L"0x{:X}", lc.get_dependent_load_flags()) },
		{ L"Security Cookie", std::format(L"0x{:X}", lc.get_security_cookie()) },
		{ L"CSD Version", std::format(L"0x{:X}", lc.get_csd_version()) },
		{ L"CFG Functions", std::format(L"{}", lc.get_guard_cf_function_count()) },
	};

	m_CfgFunctions.reserve(lc.get_guard_cf_function_count());
	auto const& exports = PE().get_exports().get_functions();
	for (auto const& f : lc.get_guard_cf_functions()) {
		CfgFunction func;
		func.Rva = f;
		auto rva = PE().get_image().va_to_rva(f);
		if (auto it = std::ranges::find_if(exports, [&](auto& exp) {
			return exp.get_rva() == f;
			}); it != exports.end()) {
			func.Name = it->get_func_name();
			func.Export = true;
		}
		else {
			func.Export = false;
			if (baseAddress) {
				DWORD64 offset = 0;
				auto sym = m_symbols.GetSymbolFromAddress(f + baseAddress, &offset);
				if (sym && offset == 0)
					func.Name = sym->GetSymbolInfo()->Name;
			}
		}
		m_CfgFunctions.emplace_back(std::move(func));
	}
	m_List.SetItemCount((int)m_Items.size());
	m_CfgList.SetItemCount((int)m_CfgFunctions.size());
}

