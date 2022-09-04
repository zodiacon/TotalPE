#pragma once

#include "dia2.h"

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

#include "DiaSession.h"
#include "DiaSymbol.h"
