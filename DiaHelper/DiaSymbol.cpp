#include "pch.h"
#include "DiaSymbol.h"

DiaSymbol::DiaSymbol(IDiaSymbol* sym) : m_spSym(sym) {
}

DiaSymbol::operator bool() const {
	return m_spSym != nullptr;
}

std::wstring DiaSymbol::Name() const {
	CComBSTR name;
	return S_OK == m_spSym->get_name(&name) ? name.m_str : L"";
}

uint32_t DiaSymbol::Id() const {
	uint32_t id = 0;
	m_spSym->get_symIndexId((DWORD*)&id);
	return id;
}

int32_t DiaSymbol::Offset() const {
	int32_t offset;
	m_spSym->get_offset((LONG*)&offset);
	return offset;
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
