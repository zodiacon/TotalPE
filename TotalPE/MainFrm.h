// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "View.h"
#include "Interfaces.h"
#include <OwnerDrawnMenu.h>
#include <TreeViewHelper.h>
#include "ViewManager.h"
#include "RecentFilesManager.h"
#include "AppSettings.h"

class CMainFrame :
	public CFrameWindowImpl<CMainFrame>,
	public CAutoUpdateUI<CMainFrame>,
	public COwnerDrawnMenu<CMainFrame>,
	public CTreeViewHelper<CMainFrame>,
	public CMessageFilter,
	public IMainFrame,
	public CIdleHandler {
public:
	DECLARE_FRAME_WND_CLASS(nullptr, IDR_MAINFRAME)

	CMainFrame();

	bool OpenPE(PCWSTR path);

	void OnTreeSelChanged(HWND tree, HTREEITEM hOld, HTREEITEM hNew);

	static int ResourceTypeIconIndex(WORD type);
	static int DirectoryToIconIndex(int dir);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	const UINT WM_CREATE_VIEW = WM_APP + 55;

	BEGIN_MSG_MAP(CMainFrame)
		NOTIFY_CODE_HANDLER(TVN_KEYDOWN, OnTreeKeyDown)
		MESSAGE_HANDLER(WM_CREATE_VIEW, OnCreateView)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER(ID_FILE_OPENINANEWWINDOW, OnFileOpenNewWindow)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_RANGE_HANDLER(ID_VIEW_EXPORTS, ID_VIEW_DEBUG, OnViewPEItem)
		COMMAND_ID_HANDLER(ID_FILE_CLOSE, OnFileClose)
		COMMAND_ID_HANDLER(ID_OPTIONS_ALWAYSONTOP, OnAlwaysOnTop)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_RANGE_HANDLER(ID_TREEICONSIZE_SMALL, ID_TREEICONSIZE_LARGE, OnChangeTreeIconSize)
		COMMAND_ID_HANDLER(ID_FILE_RUNASADMINISTRATOR, OnRunAsAdmin)
		COMMAND_RANGE_HANDLER(ATL_IDS_MRU_FILE, ATL_IDS_MRU_FILE + 29, OnRecentFile)
		if (uMsg == WM_COMMAND && m_CurrentView) {
			::SendMessage(m_CurrentView, WM_COMMAND, wParam, lParam);
		}
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP(CAutoUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		CHAIN_MSG_MAP(COwnerDrawnMenu<CMainFrame>)
		CHAIN_MSG_MAP(CTreeViewHelper<CMainFrame>)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

private:
	HWND GetHwnd() const override;
	BOOL TrackPopupMenu(HMENU hMenu, DWORD flags, int x, int y) override;
	CUpdateUIBase& GetUI() override;
	bool AddToolBar(HWND tb) override;
	void SetStatusText(int index, PCWSTR text) override;
	CString const& GetPEPath() const override;
	CString GetSelectedTreeItemPath() const override;
	CString GetTreeItemText(int parents) const override;
	HIMAGELIST GetTreeImageList() const override;
	int GetResourceIconIndex(WORD type) const override;
	bool GotoTreeItemResource(PCWSTR path) override;

	static TreeItemType TreeItemWithIndex(TreeItemType type, int index);
	CString DoFileOpen() const;

	void InitPETree();
	void InitMenu();
	void UpdateUI();
	void BuildTreeImageList();
	void ParseResources(HTREEITEM hRoot);
	void ParseResources(HTREEITEM hRoot, pe_resource_directory_entry const& node, int depth = 0);
	void UpdateRecentFilesMenu();
	void MakeAlwaysOnTop();

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreateView(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAlwaysOnTop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileOpenNewWindow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnChangeTreeIconSize(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTreeKeyDown(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnViewPEItem(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRunAsAdmin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRecentFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CSplitterWindow m_Splitter;
	CTreeViewCtrl m_Tree;
	CMultiPaneStatusBarCtrl m_StatusBar;
	CString m_Path;
	std::unique_ptr<pe_image_full> m_pe;
	ViewManager m_ViewMgr;
	HWND m_CurrentView{ nullptr };
	inline static int s_Frames{ 0 };
	inline static RecentFilesManager s_recentFiles;
	inline static AppSettings s_settings;
};
