#pragma once

enum class TreeItemType : size_t {
	None = 0,
	Image = 1 << 15,
	Directories = 1 << 16,
	Sections = 1 << 17,
	Headers = 1 << 18,
	Resources = 1 << 19,
	ResourceTypeName = 1 << 20,
	ResourceName,
	ResourceLnaguage,
	ContextMenu = 1 << 21,
	CLR = 1 << 22,
	Language = 1LL << 62,
	Resource = 1LL << 63,
};
DEFINE_ENUM_FLAG_OPERATORS(TreeItemType);

enum class TextFormat {
	Xml,
	Json,
	JScript,
	Plain,
};

struct IMainFrame abstract {
	virtual HWND GetHwnd() const = 0;
	virtual BOOL TrackPopupMenu(HMENU hMenu, DWORD flags, int x, int y) = 0;
	virtual CUpdateUIBase& GetUI() = 0;
	virtual bool AddToolBar(HWND tb) = 0;
	virtual void SetStatusText(int index, PCWSTR text) = 0;
	virtual CString const& GetPEPath() const = 0;
	virtual CString GetSelectedTreeItemPath() const = 0;
	virtual CString GetTreeItemText(int parents) const = 0;
	virtual HIMAGELIST GetTreeImageList() const = 0;
	virtual int GetResourceIconIndex(WORD type) const = 0;
	virtual bool GotoTreeItemResource(PCWSTR path) = 0;
};
