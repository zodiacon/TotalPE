#pragma once

#include <string>
#include <vector>

struct IDiaSymbol;

class DiaSymbol {
	friend class DiaSession;
public:
    operator bool() const;

    std::wstring Name() const;
    std::wstring UndecoratedName() const;
    uint32_t Id() const;
    uint32_t Age() const;
    int32_t Offset() const;
    AccessMode Access() const;
    DiaSymbol ClassParent() const;
    DiaSymbol LexicalParent() const;
    DiaSymbol Type() const;
    LocationKind Location() const;
    SymbolTag Tag() const;
    DataItemKind Kind() const;
    std::vector<DiaSymbol> FindChildren(SymbolTag tag = SymbolTag::Null, PCWSTR name = nullptr,
        CompareOptions options = CompareOptions::None) const;

private:
	DiaSymbol(IDiaSymbol* sym);

	CComPtr<IDiaSymbol> m_spSym;
};

