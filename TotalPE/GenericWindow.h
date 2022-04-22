#pragma once

template<typename T>
class CGenericFrame : public CFrameWindowImpl<CGenericFrame<T>> {
public:
	void OnFinalMessage(HWND) override {
		delete this;
	}

	void SetClient(T* client) {
		ATLASSERT(client);
		m_pClient = client;
		::SetParent(m_pClient->m_hWnd, this->m_hWnd);
		this->m_hWndClient = m_pClient->m_hWnd;
	}

	BEGIN_MSG_MAP(CGenericFrame<T>)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		if (m_pClient) {
			LRESULT result{ 0 };
			if (m_pClient->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, result))
				return true;
		}
		CHAIN_MSG_MAP(CFrameWindowImpl<CGenericFrame<T>>)
	END_MSG_MAP()

private:
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
		m_pClient = nullptr;
		return 0;
	}

private:
	T* m_pClient{ nullptr };
};
