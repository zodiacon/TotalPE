#pragma once

struct cs_insn;

enum class DllCharacteristics : unsigned short {
	None = 0,
	HighEntropyVA = 0x20,
	DynamicBase = 0x40,
	ForceIntegrity = 0x80,
	NxCompat = 0x100,
	NoIsolation = 0x200,
	NoSEH = 0x400,
	NoBind = 0x800,
	AppContainer = 0x1000,
	WDMDriver = 0x2000,
	ControlFlowGuard = 0x4000,
	TerminalServerAware = 0x8000
};
DEFINE_ENUM_FLAG_OPERATORS(DllCharacteristics);

struct PEStrings abstract final {
	static std::wstring MagicToString(uint16_t magic);
	static std::wstring SubsystemToString(uint32_t type);
	static std::wstring MachineTypeToString(uint16_t);
	static std::wstring CharacteristicsToString(uint32_t cs);
	static std::wstring ToDecAndHex(DWORD value, bool hexFirst = false);
	static std::wstring Sec1970ToString(DWORD secs);
	static std::wstring DllCharacteristicsToString(uint32_t ch);
	static std::wstring ToHex(DWORD value, bool leadingZero = false);
	static std::wstring ToHex(ULONGLONG value);
	static std::wstring ToMemorySize(ULONGLONG size);
	static std::wstring ResourceTypeToString(WORD id);
	static CString FormatInstruction(const cs_insn& inst);
	static std::wstring ManagedTypeAttributesToString(CorTypeAttr attr);
	//static std::wstring MemberAttributesToString(const ManagedMember& member);
	static std::wstring MethodAttributesToString(CorMethodAttr attr);
	static std::wstring FieldAttributesToString(CorFieldAttr attr);
	static std::wstring PropertyAttributesToString(CorPropertyAttr attr);
	static std::wstring EventAttributesToString(CorEventAttr attr);
	static PCWSTR GetDataDirectoryName(int index);
	static std::wstring SectionCharacteristicsToString(DWORD c);
	static std::wstring PrimaryLanguageToString(WORD l);
	static std::wstring LanguageToString(DWORD lang);
	static std::wstring UndecorateName(PCWSTR name);
	static std::wstring VersionFileOSToString(DWORD type);
	static std::wstring FileTypeToString(DWORD type);
	static std::wstring FileSubTypeToString(DWORD type, DWORD subType);
	static std::wstring FileFlagsToString(DWORD flags);
	static PCWSTR DebugTypeToString(DWORD type);
	static PCWSTR CertificateTypeToString(DWORD type);
};

