#pragma once

#pragma once

namespace std {
	wstring to_wstring(LIEF::PE::SUBSYSTEM type);
	wstring to_wstring(LIEF::PE::MACHINE_TYPES machine);
	wstring to_wstring(LIEF::PE::HEADER_CHARACTERISTICS cs);
};

struct PEStrings abstract final {
	static std::wstring ToDecAndHex(DWORD value, bool hexFirst = false);
	//static char const* MagicToString(OptionalHeaderMagic magic);
	//static char const* MachineTypeToString(MachineType type);
	static std::wstring Sec1970ToString(DWORD secs);
	//static std::string CharacteristicsToString(ImageCharacteristics ch);
	//static std::string DllCharacteristicsToString(DllCharacteristics ch);
	static std::wstring ToHex(DWORD value, bool leadingZero = false);
	static std::wstring ToHex(ULONGLONG value);
	static std::string ToMemorySize(ULONGLONG size);
	static wchar_t const* ResourceTypeToString(WORD id);
	//static std::wstring FormatInstruction(const cs_insn& inst);
	static std::wstring ManagedTypeAttributesToString(CorTypeAttr attr);
	//static std::wstring MemberAttributesToString(const ManagedMember& member);
	static std::wstring MethodAttributesToString(CorMethodAttr attr);
	static std::wstring FieldAttributesToString(CorFieldAttr attr);
	static std::wstring PropertyAttributesToString(CorPropertyAttr attr);
	static std::wstring EventAttributesToString(CorEventAttr attr);
	static PCWSTR GetDataDirectoryName(int index);
	static std::string SectionCharacteristicsToString(DWORD c);
};

