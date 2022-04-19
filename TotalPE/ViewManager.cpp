#include "pch.h"
#include "ViewManager.h"
#include "PEImageView.h"
#include "SectionsView.h"
#include "TextView.h"
#include "ReadOnlyHexView.h"

ViewManager::ViewManager(IMainFrame* frame) : m_pFrame(frame) {
    m_views.reserve(16);
}

HWND ViewManager::CreateOrGetView(TreeItemType type, HWND hParent, pe_image_full const& pe) {
    if (auto it = m_views.find(type); it != m_views.end())
        return it->second;

    HWND hView { nullptr };

    switch (type) {
        case TreeItemType::Image:
        {
            auto view = new CPEImageView(m_pFrame, type, pe);
            hView = view->DoCreate(hParent);
            break;
        }
        case TreeItemType::Sections:
        {
            auto view = new CSectionsView(m_pFrame, type, pe);
            hView = view->DoCreate(hParent);
            break;
        }
    }
    if (!hView) {
        if ((type & TreeItemType::Resource) == TreeItemType::Resource) {
            auto node = (pe_resource_directory_entry*)(DWORD_PTR(type) & ((1LL << 62) - 1));
            auto& data = node->get_data_entry().get_data();
            auto typeName = m_pFrame->GetTreeItemText(2);
            if (typeName == L"Manifest" || typeName == L"HTML") {
                CStringA xml((PCSTR)data.data(), (int)data.size());
                auto view = new CTextView(m_pFrame, type, pe);
                hView = view->DoCreate(hParent);
                view->SetText(CString(xml), TextFormat::Xml);
            }
            else if (typeName == L"Version") {
                pe_file_version_info info(data.data(), data.size());
                auto& fi = info.get_file_info();
            }
            else {
                auto view = new CReadOnlyHexView(m_pFrame, type, pe);
                hView = view->DoCreate(hParent);
                view->SetData(data);
            }
        }
    }

    if (hView)
        m_views.insert({ type, hView });
    return hView;
}

void ViewManager::Clear() {
    for (auto& [_, view] : m_views)
        ::DestroyWindow(view);
    m_views.clear();
}
