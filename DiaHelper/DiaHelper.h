#pragma once

#include "dia2.h"
#include "DiaSymbol.h"
#include <optional>
#include <vector>
#include <string>

enum class CompareOptions {
    None = 0,
    CaseSensitive = 0x1,
    CaseInsensitive = 0x2,
    FNameExt = 0x4,
    RegularExpression = 0x8,
    UndecoratedName = 0x10,
    CaseInRegularExpression = (RegularExpression | CaseInsensitive)
};
DEFINE_ENUM_FLAG_OPERATORS(CompareOptions);

class DiaSession {
public:
	bool OpenImage(PCWSTR path);
	bool OpenPdb(PCWSTR path);
	void Close();
	std::wstring LastError() const;

	DiaSymbol GlobalScope() const;
	std::vector<DiaSymbol> FindChildren(DiaSymbol const& parent, SymbolTag tag = SymbolTag::Null, PCWSTR name = nullptr,
		CompareOptions options = CompareOptions::None) const;

private:
	bool OpenCommon(PCWSTR path, bool image);

private:
	CComPtr<IDiaSession> m_spSession;
	CComPtr<IDiaDataSource> m_spSource;
};

