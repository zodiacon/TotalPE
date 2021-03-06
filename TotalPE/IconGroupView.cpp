#include "pch.h"
#include "resource.h"
#include "IconGroupView.h"
#include "ResourceHelper.h"
#include "IconWriter.h"
#include <ThemeHelper.h>

void CIconGroupView::SetGroupIconData(std::vector<uint8_t> const& data) {
#pragma pack(push, 1)
	struct IconDirectoryEntry {
		BYTE  bWidth;
		BYTE  bHeight;
		BYTE  bColorCount;
		BYTE  bReserved;
		WORD  wPlanes;
		WORD  wBitCount;
		DWORD dwBytesInRes;
		WORD Id;
	};

	struct IconHeader {
		WORD Reserved;
		WORD Type;
		WORD Count;
		IconDirectoryEntry Entries[1];
	};
#pragma pack(pop)

	auto header = (IconHeader const*)data.data();
	ATLASSERT(header->Reserved == 0);
	bool isIcon = header->Type == 1;

	static std::vector<ResourceItem> icons, cursors;
	static pe_image_full const* pe;
	if ((isIcon && icons.empty()) || (!isIcon && cursors.empty()) || pe != &PE()) {
		pe = &PE();
		ResourceHelper rh(*pe);
		if (isIcon)
			icons = rh.GetFlatResources(L"Icon");
		else
			cursors = rh.GetFlatResources(L"Cursor");
	}
	auto& v = isIcon ? icons : cursors;
	int y = 10;
	for (int i = 0; i < header->Count; i++) {
		auto const& entry = header->Entries[i];
		auto id = std::format(L"#{}", entry.Id);
		if (auto it = std::ranges::find_if(v, [&](auto& e) { return e.Name == id; }); it != v.end()) {
			CIconHandle icon;
			auto& idata = it->Entry->get_data();
			auto width = entry.bWidth ? entry.bWidth : 256;
			icon.m_hIcon = ::CreateIconFromResourceEx(const_cast<PBYTE>(idata.data()), 
				(DWORD)idata.size(), isIcon, 0x30000, width, width, LR_DEFAULTCOLOR);
			if (icon) {
				IconData id;
				id.Icon = icon;
				id.Size = width;
				id.Id = entry.Id;
				id.Colors = entry.wBitCount * entry.wPlanes;
				m_Icons.emplace_back(std::move(id));
				y += width + 12;
			}
		}
	}
	SetScrollSize(450, y);
}

void CIconGroupView::SetIconData(std::vector<uint8_t> const& data, bool icon) {
	m_IconSize = *(int*)(data.data() + sizeof(DWORD));
	m_Icon = ::CreateIconFromResourceEx((PBYTE)data.data(), (DWORD)data.size(), icon, 0x30000, m_IconSize, m_IconSize, LR_DEFAULTCOLOR);
	SetScrollSize(500, 300);
}

LRESULT CIconGroupView::OnCreate(UINT, WPARAM, LPARAM, BOOL& handled) {
	handled = FALSE;
	return 0;
}

LRESULT CIconGroupView::OnContextMenu(UINT, WPARAM, LPARAM lp, BOOL&) {
	CPoint pt(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
	CPoint pt2(pt);
	ScreenToClient(&pt);
	CPoint offset;
	GetScrollOffset(offset);
	pt += offset;
	int i = 0;
	for (auto const& icon : m_Icons) {
		if (icon.Rect.PtInRect(pt)) {
			CMenu menu;
			menu.LoadMenu(IDR_CONTEXT);
			m_SelectedIcon = i;
			Frame()->TrackPopupMenu(menu.GetSubMenu(3), 0, pt2.x, pt2.y);
			break;
		}
		i++;
	}
	return 0;
}

LRESULT CIconGroupView::OnDestroy(UINT, WPARAM, LPARAM, BOOL&) {
	if(m_Icon)
		m_Icon.DestroyIcon();
	for (auto& icon : m_Icons)
		icon.Icon.DestroyIcon();

	return 0;
}

LRESULT CIconGroupView::OnEraseBkgnd(UINT, WPARAM wp, LPARAM, BOOL&) {
	//CDCHandle dc((HDC)wp);
	//dc.FillSolidRect(0, 0, 1000, 1000, 255);
	return 0;
}

LRESULT CIconGroupView::OnExportIcon(WORD, WORD, HWND, BOOL&) {
	ATLASSERT(m_SelectedIcon >= 0);
	CSimpleFileDialog dlg(FALSE, L"ico", nullptr, OFN_EXPLORER | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT,
		L"Icon Files (*.ico)\0*.ico\0All Files\0*.*\0", m_hWnd);
	ThemeHelper::Suspend();
	if (IDOK == dlg.DoModal()) {
		auto const& icon = m_Icons[m_SelectedIcon];
		if (!IconWriter::Save(dlg.m_szFileName, icon.Icon.m_hIcon, icon.Colors)) {
			AtlMessageBox(m_hWnd, L"Failed to save icon", IDS_TITLE, MB_ICONERROR);
		}
	}
	ThemeHelper::Resume();

	return 0;
}

void CIconGroupView::DoPaint(CDCHandle dc) {
	CFont font;
	font.CreatePointFont(110, L"Consolas");
	dc.SelectFont(font);
	dc.SetBkMode(TRANSPARENT);
	if (m_Icon) {
		dc.TextOutW(10, 10, std::format(L"{} X {}", m_IconSize, m_IconSize).c_str());
		dc.DrawIconEx(10, 50, m_Icon, m_IconSize, m_IconSize, 0, nullptr, DI_NORMAL);
	}
	else {
		int y = 10;
		for (auto& icon : m_Icons) {
			auto size = icon.Size;
			CRect rc(10, y, 160, y + size);
			auto text = std::format(L"{} X {} ({} bit)", size, size, icon.Colors);
			dc.DrawText(text.c_str(), 
				-1, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			icon.Icon.DrawIconEx(dc, 180, y, size, size, 0, nullptr, DI_NORMAL);
			icon.Rect = CRect(180, y, 180 + size, y + size);
			y += size + 12;
		}
	}
}
