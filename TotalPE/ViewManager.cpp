#include "pch.h"
#include "ViewManager.h"
#include "PEImageView.h"
#include "SectionsView.h"
#include "TextView.h"
#include "ReadOnlyHexView.h"
#include "ExportsView.h"
#include "ImportsView.h"
#include "VersionView.h"
#include "DirectoriesView.h"
#include "ResourcesView.h"
#include "DebugView.h"
#include "MessageTableView.h"
#include "SecurityView.h"
#include "RelocationsView.h"
#include "ExceptionsView.h"
#include "LoadConfigView.h"
#include "ResourceHelper.h"
#include "AcceleratorTableView.h"
#include "IconGroupView.h"
#include "BitmapView.h"
#include "AssemblyView.h"
#include "GenericWindow.h"
#include "MenuView.h"
#include "SymbolsView.h"

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
            auto view = new CPEImageView(m_pFrame, pe);
            hView = view->DoCreate(hParent);
            break;
        }
        case TreeItemType::Headers:
        {
            auto view = new CReadOnlyHexView(m_pFrame);
            hView = view->DoCreate(hParent);
            view->SetData(pe.get_image().get_headers_data());
            break;
        }
        case TreeItemType::Sections:
        {
            auto view = new CSectionsView(m_pFrame, pe);
            hView = view->DoCreate(hParent);
            break;
        }
        case TreeItemType::Directories:
        {
            auto view = new CDirectoriesView(m_pFrame, pe);
            hView = view->DoCreate(hParent);
            break;
        }
        case TreeItemType::Resources:
        {
            auto view = new CResourcesView(m_pFrame, pe);
            hView = view->DoCreate(hParent);
            break;
        }
    }
    if (!hView && type > TreeItemType::Directories && vtype < (DWORD_PTR)TreeItemType::Directories + 16) {
        auto index = uint32_t(vtype - (DWORD_PTR)TreeItemType::Directories - 1);
        switch (index) {
            case IMAGE_DIRECTORY_ENTRY_EXPORT:
            {
                auto view = new CExportsView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                break;
            }

            case IMAGE_DIRECTORY_ENTRY_IMPORT:
            {
                auto view = new CImportsView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                break;
            }

            case IMAGE_DIRECTORY_ENTRY_DEBUG:
            {
                auto view = new CDebugView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                break;
            }

            case IMAGE_DIRECTORY_ENTRY_SECURITY:
            {
                auto view = new CSecurityView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                break;
            }

            case IMAGE_DIRECTORY_ENTRY_BASERELOC:
            {
                auto view = new CRelocationsView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                break;
            }

            case IMAGE_DIRECTORY_ENTRY_EXCEPTION:
            {
                auto view = new CExceptionsView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                break;
            }

            case IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG:
            {
                auto view = new CLoadConfigView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                break;
            }

            default:
                // all other directories...

                auto view = new CReadOnlyHexView(m_pFrame);
                hView = view->DoCreate(hParent);
                pe_image_io io(pe.get_image());
                auto address = pe.get_image().get_directory_virtual_address(index);
                io.set_image_offset(address);
                std::vector<uint8_t> data;
                io.read(data, pe.get_image().get_directory_virtual_size(index));               
                view->SetData(data);
                view->SetAddress(address);
                break;
        }
    }
    if (!hView && type >= TreeItemType::Symbols && vtype < (DWORD_PTR)TreeItemType::Symbols + 256) {
        auto view = new CSymbolsView(m_pFrame);
        hView = view->DoCreate(hParent);
        view->BuildItems(m_pFrame->GetSymbols(), SymbolTag(vtype & 0xff));
    }

    if (!hView && type > TreeItemType::Sections && DWORD_PTR(type) < (DWORD_PTR)TreeItemType::Sections + 999) {
        // section - show as hex for now
        auto view = new CReadOnlyHexView(m_pFrame);
        hView = view->DoCreate(hParent);
        auto section = pe.get_image().get_sections()[DWORD_PTR(type) - DWORD_PTR(TreeItemType::Sections) - 1];
        view->SetData(section->get_section_data());
    }
    if (!hView && (type & TreeItemType::ResourceTypeName) == TreeItemType::ResourceTypeName) {
        auto typeName = m_pFrame->GetTreeItemText(0);
        ResourceHelper rh(pe);
        auto res = rh.GetFlatResources(typeName);
        if (typeName == L"String Table") {
            auto view = new CMessageTableView(m_pFrame, pe);
            hView = view->DoCreate(hParent);
            for (auto const& r : res) {
                view->SetStringTableData(r.Entry->get_data(), _wtoi(r.Name.substr(1).c_str()));
            }
        }
        else if (typeName == L"Accelerators") {
            auto view = new CAcceleratorTableView(m_pFrame, pe);
            hView = view->DoCreate(hParent);
            for (auto const& r : res) {
                view->AddAccelTable(r.Entry->get_data());
            }
        }
    }
    if (!hView) {
        if ((type & TreeItemType::Resource) == TreeItemType::Resource) {
            auto combined = (type & TreeItemType::Language) == TreeItemType::Language;
            auto node = (pe_resource_directory_entry*)(vtype & ((1LL << 61) - 1));
            auto& data = node->get_data_entry().get_data();
            auto typeName = m_pFrame->GetTreeItemText(combined ? 1 : 2);
            if (typeName == L"Manifest" || typeName == L"HTML") {
                CStringA xml((PCSTR)data.data(), (int)data.size());
                auto view = new CTextView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                view->SetText(CString(xml), TextFormat::Xml);
            }
            else if (typeName == L"Version") {
                auto view = new CVersionView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                view->SetData(data);
            }
            else if (typeName == L"Message Table") {
                auto view = new CMessageTableView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                view->SetMessageTableData(data.data());
            }
            else if (typeName == L"String Table") {
                auto view = new CMessageTableView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                auto sid = m_pFrame->GetTreeItemText(combined ? 0 : 1);
                if (combined)
                    sid = sid.Left(sid.Find(L'('));
                view->SetStringTableData(data, _wtoi(sid.Mid(1)));
            }
            else if (typeName == L"Accelerators") {
                auto view = new CAcceleratorTableView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                view->AddAccelTable(data);
            }
            //else if (typeName == L"Menu") {
            //    auto view = new CMenuView(m_pFrame, pe);
            //    hView = view->DoCreate(hParent);
            //    view->SetData(data);
            //}
            else if (typeName == L"Bitmap" || typeName.CompareNoCase(L"image") == 0) {
                auto view = new CBitmapView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                view->SetData(data);
            }
            else if (typeName == L"Icon" || typeName == L"Group Icon" || typeName == L"Group Cursor" || typeName == L"Cursor") {
                auto view = new CIconGroupView(m_pFrame, pe);
                hView = view->DoCreate(hParent);
                if (typeName == L"Icon" || typeName == L"Cursor")
                    view->SetIconData(data, typeName == L"Icon");
                else
                    view->SetGroupIconData(data);
            }
            else {
                auto view = new CReadOnlyHexView(m_pFrame);
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

HWND ViewManager::CreateAssemblyView(pe_image_full const& pe, PCWSTR title, std::vector<uint8_t> const& code, uint64_t address) {
    auto frame = new CGenericFrame<CAssemblyView>;
    frame->Create(nullptr, frame->rcDefault, title, WS_OVERLAPPEDWINDOW);
    auto hSmall = AtlLoadIconImage(IDI_CODE, 0, 16, 16);
    auto hBig = AtlLoadIconImage(IDI_CODE, 0, 32, 32);
    frame->SetIcon(hSmall, FALSE);
    frame->SetIcon(hBig, TRUE);
    auto view = new CAssemblyView(pe);
    view->Create(frame->m_hWnd, frame->rcDefault, L"", WS_CHILD | WS_VISIBLE);
    pe_image_io io(pe.get_image());
    view->SetCode(address, code);
    frame->SetClient(view);
    frame->ShowWindow(SW_SHOWDEFAULT);

    return frame->m_hWnd;
}
