#include "pch.h"
#include "DiaHelper.h"

DiaSymbol::DiaSymbol(IDiaSymbol* sym) : m_spSym(sym) {
}

DiaSymbol::operator bool() const {
	return m_spSym != nullptr;
}

std::wstring DiaSymbol::Name() const {
	CComBSTR name;
	return S_OK == m_spSym->get_name(&name) ? name.m_str : L"";
}

std::wstring DiaSymbol::UndecoratedName() const {
	CComBSTR name;
	m_spSym->get_undecoratedName(&name);
	return name.Length() == 0 ? L"" : name.m_str;
}

uint32_t DiaSymbol::Id() const {
	uint32_t id = 0;
	m_spSym->get_symIndexId((DWORD*)&id);
	return id;
}

uint32_t DiaSymbol::Age() const {
	uint32_t age = 0;
	m_spSym->get_age((DWORD*)&age);
	return age;
}

int32_t DiaSymbol::Offset() const {
	int32_t offset;
	m_spSym->get_offset((LONG*)&offset);
	return offset;
}

AccessMode DiaSymbol::Access() const {
	DWORD access = 0;
	m_spSym->get_access(&access);
	return AccessMode(access);
}

DiaSymbol DiaSymbol::ClassParent() const {
	CComPtr<IDiaSymbol> sym;
	m_spSym->get_classParent(&sym);
	return DiaSymbol(sym);
}

DiaSymbol DiaSymbol::LexicalParent() const {
	CComPtr<IDiaSymbol> sym;
	m_spSym->get_lexicalParent(&sym);
	return DiaSymbol(sym);
}

DiaSymbol DiaSymbol::Type() const {
	CComPtr<IDiaSymbol> sym;
	m_spSym->get_type(&sym);
	return DiaSymbol(sym);
}

LocationKind DiaSymbol::Location() const {
	DWORD loc = 0;
	m_spSym->get_locationType(&loc);
	return static_cast<LocationKind>(loc);
}

SymbolTag DiaSymbol::Tag() const {
	DWORD tag = 0;
	m_spSym->get_symTag(&tag);
	return SymbolTag(tag);
}

DataItemKind DiaSymbol::Kind() const {
	DWORD kind = 0;
	m_spSym->get_dataKind(&kind);
	return static_cast<DataItemKind>(kind);
}

std::vector<DiaSymbol> DiaSymbol::FindChildren(SymbolTag tag, PCWSTR name, CompareOptions options) const {
	std::vector<DiaSymbol> symbols;
	CComPtr<IDiaEnumSymbols> spEnum;
	if (SUCCEEDED(m_spSym->findChildren((enum SymTagEnum)tag, name, (DWORD)options, &spEnum))) {
		LONG count = 0;
		spEnum->get_Count(&count);
		ULONG ret;
		for (LONG i = 0; i < count; i++) {
			CComPtr<IDiaSymbol> sym;
			spEnum->Next(1, &sym, &ret);
			ATLASSERT(sym);
			if (sym == nullptr)
				break;
			symbols.push_back(DiaSymbol(sym));
		}
	}
	return symbols;
}
