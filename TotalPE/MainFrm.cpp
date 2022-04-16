// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "resource.h"
#include "MainFrm.h"
#include "View.h"
#include "AboutDlg.h"
#include "PEStrings.h"

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
	if (hView)
		m_Splitter.SetSplitterPane(1, hView);
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
		IDI_SECTION,
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
	auto dirs = InsertTreeItem(m_Tree, L"Directories", 5, 2, TreeItemType::Directories, root);
	index = 1;
	for (auto& dir : m_pe->data_directories()) {
		if (!dir.has_section())
			continue;
		InsertTreeItem(m_Tree, PEStrings::GetDataDirectoryName((int)dir.type()), 5, 2, TreeItemWithIndex(TreeItemType::Directories, index++), dirs);
	}
	auto resources = InsertTreeItem(m_Tree, L"Resources", 3, TreeItemType::Resources, root);
	
	m_Tree.Expand(root, TVE_EXPAND);
	m_Tree.SelectItem(root);
	m_Tree.SetRedraw();
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	CreateSimpleStatusBar();

	m_hWndClient = m_Splitter.Create(m_hWnd, rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_Tree.Create(m_Splitter, rcDefault, nullptr,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
	
	m_Splitter.SetSplitterPane(0, m_Tree);
	m_Splitter.SetSplitterPosPct(20);

	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != nullptr);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != nullptr);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// TODO: add code to initialize document

	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
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
	CSimpleFileDialog dlg(TRUE, nullptr, nullptr, OFN_EXPLORER | OFN_ENABLESIZING,
		L"PE Files\0*.exe;*.dll;*.efi;*.ocx;*.cpl;*.sys\0All Files\0*.*\0");
	if (IDOK == dlg.DoModal()) {
		CWaitCursor wait;
		auto pe = LIEF::PE::Parser::parse((PCSTR)CStringA(dlg.m_szFileName));
		if (!pe) {
			AtlMessageBox(m_hWnd, L"Error parsing file", IDS_TITLE, MB_ICONERROR);
		}
		else {
			m_Path = dlg.m_szFileName;
			m_pe = std::move(pe);
			m_ViewMgr.Clear();
			InitPETree();
		}
	}
	return 0;
}
