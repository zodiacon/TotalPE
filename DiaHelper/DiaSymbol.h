#pragma once

#include <string>

struct IDiaSymbol;

enum class SymbolTag {
    Null,
    Exe,
    Compiland,
    CompilandDetails,
    CompilandEnv,
    Function,
    Block,
    Data,
    Annotation,
    Label,
    PublicSymbol,
    UDT,
    Enum,
    FunctionType,
    PointerType,
    ArrayType,
    BaseType,
    Typedef,
    BaseClass,
    Friend,
    FunctionArgType,
    FuncDebugStart,
    FuncDebugEnd,
    UsingNamespace,
    VTableShape,
    VTable,
    Custom,
    Thunk,
    CustomType,
    ManagedType,
    Dimension,
    CallSite,
    InlineSite,
    BaseInterface,
    VectorType,
    MatrixType,
    HLSLType,
    Caller,
    Callee,
    Export,
    HeapAllocationSite,
    CoffGroup,
    Inlinee,
    Max
};

enum class AccessMode {
	Private = 1,
	Protected,
	Public
};

enum class LocationKind {
    Null,
    Static,
    TLS,
    RegRel,
    ThisRel,
    Enregistered,
    BitField,
    Slot,
    IlRel,
    MetaData,
    Constant,
    RegRelAliasIndir,
    Max
};

enum class DataItemKind {
    Unknown,
    Local,
    StaticLocal,
    Param,
    ObjectPtr,
    FileStatic,
    Global,
    Member,
    StaticMember,
    Constant
};

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

private:
	DiaSymbol(IDiaSymbol* sym);

	CComPtr<IDiaSymbol> m_spSym;
};

