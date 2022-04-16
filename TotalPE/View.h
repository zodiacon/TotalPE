// View.h : interface of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Interfaces.h"

template<typename T>
class CView abstract : public CFrameWindowImpl<T, CWindow, CControlWinTraits> {
	using BaseFrame = CFrameWindowImpl<T, CWindow, CControlWinTraits>;
public:
	explicit CView(IMainFrame* frame, LIEF::PE::Binary const& pe) : m_pFrame(frame), m_pe(pe) {}
	IMainFrame* Frame() const {
		return m_pFrame;
	}
	auto& PE() {
		return m_pe;
	}

	void OnFinalMessage(HWND) override {
		delete this;
	}

	HWND DoCreate(HWND hParent) {
		auto p = static_cast<T*>(this);
		return p->Create(hParent, p->rcDefault, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS);
	}

	BEGIN_MSG_MAP(CView)
		CHAIN_MSG_MAP(BaseFrame)
	END_MSG_MAP()

private:
	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	IMainFrame* m_pFrame;
	LIEF::PE::Binary const& m_pe;
};

