#include "pch.h"
#include "IconGroupView.h"
#include "ResourceHelper.h"

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
		WORD          Reserved;
		WORD          Type;
		WORD          Count;
		IconDirectoryEntry  Entries[1];
	};
#pragma pack(pop)

	auto header = (IconHeader const*)data.data();
	ATLASSERT(header->Reserved == 0);
	ATLASSERT(header->Type == 1);

	static std::vector<ResourceItem> icons;
	static pe_image_full const* pe;
	if (icons.empty() || pe != &PE()) {
		pe = &PE();
		ResourceHelper rh(*pe);
		icons = rh.GetFlatResources(L"Icon");
	}
	for (int i = 0; i < header->Count; i++) {
		auto const& entry = header->Entries[i];
		auto id = std::format(L"#{}", entry.Id);
		if (auto it = std::find_if(icons.begin(), icons.end(), [&](auto& e) { return e.Name == id; }); it != icons.end()) {
			CIconHandle icon;
			auto& idata = it->Entry->get_data();
			icon.CreateIconFromResourceEx((PBYTE)idata.data(), (DWORD)idata.size(), 0x30000, entry.bWidth, entry.bHeight, LR_DEFAULTCOLOR);
			if (icon)
				m_Icons.emplace_back(icon, entry.bWidth);
		}

	}
	Invalidate();
}

void CIconGroupView::SetIconData(std::vector<uint8_t> const& data, bool icon) {
	m_IconSize = *(int*)(data.data() + sizeof(DWORD));
	m_Icon.CreateIconFromResourceEx((PBYTE)data.data(), (DWORD)data.size(), 0x30000, m_IconSize, m_IconSize, LR_DEFAULTCOLOR);
	Invalidate();
}

LRESULT CIconGroupView::OnCreate(UINT, WPARAM, LPARAM, BOOL&) {
	return LRESULT();
}

LRESULT CIconGroupView::OnDestroy(UINT, WPARAM, LPARAM, BOOL&) {
	if(m_Icon)
		m_Icon.DestroyIcon();
	for (auto& [icon, _] : m_Icons)
		icon.DestroyIcon();

	return 0;
}

LRESULT CIconGroupView::OnPaint(UINT, WPARAM, LPARAM, BOOL&) {
	CPaintDC dc(m_hWnd);
	CFont font;
	font.CreatePointFont(110, L"Consolas");
	dc.SelectFont(font);
	if (m_Icon) {
		dc.TextOutW(10, 10, std::format(L"{} X {}", m_IconSize, m_IconSize).c_str());
		dc.DrawIconEx(10, 50, m_Icon, m_IconSize, m_IconSize, 0, nullptr, DI_NORMAL);
	}
	else {
		int y = 10;
		for (auto& [icon, size] : m_Icons) {
			CRect rc(10, y, 90, y + size);
			dc.DrawText(std::format(L"{} X {}", size, size).c_str(), -1, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			dc.DrawIconEx(100, y, icon, size, size, 0, nullptr, DI_NORMAL);
			y += size + 12;
		}
	}
	return 0;
}
