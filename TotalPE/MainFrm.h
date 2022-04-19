// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "View.h"
#include "Interfaces.h"
#include <OwnerDrawnMenu.h>
#include <TreeViewHelper.h>
#include "ViewManager.h"

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

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	const UINT WM_CREATE_VIEW = WM_APP + 55;

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE_VIEW, OnCreateView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER(ID_FILE_OPENINANEWWINDOW, OnFileOpenNewWindow)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_CLOSE, OnFileClose)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_MSG_MAP(CAutoUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		CHAIN_MSG_MAP(COwnerDrawnMenu<CMainFrame>)
		CHAIN_MSG_MAP(CTreeViewHelper<CMainFrame>)
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

	static TreeItemType TreeItemWithIndex(TreeItemType type, int index);
	CString DoFileOpen() const;

	void InitPETree();
	void InitMenu();
	void ParseResources(HTREEITEM hRoot);
	void ParseResources(HTREEITEM hRoot, pe_resource_directory_entry const& node, int depth = 0);
	static int ResourceTypeIconIndex(WORD type);

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreateView(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileOpenNewWindow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CSplitterWindow m_Splitter;
	CTreeViewCtrl m_Tree;
	CString m_Path;
	int m_TreeIconSize{ 16 };
	std::unique_ptr<pe_image_full> m_pe;
	ViewManager m_ViewMgr;
	HWND m_CurrentView{ nullptr };
	inline static int s_Frames{ 0 };
};
