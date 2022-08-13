
#include "pch.h"
#include "resource.h"
#include "MainFrm.h"
#include "View.h"
#include "AboutDlg.h"
#include "PEStrings.h"
#include <ToolbarHelper.h>
#include "SecurityHelper.h"
#include "IconHelper.h"
#include <ThemeHelper.h>

CMainFrame::CMainFrame() : m_ViewMgr(this) {
}

LRESULT CMainFrame::OnCreateView(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	auto ln = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
	CWaitCursor wait;
	for (int i = 2; i < 6; i++)
		SetStatusText(i, L"");
	auto hView = m_ViewMgr.CreateOrGetView((TreeItemType)wParam, m_Splitter, *m_pe);
	if (hView) {
		if (m_CurrentView)
			::ShowWindow(m_CurrentView, SW_HIDE);
		::ShowWindow(hView, SW_SHOW);
		m_Splitter.SetSplitterPane(1, hView);

		m_CurrentView = hView;
	}
	return 0;
}

void CMainFrame::OnTreeSelChanged(HWND tree, HTREEITEM hOld, HTREEITEM hNew) {
	auto type = GetItemData<TreeItemType>(m_Tree, hNew);
	PostMessage(WM_CREATE_VIEW, (WPARAM)type);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) {
	return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle() {
	UIUpdateToolBar();
	return FALSE;
}

HWND CMainFrame::GetHwnd() const {
	return m_hWnd;
}

BOOL CMainFrame::TrackPopupMenu(HMENU hMenu, DWORD flags, int x, int y) {
	return ShowContextMenu(hMenu, flags, x, y);
}

CUpdateUIBase& CMainFrame::GetUI() {
	return *this;
}

bool CMainFrame::AddToolBar(HWND tb) {
	return UIAddToolBar(tb);
}

void CMainFrame::SetStatusText(int index, PCWSTR text) {
	m_StatusBar.SetText(index, text);
}

CString const& CMainFrame::GetPEPath() const {
	return m_Path;
}

CString CMainFrame::GetSelectedTreeItemPath() const {
	return GetFullItemPath(m_Tree, m_Tree.GetSelectedItem());
}

CString CMainFrame::GetTreeItemText(int parents) const {
	auto hItem = m_Tree.GetSelectedItem();
	while (parents-- > 0)
		hItem = m_Tree.GetParentItem(hItem);

	CString text;
	m_Tree.GetItemText(hItem, text);
	return text;
}

HIMAGELIST CMainFrame::GetTreeImageList() const {
	return m_Tree.GetImageList(TVSIL_NORMAL).m_hImageList;
}

int CMainFrame::GetResourceIconIndex(WORD type) const {
	return ResourceTypeIconIndex(type);
}

bool CMainFrame::GotoTreeItemResource(PCWSTR path) {
	auto hRoot = FindChild(m_Tree, m_Tree.GetRootItem(), L"Resources");
	ATLASSERT(hRoot);
	auto hItem = FindItem(m_Tree, hRoot, path);
	ATLASSERT(hItem);
	if (hItem) {
		m_Tree.SelectItem(hItem);
		m_Tree.EnsureVisible(hItem);
		return true;
	}
	return false;
}

TreeItemType CMainFrame::TreeItemWithIndex(TreeItemType type, int index) {
	return static_cast<TreeItemType>((DWORD64)type + index);
}

CString CMainFrame::DoFileOpen() const {
	CSimpleFileDialog dlg(TRUE, nullptr, nullptr, OFN_EXPLORER | OFN_ENABLESIZING,
		L"PE Files\0*.exe;*.dll;*.efi;*.ocx;*.cpl;*.sys;*.mui;*.mun\0All Files\0*.*\0");
	ThemeHelper::Suspend();
	auto path = IDOK == dlg.DoModal() ? dlg.m_szFileName : L"";
	ThemeHelper::Resume();
	return path;
}

void CMainFrame::BuildTreeImageList() {
	CImageList images;
	int iconSize = s_settings.TreeIconSize() * 4 + 16;
	images.Create(iconSize, iconSize, ILC_COLOR32, 10, 6);
	m_Tree.SetImageList(images, TVSIL_NORMAL);

	WORD icon = 0;
	auto hIcon = ::ExtractAssociatedIcon(_Module.GetModuleInstance(), (PWSTR)(PCWSTR)m_Path, &icon);
	if (!hIcon)
		hIcon = AtlLoadSysIcon(IDI_APPLICATION);
	images.AddIcon(hIcon);

	UINT icons[] = {
		IDI_SECTIONS, IDI_DIR_OPEN, IDI_RESOURCE, IDI_HEADER, IDI_DIR_CLOSED,
		IDI_SECTION, IDI_GLOBE, IDI_EXPORT_DIR, IDI_IMPORT_DIR, IDI_DEBUG,
		IDI_DELAY_IMPORT, IDI_EXCEPTION, IDI_SHIELD2, IDI_RELOC,

		IDI_MANIFEST, IDI_VERSION, IDI_ICONS, IDI_CURSORS, IDI_DIALOGS,
		IDI_BITMAP, IDI_FONT, IDI_HTML, IDI_TEXT,
	};

	for (auto icon : icons)
		images.AddIcon(AtlLoadIconImage(icon, 0, iconSize, iconSize));
}

void CMainFrame::InitPETree() {
	m_Tree.SetRedraw(FALSE);
	m_Tree.DeleteAllItems();
	BuildTreeImageList();

	auto root = InsertTreeItem(m_Tree, m_Path.Mid(m_Path.ReverseFind(L'\\') + 1), 0, TreeItemType::Image);
	auto headers = InsertTreeItem(m_Tree, L"Headers", 4, TreeItemType::Headers, root);

	auto sections = InsertTreeItem(m_Tree, L"Sections", 1, TreeItemType::Sections, root);
	int index = 1;
	auto& image = m_pe->get_image();
	for (auto& section : image.get_sections()) {
		InsertTreeItem(m_Tree, CString(section->get_section_name().c_str()), 6, TreeItemWithIndex(TreeItemType::Sections, index++), sections);
	}
	m_Tree.Expand(sections, TVE_EXPAND);

	auto dirs = InsertTreeItem(m_Tree, L"Directories", 5, 2, TreeItemType::Directories, root);
	for (int i = 0; i < 16; i++) {
		if (!image.has_directory(i))
			continue;
		int icon = DirectoryToIconIndex(i);
		int selectedIcon = icon;
		if (icon < 0) {
			icon = 5;
			selectedIcon = 2;
		}
		InsertTreeItem(m_Tree, PEStrings::GetDataDirectoryName(i), icon, selectedIcon, TreeItemWithIndex(TreeItemType::Directories, i + 1), dirs);
	}
	m_Tree.Expand(dirs, TVE_EXPAND);
	if (image.has_directory(IMAGE_DIRECTORY_ENTRY_RESOURCE)) {
		auto resources = InsertTreeItem(m_Tree, L"Resources", 3, TreeItemType::Resources, root);
		ParseResources(resources);
		m_Tree.Expand(resources, TVE_EXPAND);
	}
	m_Tree.Expand(root, TVE_EXPAND);
	m_Tree.SelectItem(root);
	m_Tree.SetRedraw();
	m_Tree.SetFocus();
}

void CMainFrame::InitMenu() {
	struct {
		int id;
		UINT icon;
		HICON hIcon{ nullptr };
	} const commands[] = {
		{ ID_EDIT_COPY, IDI_COPY },
		{ ID_EDIT_PASTE, IDI_PASTE },
		{ ID_FILE_OPEN, IDI_FILE_OPEN },
		{ ID_VIEW_SECTIONS, IDI_SECTIONS },
		{ ID_VIEW_DIRECTORIES, IDI_DIR_OPEN },
		{ ID_VIEW_RESOURCES, IDI_RESOURCE },
		{ ID_VIEW_EXPORTS, IDI_EXPORT_DIR },
		{ ID_VIEW_IMPORTS, IDI_IMPORT_DIR },
		{ ID_VIEW_MANIFEST, IDI_MANIFEST },
		{ ID_VIEW_VERSION, IDI_VERSION },
		{ ID_VIEW_DEBUG, IDI_DEBUG},
		{ ID_FILE_RUNASADMINISTRATOR, 0, IconHelper::GetShieldIcon() },
	};
	for (auto& cmd : commands) {
		if (cmd.icon)
			AddCommand(cmd.id, cmd.icon);
		else
			AddCommand(cmd.id, cmd.hIcon);
	}
	AddMenu(GetMenu());
	UIAddMenu(GetMenu());
	UIAddMenu(IDR_CONTEXT);
}

void CMainFrame::UpdateUI() {
	UIEnable(ID_VIEW_EXPORTS, m_pe && m_pe->get_image().has_directory(IMAGE_DIRECTORY_ENTRY_EXPORT));
	UIEnable(ID_VIEW_IMPORTS, m_pe && m_pe->get_image().has_directory(IMAGE_DIRECTORY_ENTRY_IMPORT));
	UIEnable(ID_VIEW_RESOURCES, m_pe && m_pe->get_image().has_directory(IMAGE_DIRECTORY_ENTRY_RESOURCE));
	UIEnable(ID_VIEW_DEBUG, m_pe && m_pe->get_image().has_directory(IMAGE_DIRECTORY_ENTRY_DEBUG));
	UIEnable(ID_VIEW_SECTIONS, m_pe != nullptr);
	UIEnable(ID_VIEW_DIRECTORIES, m_pe != nullptr);
	UIEnable(ID_FILE_CLOSE, m_pe != nullptr);
	pe_resource_directory_entry const* dummy;
	UIEnable(ID_VIEW_MANIFEST, m_pe != nullptr && m_pe->get_resources().entry_by_id(dummy, 24));
	UIEnable(ID_VIEW_VERSION, m_pe != nullptr && m_pe->get_resources().entry_by_id(dummy, 16));
}

void CMainFrame::ParseResources(HTREEITEM hRoot) {
	auto& node = m_pe->get_resources();
	for (auto const& entry : node.get_entry_list()) {
		ParseResources(hRoot, entry);
	}
}

int CMainFrame::ResourceTypeIconIndex(WORD type) {
	static const int indices[] = {
		-1, 3, 5, 2, -1, 4, 8,		// string table
		-1, 6, 6, -1, -1, 3,		// group cursor
		-1, 2, -1, 1, 4, -1, -1,	// P&P
		-1, -1, -1, 7, 0,			// manifest
	};
	if (type >= _countof(indices))
		return 3;
	return indices[type] < 0 ? 3 : (indices[type] + 15);
}

int CMainFrame::DirectoryToIconIndex(int dir) {
	static const int icons[] = {
		8, 9, 3, 12,
		13, 14, 10, -1,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
	};
	return icons[dir];
}

void CMainFrame::ParseResources(HTREEITEM hRoot, pe_resource_directory_entry const& node, int depth) {
	std::wstring name;
	if (node.is_named())
		name = node.get_name();
	else
		name = std::format(L"#{}", node.get_id());

	TreeItemType type;
	int icon = 3;
	if (depth == 2) {
		icon = 7;	// language
		auto langname = PEStrings::LanguageToString(node.get_id());
		if (!langname.empty())
			name = std::move(langname);
	}
	if (depth == 0 && !node.is_named()) {
		auto friendlyName = PEStrings::ResourceTypeToString(node.get_id());
		if (!friendlyName.empty()) {
			name = std::move(friendlyName);
		}
	}
	if (depth == 0) {
		if (name.empty())
			name = std::format(L"#{}", node.get_id());
		type = !name.empty() ? TreeItemWithIndex(TreeItemType::ResourceTypeName, node.get_id()) : TreeItemWithIndex(TreeItemType::Resource, node.get_id());
		icon = ResourceTypeIconIndex(node.get_id());
	}
	else if (depth == 1) {
		type = TreeItemType::ResourceName;
		int dummy;
		m_Tree.GetItemImage(hRoot, icon, dummy);
		if (node.get_resource_directory().get_entry_list().size() == 1) {
			// add the only resource, no need for another level
			auto& child = node.get_resource_directory().get_entry_list()[0];
			name += L" (" + PEStrings::LanguageToString(child.get_id()) + L")";
			type = static_cast<TreeItemType>((DWORD_PTR)&child) | TreeItemType::Resource | TreeItemType::Language;
		}
	}
	else if (depth == 2) {
		ATLASSERT(node.is_includes_data());
		type = static_cast<TreeItemType>((DWORD_PTR)&node) | TreeItemType::Resource;
	}
	hRoot = InsertTreeItem(m_Tree, name.c_str(), icon, type, hRoot, TVI_SORT);
	if (depth == 1 && (type & TreeItemType::Language) == TreeItemType::Language)
		return;

	if (!node.is_includes_data()) {
		for (auto& child : node.get_resource_directory().get_entry_list())
			ParseResources(hRoot, child, depth + 1);
	}
}

void CMainFrame::UpdateRecentFilesMenu() {
	if (s_recentFiles.IsEmpty())
		return;

	auto menu = ((CMenuHandle)GetMenu()).GetSubMenu(0);
	CString text;
	int i = 0;
	for (; ; i++) {
		menu.GetMenuString(i, text, MF_BYPOSITION);
		if (text == L"&Recent Files")
			break;
	}
	menu = menu.GetSubMenu(i);
	while (menu.DeleteMenu(0, MF_BYPOSITION))
		;

	i = 0;
	for (auto& file : s_recentFiles.Files()) {
		menu.AppendMenu(MF_BYPOSITION, ATL_IDS_MRU_FILE + i++, file.c_str());
	}
	AddSubMenu(menu);
}

void CMainFrame::MakeAlwaysOnTop() {
	UISetCheck(ID_OPTIONS_ALWAYSONTOP, s_settings.AlwaysOnTop());
	SetWindowPos(s_settings.AlwaysOnTop() ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

bool CMainFrame::OpenPE(PCWSTR path) {
	CWaitCursor wait;
	auto pe = std::make_unique<pe_image_full>(std::string(CStringA(path)));
	if (pe->get_image().get_image_status() != pe_image_status::pe_image_status_ok) {
		AtlMessageBox(m_hWnd, L"Error parsing file", IDS_TITLE, MB_ICONERROR);
		return false;
	}

	m_pe = std::move(pe);
	m_Path = path;
	m_ViewMgr.Clear();
	InitPETree();
	UpdateUI();

	CString ftitle;
	ftitle.LoadString(IDR_MAINFRAME);
	CString title = m_Path.Mid(m_Path.ReverseFind(L'\\') + 1) + L" - " + ftitle;
	SetWindowText(title);
	s_recentFiles.AddFile(path);
	s_settings.RecentFiles(s_recentFiles.Files());
	UpdateRecentFilesMenu();

	return true;
}

void CMainFrame::InitDarkTheme() const {
	s_DarkTheme.BackColor = s_DarkTheme.SysColors[COLOR_WINDOW] = RGB(32, 32, 32);
	s_DarkTheme.TextColor = s_DarkTheme.SysColors[COLOR_WINDOWTEXT] = RGB(248, 248, 248);
	s_DarkTheme.SysColors[COLOR_HIGHLIGHT] = RGB(10, 10, 160);
	s_DarkTheme.SysColors[COLOR_HIGHLIGHTTEXT] = RGB(240, 240, 240);
	s_DarkTheme.SysColors[COLOR_MENUTEXT] = s_DarkTheme.TextColor;
	s_DarkTheme.SysColors[COLOR_CAPTIONTEXT] = s_DarkTheme.TextColor;
	s_DarkTheme.SysColors[COLOR_BTNFACE] = s_DarkTheme.BackColor;
	s_DarkTheme.SysColors[COLOR_BTNTEXT] = s_DarkTheme.TextColor;
	s_DarkTheme.SysColors[COLOR_3DLIGHT] = RGB(192, 192, 192);
	s_DarkTheme.SysColors[COLOR_BTNHIGHLIGHT] = RGB(192, 192, 192);
	s_DarkTheme.SysColors[COLOR_CAPTIONTEXT] = s_DarkTheme.TextColor;
	s_DarkTheme.SysColors[COLOR_3DSHADOW] = s_DarkTheme.TextColor;
	s_DarkTheme.SysColors[COLOR_SCROLLBAR] = s_DarkTheme.BackColor;
	s_DarkTheme.SysColors[COLOR_APPWORKSPACE] = s_DarkTheme.BackColor;
	s_DarkTheme.StatusBar.BackColor = RGB(16, 0, 16);
	s_DarkTheme.StatusBar.TextColor = s_DarkTheme.TextColor;

	s_DarkTheme.Name = L"Dark";
	s_DarkTheme.Menu.BackColor = s_DarkTheme.BackColor;
	s_DarkTheme.Menu.TextColor = s_DarkTheme.TextColor;
}


LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	DragAcceptFiles();
	s_Frames++;
	if (s_Frames == 1) {
		InitDarkTheme();
		if (s_settings.LoadFromKey(L"SOFTWARE\\ScorpioSoftware\\TotalPE")) {
			s_recentFiles.Set(s_settings.RecentFiles());
		}
	}
	CreateSimpleStatusBar();
	m_StatusBar.SubclassWindow(m_hWndStatusBar);
	int parts[] = { 200, 400, 600, 800, 1000 };
	m_StatusBar.SetParts(_countof(parts), parts);

	ToolBarButtonInfo const buttons[] = {
		{ ID_FILE_OPEN, IDI_FILE_OPEN },
		{ 0 },
		{ ID_EDIT_COPY, IDI_COPY },
		{ 0 },
		{ ID_VIEW_SECTIONS, IDI_SECTIONS },
		{ ID_VIEW_DIRECTORIES, IDI_DIR_OPEN },
		{ ID_VIEW_RESOURCES, IDI_RESOURCE },
		{ ID_VIEW_EXPORTS, IDI_EXPORT_DIR },
		{ ID_VIEW_IMPORTS, IDI_IMPORT_DIR },
		{ ID_VIEW_MANIFEST, IDI_MANIFEST },
		{ ID_VIEW_VERSION, IDI_VERSION },
		{ ID_VIEW_DEBUG, IDI_DEBUG},
	};
	CMenuHandle hMenu = GetMenu();
	if (SecurityHelper::IsRunningElevated()) {
		hMenu.GetSubMenu(0).DeleteMenu(ID_FILE_RUNASADMINISTRATOR, MF_BYCOMMAND);
		hMenu.GetSubMenu(0).DeleteMenu(0, MF_BYPOSITION);
		CString text;
		GetWindowText(text);
		SetWindowText(text + L" (Administrator)");
	}

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	auto tb = ToolbarHelper::CreateAndInitToolBar(m_hWnd, buttons, _countof(buttons));

	AddSimpleReBarBand(tb);
	UIAddToolBar(tb);

	m_hWndClient = m_Splitter.Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	m_Tree.Create(m_Splitter, rcDefault, nullptr,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TVS_HASBUTTONS | TVS_HASLINES | TVS_SHOWSELALWAYS);
	m_Tree.SetExtendedStyle(TVS_EX_DOUBLEBUFFER | TVS_EX_RICHTOOLTIP, 0);

	m_Splitter.SetSplitterPane(0, m_Tree);
	m_Splitter.SetSplitterPosPct(15);

	InitMenu();

	UISetCheck(ID_VIEW_STATUS_BAR, 1);
	UIEnable(ID_FILE_CLOSE, false);

	SetCheckIcon(AtlLoadIconImage(IDI_CHECK, 0, 16, 16), AtlLoadIconImage(IDI_RADIO, 0, 16, 16));
	UISetRadioMenuItem(ID_TREEICONSIZE_SMALL + s_settings.TreeIconSize(), ID_TREEICONSIZE_SMALL, ID_TREEICONSIZE_LARGE);

	UpdateUI();
	UpdateRecentFilesMenu();

	if (s_settings.DarkMode()) {
		ThemeHelper::SetCurrentTheme(s_DarkTheme, m_hWnd);
		ThemeHelper::UpdateMenuColors(*this, true);
		UpdateMenu(GetMenu(), true);
		DrawMenuBar();
	}
	MakeAlwaysOnTop();
	UISetCheck(ID_OPTIONS_DARKMODE, s_settings.DarkMode());

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != nullptr);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	if (--s_Frames == 0) {
		s_settings.Save();
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != nullptr);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);
		bHandled = FALSE;
	}
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnAlwaysOnTop(WORD, WORD, HWND, BOOL&) {
	s_settings.AlwaysOnTop(!s_settings.AlwaysOnTop());
	MakeAlwaysOnTop();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	auto bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnFileOpen(WORD, WORD, HWND, BOOL&) {
	auto path = DoFileOpen();
	if (path.IsEmpty())
		return 0;

	OpenPE(path);
	return 0;
}

LRESULT CMainFrame::OnFileOpenNewWindow(WORD, WORD, HWND, BOOL&) {
	auto path = DoFileOpen();
	if (path.IsEmpty())
		return 0;

	auto frame = new CMainFrame;
	frame->CreateEx();
	if (!frame->OpenPE(path)) {
		frame->DestroyWindow();
		return 0;
	}
	frame->ShowWindow(SW_SHOWDEFAULT);
	return 0;
}

LRESULT CMainFrame::OnFileClose(WORD, WORD, HWND, BOOL&) {
	if (m_pe) {
		m_pe.reset();
		m_ViewMgr.Clear();
		m_Tree.DeleteAllItems();
		m_Splitter.SetSplitterPane(1, nullptr);
		UpdateUI();
	}
	return 0;
}

LRESULT CMainFrame::OnChangeTreeIconSize(WORD, WORD id, HWND, BOOL&) {
	s_settings.TreeIconSize(id - ID_TREEICONSIZE_SMALL);
	BuildTreeImageList();
	m_Tree.RedrawWindow();
	UISetRadioMenuItem(id, ID_TREEICONSIZE_SMALL, ID_TREEICONSIZE_LARGE);

	return 0;
}

LRESULT CMainFrame::OnTreeKeyDown(int /*idCtrl*/, LPNMHDR hdr, BOOL& /*bHandled*/) {
	auto tv = (NMTVKEYDOWN*)hdr;
	if (m_CurrentView && tv->wVKey == VK_TAB) {
		::SetFocus(m_CurrentView);
		return 1;
	}
	return 0;
}

LRESULT CMainFrame::OnViewPEItem(WORD, WORD id, HWND, BOOL&) {
	ATLASSERT(m_pe);
	auto item = id - ID_VIEW_EXPORTS;
	static const TreeItemType items[] = {
		TreeItemWithIndex(TreeItemType::Directories, IMAGE_DIRECTORY_ENTRY_EXPORT + 1),
		TreeItemWithIndex(TreeItemType::Directories, IMAGE_DIRECTORY_ENTRY_IMPORT + 1),
		TreeItemType::Sections,
		TreeItemType::Directories,
		TreeItemType::Resources,
		TreeItemWithIndex(TreeItemType::ResourceTypeName, 24),
		TreeItemWithIndex(TreeItemType::ResourceTypeName, 16),
		TreeItemWithIndex(TreeItemType::Directories, IMAGE_DIRECTORY_ENTRY_DEBUG + 1),
	};
	auto hItem = FindItemByData(m_Tree, m_Tree.GetRootItem(), items[item]);
	if (hItem) {
		if ((items[item] & TreeItemType::ResourceTypeName) == TreeItemType::ResourceTypeName) {
			auto hChild = m_Tree.GetChildItem(hItem);
			if (hChild)
				hItem = hChild;
		}
		m_Tree.SelectItem(hItem);
		m_Tree.EnsureVisible(hItem);
	}
	return 0;
}

LRESULT CMainFrame::OnRunAsAdmin(WORD, WORD, HWND, BOOL&) {
	if (SecurityHelper::RunElevated(nullptr, true)) {
		s_Frames = 1;
		SendMessage(WM_CLOSE);
	}

	return 0;
}

LRESULT CMainFrame::OnRecentFile(WORD, WORD id, HWND, BOOL&) {
	OpenPE(s_recentFiles.Files()[id - ATL_IDS_MRU_FILE].c_str());
	return 0;
}

LRESULT CMainFrame::OnDropFiles(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	auto hDrop = (HDROP)wParam;
	WCHAR path[MAX_PATH];
	if (::DragQueryFile(hDrop, 0, path, _countof(path)))
		OpenPE(path);
	::DragFinish(hDrop);
	return 0;
}

LRESULT CMainFrame::OnToggleDarkMode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	s_settings.DarkMode(!s_settings.DarkMode());
	if (s_settings.DarkMode())
		ThemeHelper::SetCurrentTheme(s_DarkTheme, m_hWnd);
	else
		ThemeHelper::SetDefaultTheme(m_hWnd);
	ThemeHelper::UpdateMenuColors(*this, s_settings.DarkMode());
	UpdateMenuBase(GetMenu(), true);
	DrawMenuBar();
	UISetCheck(ID_OPTIONS_DARKMODE, s_settings.DarkMode());

	return 0;
}

TreeItemType CMainFrame::RegisterContextMenu(TreeItemType type, UINT menuId, UINT submenuIndex) {
	type |= TreeItemType::ContextMenu | (TreeItemType)((menuId << 8) | submenuIndex);
	return type;
}

LRESULT CMainFrame::OnTreeRightClick(HWND tree, HTREEITEM hItem, CPoint const& pt) {
	auto data = GetItemData<TreeItemType>(m_Tree, hItem);
	if ((data & TreeItemType::ContextMenu) == TreeItemType::None)
		return 0;

	data &= ~TreeItemType::ContextMenu;
	auto index = (int)data & 0xff;
	auto id = (int)data >> 8;
	CMenu menu;
	ATLVERIFY(menu.LoadMenu(id));
	ShowContextMenu(menu.GetSubMenu(index), 0, pt.x, pt.y);

	return 1;
}


