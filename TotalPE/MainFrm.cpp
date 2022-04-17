// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "resource.h"
#include "MainFrm.h"
#include "View.h"
#include "AboutDlg.h"
#include "PEStrings.h"
#include <ToolbarHelper.h>

#ifdef _DEBUG
#pragma comment(lib, "../Lib/Liefd.lib")
#pragma comment(lib, "../WTLHelper/x64/Debug/WTLHelper.lib")
#else
#pragma comment(lib, "../Lib/Lief.lib")
#pragma comment(lib, "../WTLHelper/x64/Release/WTLHelper.lib")
#endif

CMainFrame::CMainFrame() : m_ViewMgr(this) {
}

void CMainFrame::OnTreeSelChanged(HWND tree, HTREEITEM hOld, HTREEITEM hNew) {
	auto type = GetItemData<TreeItemType>(m_Tree, hNew);
	auto hView = m_ViewMgr.CreateOrGetView(type, m_Splitter, *m_pe);
	if (hView) {
		if (m_CurrentView)
			::ShowWindow(m_CurrentView, SW_HIDE);
		::ShowWindow(hView, SW_SHOW);
		m_Splitter.SetSplitterPane(1, hView);
		m_CurrentView = hView;
	}
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) {
	return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle() {
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
}

CString const& CMainFrame::GetPEPath() const {
	return m_Path;
}

TreeItemType CMainFrame::TreeItemWithIndex(TreeItemType type, int index) {
	return static_cast<TreeItemType>((int)type + index);
}

CString CMainFrame::DoFileOpen() const {
	CSimpleFileDialog dlg(TRUE, nullptr, nullptr, OFN_EXPLORER | OFN_ENABLESIZING,
		L"PE Files\0*.exe;*.dll;*.efi;*.ocx;*.cpl;*.sys\0All Files\0*.*\0");
	return IDOK == dlg.DoModal() ? dlg.m_szFileName : L"";
}

void CMainFrame::InitPETree() {
	m_Tree.SetRedraw(FALSE);
	m_Tree.DeleteAllItems();
	CImageList images;
	images.Create(m_TreeIconSize, m_TreeIconSize, ILC_COLOR32, 10, 6);
	m_Tree.SetImageList(images, TVSIL_NORMAL);

	WORD icon = 0;
	auto hIcon = ::ExtractAssociatedIcon(_Module.GetModuleInstance(), (PWSTR)(PCWSTR)m_Path, &icon);
	if (!hIcon)
		hIcon = AtlLoadSysIcon(IDI_APPLICATION);
	images.AddIcon(hIcon);

	UINT icons[] = {
		IDI_SECTIONS, IDI_DIR_OPEN, IDI_RESOURCE, IDI_HEADER, IDI_DIR_CLOSED,
		IDI_SECTION, IDI_GLOBE,
		IDI_MANIFEST, IDI_VERSION, IDI_ICONS, IDI_CURSORS, IDI_DIALOGS,
		IDI_BITMAP, IDI_FONT, IDI_HTML,
	};

	for (auto icon : icons)
		images.AddIcon(AtlLoadIconImage(icon, 0, m_TreeIconSize, m_TreeIconSize));

	auto root = InsertTreeItem(m_Tree, m_Path.Mid(m_Path.ReverseFind(L'\\') + 1), 0, TreeItemType::Image);
	auto headers = InsertTreeItem(m_Tree, L"Headers", 4, TreeItemType::Headers, root);

	auto sections = InsertTreeItem(m_Tree, L"Sections", 1, TreeItemType::Sections, root);
	int index = 1;
	for (auto& section : m_pe->sections()) {
		InsertTreeItem(m_Tree, CString(section.name().c_str()), 6, TreeItemWithIndex(TreeItemType::Sections, index++), sections);
	}
	m_Tree.Expand(sections, TVE_EXPAND);

	auto dirs = InsertTreeItem(m_Tree, L"Directories", 5, 2, TreeItemType::Directories, root);
	index = 1;
	for (auto& dir : m_pe->data_directories()) {
		if (!dir.has_section())
			continue;
		InsertTreeItem(m_Tree, PEStrings::GetDataDirectoryName((int)dir.type()), 5, 2, TreeItemWithIndex(TreeItemType::Directories, index++), dirs);
	}
	m_Tree.Expand(dirs, TVE_EXPAND);
	if (m_pe->has_resources()) {
		auto resources = InsertTreeItem(m_Tree, L"Resources", 3, TreeItemType::Resources, root);
		ParseResources(resources);
		m_Tree.Expand(resources, TVE_EXPAND);
		//auto const rm = m_pe->resources_manager();
		//if (rm.has_manifest())
		//	InsertTreeItem(m_Tree, L"Manifest", 7, TreeItemType::Manifest, resources, TVI_SORT);
		//if (rm.has_version())
		//	InsertTreeItem(m_Tree, L"Version", 8, TreeItemType::Version, resources, TVI_SORT);

		//if (rm.has_icons()) {
		//	auto icons = InsertTreeItem(m_Tree, L"Icons", 9, TreeItemType::Icons, resources, TVI_SORT);
		//	index = 1;
		//	for (auto& icon : rm.icons()) {
		//		auto name = L"#" + std::to_wstring(icon.id());
		//		InsertTreeItem(m_Tree, name.c_str(), 9, TreeItemWithIndex(TreeItemType::Icons, index++), icons, TVI_SORT);
		//	}
		//}
		//if(rm.has_dialogs()) {
		//	auto dialogs = InsertTreeItem(m_Tree, L"Dialogs", 11, TreeItemType::Dialogs, resources, TVI_SORT);
		//	index = 1;
		//	for (auto& dlg : rm.dialogs()) {
		//		auto name = dlg.title().c_str();
		//		InsertTreeItem(m_Tree, (PCWSTR)name, 11, TreeItemWithIndex(TreeItemType::Icons, index++), dialogs, TVI_SORT);
		//	}
		//}

		//if (rm.has_string_table()) {
		//	auto tables = InsertTreeItem(m_Tree, L"String Table", 12, TreeItemType::StringTables, resources, TVI_SORT);
		//	index = 1;
		//	for (auto& st : rm.string_table()) {
		//		auto name = st.name().c_str();
		//		InsertTreeItem(m_Tree, (PCWSTR)name, 11, TreeItemWithIndex(TreeItemType::StringTables, index++), tables, TVI_SORT);
		//	}
		//}
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
	} commands[] = {
		{ ID_EDIT_COPY, IDI_COPY },
		{ ID_EDIT_PASTE, IDI_PASTE },
		{ ID_FILE_OPEN, IDI_FILE_OPEN },
	};

	for (auto& cmd : commands) {
		if (cmd.icon)
			AddCommand(cmd.id, cmd.icon);
		else
			AddCommand(cmd.id, cmd.hIcon);
	}
	AddMenu(GetMenu());
}

void CMainFrame::ParseResources(HTREEITEM hRoot) {
	auto node = m_pe->resources();
	ParseResources(hRoot, node);
}

int CMainFrame::ResourceTypeIconIndex(WORD type) {
	static const int indices[] = {
		-1, 3, 5, 2, -1, 4, -1,		// string table
		-1, 6, 6, -1, -1, 3,		// group cursor
		-1, 2, -1, 1, 4, -1, -1,	// P&P
		-1, -1, -1, 7, 0,			// manifest
	};
	return type >= _countof(indices) ? -1 : indices[type];
}

void CMainFrame::ParseResources(HTREEITEM hRoot, LIEF::PE::ResourceNode* node) {
	CString name;
	if (node->has_name())
		name = (PCWSTR)node->name().c_str();
	else
		name.Format(L"#%d", node->id());

	if (node->depth() > 0) {
		int icon = 3;
		if (node->depth() == 3)
			icon = 7;	// language
		if (node->depth() == 1 && !node->has_name()) {
			auto friendlyName = PEStrings::ResourceTypeToString(node->id());
			if (friendlyName) {
				name = friendlyName;
			}
		}
		if (node->depth() == 1) {
			icon = ResourceTypeIconIndex(node->id());
			icon = icon < 0 ? 3 : icon + 8;	// first icon index for resources
		}
		else if (node->depth() == 2) {
			int dummy;
			m_Tree.GetItemImage(hRoot, icon, dummy);
		}
		hRoot = InsertTreeItem(m_Tree, name, icon, TreeItemType::Resources, hRoot, TVI_SORT);
	}
	for (auto& child : node->childs())
		ParseResources(hRoot, &child);
}

bool CMainFrame::OpenPE(PCWSTR path) {
	CWaitCursor wait;
	auto pe = LIEF::PE::Parser::parse((PCSTR)CStringA(path));
	if (!pe) {
		AtlMessageBox(m_hWnd, L"Error parsing file", IDS_TITLE, MB_ICONERROR);
		return false;
	}

	m_pe = std::move(pe);
	m_Path = path;
	m_ViewMgr.Clear();
	InitPETree();
	return true;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	s_Frames++;
	CreateSimpleStatusBar();

	ToolBarButtonInfo const buttons[] = {
		{ ID_FILE_OPEN, IDI_FILE_OPEN },
		{ 0 },
		{ ID_EDIT_COPY, IDI_COPY },
	};
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	auto tb = ToolbarHelper::CreateAndInitToolBar(m_hWnd, buttons, _countof(buttons));

	AddSimpleReBarBand(tb);
	UIAddToolBar(tb);

	m_hWndClient = m_Splitter.Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_Tree.Create(m_Splitter, rcDefault, nullptr,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);

	m_Splitter.SetSplitterPane(0, m_Tree);
	m_Splitter.SetSplitterPosPct(20);

	InitMenu();
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != nullptr);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	if (--s_Frames == 0) {
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
	OpenPE(path);
	return 0;
}

LRESULT CMainFrame::OnFileOpenNewWindow(WORD, WORD, HWND, BOOL&) {
	auto path = DoFileOpen();
	auto frame = new CMainFrame;
	frame->CreateEx();
	if (!frame->OpenPE(path)) {
		frame->DestroyWindow();
		return 0;
	}
	frame->ShowWindow(SW_SHOWDEFAULT);
	return 0;
}
