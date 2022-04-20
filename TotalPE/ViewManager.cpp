#include "pch.h"
#include "ViewManager.h"
#include "PEImageView.h"
#include "SectionsView.h"
#include "TextView.h"
#include "ReadOnlyHexView.h"
#include "ExportsView.h"
#include "ImportsView.h"
#include "VersionView.h"

ViewManager::ViewManager(IMainFrame* frame) : m_pFrame(frame) {
    m_views.reserve(16);
}

HWND ViewManager::CreateOrGetView(TreeItemType type, HWND hParent, pe_image_full const& pe) {
    if (auto it = m_views.find(type); it != m_views.end())
        return it->second;

    auto vtype = (DWORD_PTR)type;
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
    if (!hView && type > TreeItemType::Directories && vtype < (DWORD_PTR)TreeItemType::Directories + 16) {
        switch (vtype - (DWORD_PTR)TreeItemType::Directories - 1) {
            case IMAGE_DIRECTORY_ENTRY_EXPORT:
            {
                auto view = new CExportsView(m_pFrame, type, pe);
                hView = view->DoCreate(hParent);
                break;
            }

            case IMAGE_DIRECTORY_ENTRY_IMPORT:
            {
                auto view = new CImportsView(m_pFrame, type, pe);
                hView = view->DoCreate(hParent);
                break;
            }
        }
    }

    if (!hView && type > TreeItemType::Sections && DWORD_PTR(type) < (DWORD_PTR)TreeItemType::Sections + 999) {
        // section - show as hex for now
        auto view = new CReadOnlyHexView(m_pFrame, type, pe);
        hView = view->DoCreate(hParent);
        auto section = pe.get_image().get_sections()[DWORD_PTR(type) - DWORD_PTR(TreeItemType::Sections) - 1];
        view->SetData(section->get_section_data());
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
                auto view = new CVersionView(m_pFrame, type, pe);
                hView = view->DoCreate(hParent);
                view->SetData(data);
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
