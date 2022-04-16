#pragma once

enum class TreeItemType : uint32_t {
	Image = 1 << 15,
	Directories = 1 << 16,
	Sections = 1 << 17,
	Headers = 1 << 18,
	Resources = 1 << 19,
	CLR = 1 << 20,
};

struct IMainFrame abstract {
	virtual HWND GetHwnd() const = 0;
	virtual BOOL TrackPopupMenu(HMENU hMenu, DWORD flags, int x, int y) = 0;
	virtual CUpdateUIBase& GetUI() = 0;
	virtual bool AddToolBar(HWND tb) = 0;
	virtual void SetStatusText(int index, PCWSTR text) = 0;
	virtual CString const& GetPEPath() const = 0;
};
