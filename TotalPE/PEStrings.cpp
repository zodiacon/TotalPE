#include "pch.h"
#include "PEStrings.h"
#include <atltime.h>

static PCWSTR memberVisiblity[] = {
	L"Private Scope", L"Private", L"Protected and Internal", L"Internal", L"Protected",
	L"Protected or Internal", L"Public"
};

std::wstring std::to_wstring(LIEF::PE::SUBSYSTEM type) {
	switch (static_cast<DWORD>(type)) {
		case IMAGE_SUBSYSTEM_NATIVE: return L"Native";
		case IMAGE_SUBSYSTEM_WINDOWS_GUI: return L"Window GUI";
		case IMAGE_SUBSYSTEM_WINDOWS_CUI: return L"Windows CUI";
		case IMAGE_SUBSYSTEM_OS2_CUI: return L"OS2 CUI";
		case IMAGE_SUBSYSTEM_POSIX_CUI: return L"POSIX CUI";
		case IMAGE_SUBSYSTEM_NATIVE_WINDOWS: return L"Native Windows 9x";
		case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI: return L"Windows CE GUI";
		case IMAGE_SUBSYSTEM_EFI_APPLICATION: return L"EFI Application";
		case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER: return L"EFI Boot Service Driver";
		case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER: return L"EFI Runtime Driver";
		case IMAGE_SUBSYSTEM_EFI_ROM: return L"EFI ROM";
		case IMAGE_SUBSYSTEM_XBOX: return L"XBOX";
		case IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION: return L"Windows Boot Application";
	}
	return L"(Unknown)";
}

std::wstring PEStrings::ToDecAndHex(DWORD value, bool hexFirst) {
	std::wstring text;
	if (hexFirst)
		text = std::format(L"0x{:X} ({})", value, value);
	else
		text = std::format(L"{} (0x{:X})", value, value);
	return text;
}

//char const* PEStrings::MagicToString(OptionalHeaderMagic magic) {
//	switch (magic) {
//		case OptionalHeaderMagic::PE32: return "PE32";
//		case OptionalHeaderMagic::PE32Plus: return "PE32+";
//		case OptionalHeaderMagic::Rom: return "ROM";
//	}
//	return "";
//}
//
std::wstring std::to_wstring(LIEF::PE::MACHINE_TYPES type) {
	switch ((DWORD)type) {
		case IMAGE_FILE_MACHINE_UNKNOWN: return L"Unknown";
		case IMAGE_FILE_MACHINE_I386: return L"x86";
		case IMAGE_FILE_MACHINE_ARM: return L"ARM";
		case IMAGE_FILE_MACHINE_ARMNT: return L"ARM NT";
		case IMAGE_FILE_MACHINE_IA64: return L"IA64";
		case IMAGE_FILE_MACHINE_AMD64: return L"x64 (AMD64)";
		case IMAGE_FILE_MACHINE_ARM64: return L"ARM 64";
	}
	return L"";
}

std::wstring PEStrings::Sec1970ToString(DWORD secs) {
	return (PCWSTR)CTime(secs).Format(L"%X");
}

std::wstring std::to_wstring(LIEF::PE::HEADER_CHARACTERISTICS cs) {
	std::wstring result;

	static const struct {
		DWORD cs;
		PCWSTR text;
	} chars[] = {
		{ IMAGE_FILE_RELOCS_STRIPPED,			L"Relocations Stripped" },
		{ IMAGE_FILE_EXECUTABLE_IMAGE,			L"Executable Image" },
		{ IMAGE_FILE_AGGRESIVE_WS_TRIM,			L"Aggressive Trim Working Set" },
		{ IMAGE_FILE_LARGE_ADDRESS_AWARE,		L"Large Address Aware" },
		{ IMAGE_FILE_DEBUG_STRIPPED,			L"Debug Info Stripped" },
		{ IMAGE_FILE_LINE_NUMS_STRIPPED,		L"Line Numbers Stripped" },
		{ IMAGE_FILE_DLL,						L"DLL File" },
		{ IMAGE_FILE_BYTES_REVERSED_LO,			L"Little Endian" },
		{ IMAGE_FILE_32BIT_MACHINE,				L"32-Bit Machine" },
		{ IMAGE_FILE_LOCAL_SYMS_STRIPPED,		L"Local Symbols Stripped" },
		{ IMAGE_FILE_NET_RUN_FROM_SWAP,			L"Net Run from Swap" },
		{ IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP,	L"Removable Run from Swap" },
		{ IMAGE_FILE_UP_SYSTEM_ONLY,			L"Single CPU Only" },
		{ IMAGE_FILE_SYSTEM,					L"System File" },
		{ IMAGE_FILE_BYTES_REVERSED_HI,			L"Big Endian" },
	};

	for (auto& ch : chars) {
		if ((ch.cs & (DWORD)cs) == ch.cs)
			result += std::wstring(ch.text) + L", ";
	}

	if (!result.empty())
		result = result.substr(0, result.size() - 2);
	return result;
}

//std::string PEStrings::DllCharacteristicsToString(DllCharacteristics dc) {
//	std::string result;
//
//	static const struct {
//		DllCharacteristics cs;
//		PCSTR text;
//	} chars[] = {
//		{ DllCharacteristics::AppContainer, "App Container" },
//		{ DllCharacteristics::HighEntropyVA, "High Entropy VA" },
//		{ DllCharacteristics::DynamicBase, "Dynamic Base" },
//		{ DllCharacteristics::ForceIntegrity, "Force Integrity" },
//		{ DllCharacteristics::NxCompat, "NX Compat" },
//		{ DllCharacteristics::ControlFlowGuard, "Control Flow Guard" },
//		{ DllCharacteristics::NoBind, "No Bind" },
//		{ DllCharacteristics::WDMDriver, "WDM Driver" },
//		{ DllCharacteristics::NoIsolation, "No Isolation" },
//		{ DllCharacteristics::TerminalServerAware, "Terminal Server Aware" },
//		{ DllCharacteristics::NoSEH, "No SEH" },
//	};
//
//	for (auto& ch : chars) {
//		if ((ch.cs & dc) == ch.cs)
//			result += std::string(ch.text) + ", ";
//	}
//
//	if (!result.empty())
//		result = result.substr(0, result.size() - 2);
//	return result;
//
//}

std::wstring PEStrings::ToHex(DWORD value, bool leadingZero) {
	if (leadingZero)
		return std::format(L"0x{:08X}", value);
	return std::format(L"0x{:X}", value);
}

std::wstring PEStrings::ToHex(ULONGLONG value) {
	auto result = std::format(L"0x{:X}", value);
	return result;
}

std::string PEStrings::ToMemorySize(ULONGLONG size) {
	auto result = std::format("0x{:X}", size);
	if (size < 1 << 14)
		result = std::format("{} ({} B)", result, size);
	else if (size < 1 << 23)
		result = std::format("{} ({} KB)", result, size >> 10);
	else if (size < 1LL << 33)
		result = std::format("{} ({} MB)", result, size >> 20);
	else
		result = std::format("{} ({} GB)", result, size >> 30);
	return result;
}

PCWSTR PEStrings::ResourceTypeToString(WORD id) {
	static PCWSTR types[] = {
		nullptr, L"Cursor", L"Bitmap", L"Icon",	L"Menu", L"Dialog",
		L"String Table", L"Font Directory", L"Font",
		L"Accelerators", L"RC Data", L"Message Table", L"Group Cursor", nullptr,
		L"Group Icon", nullptr, L"Version", L"Dialog Include",
		nullptr, L"Plug & Play", L"VxD", L"Animated Cursor", L"Animated Icon",
		L"HTML", L"Manifest"
	};
	return id >= _countof(types) ? nullptr : types[id];
}

//std::wstring PEStrings::FormatInstruction(const cs_insn& inst) {
//	std::wstringA text;
//	text.Format("%llX %-10s %s", inst.address, inst.mnemonic, inst.op_str);
//	return std::wstring(text);
//}

std::wstring PEStrings::ManagedTypeAttributesToString(CorTypeAttr attr) {
	static PCWSTR visiblity[] = {
		L"Private", L"Public", L"Nested Public", L"Nested Private",
		L"Nested Family", L"Nested Internal", L"Nested Protected and Internal", L"Nested Protected Internal"
	};

	std::wstring text = visiblity[attr & tdVisibilityMask];
	text += L", ";

	if ((attr & tdLayoutMask) == tdSequentialLayout)
		text += L"Sequential, ";
	else if ((attr & tdLayoutMask) == tdExplicitLayout)
		text += L"Explicit, ";

	if ((attr & tdClassSemanticsMask) == tdInterface)
		text += L"Interface, ";

	if (attr & tdAbstract)
		text += L"Abstract, ";
	if (attr & tdSealed)
		text += L"Sealed, ";
	if (attr & tdSpecialName)
		text += L"Special Name, ";
	if (attr & tdImport)
		text += L"Import, ";
	if (attr & tdSerializable)
		text += L"Serializable, ";
	if (attr & tdWindowsRuntime)
		text += L"Windows Runtime, ";

	return text.substr(0, text.size() - 2);
}

//std::wstring PEStrings::MemberAttributesToString(const ManagedMember& member) {
//	switch (member.Type) {
//		case ManagedMemberType::Method:
//		case ManagedMemberType::Constructor:
//		case ManagedMemberType::StaticConstructor:
//			return MethodAttributesToString((CorMethodAttr)member.Attributes);
//		case ManagedMemberType::Field:
//			return FieldAttributesToString((CorFieldAttr)member.Attributes);
//		case ManagedMemberType::Property:
//			return PropertyAttributesToString((CorPropertyAttr)member.Attributes);
//		case ManagedMemberType::Event:
//			return EventAttributesToString((CorEventAttr)member.Attributes);
//	}
//
//	return L"";
//}

std::wstring PEStrings::MethodAttributesToString(CorMethodAttr attr) {
	std::wstring text = memberVisiblity[attr & mdMemberAccessMask];
	text += L", ";

	if (attr & mdStatic)
		text += L"Static, ";
	if (attr & mdFinal)
		text += L"Final, ";
	if (attr & mdVirtual)
		text += L"Virtual, ";
	if (attr & mdHideBySig)
		text += L"Hide By Sig, ";

	if ((attr & mdVtableLayoutMask) == mdNewSlot)
		text += L"New Slot, ";

	if (attr & mdCheckAccessOnOverride)
		text += L"Check Access on Override, ";
	if (attr & mdAbstract)
		text += L"Abstract, ";
	if (attr & mdSpecialName)
		text += L"Special Name, ";
	if (attr & mdPinvokeImpl)
		text += L"P/Invoke, ";
	if (attr & mdUnmanagedExport)
		text += L"Unmanaged Export, ";

	if ((attr & mdReservedMask) == mdRTSpecialName)
		text += L"Runtime Special Name, ";
	if ((attr & mdReservedMask) == mdHasSecurity)
		text += L"Has Security, ";
	if ((attr & mdReservedMask) == mdRequireSecObject)
		text += L"Require Secure Object, ";

	return text.substr(0, text.size() - 2);
}

std::wstring PEStrings::FieldAttributesToString(CorFieldAttr attr) {
	std::wstring text = memberVisiblity[attr & fdFieldAccessMask];
	text += L", ";

	if (attr & fdStatic)
		text += L"Static, ";
	if (attr & fdInitOnly)
		text += L"Read Only, ";
	if (attr & fdLiteral)
		text += L"Literal, ";
	if (attr & fdNotSerialized)
		text += L"Not Serialized, ";
	if (attr & fdSpecialName)
		text += L"Special Name, ";
	if (attr & fdPinvokeImpl)
		text += L"P/Invoke, ";

	if ((attr & fdReservedMask) == fdRTSpecialName)
		text += L"Runtime Special Name, ";
	if ((attr & fdReservedMask) == fdHasFieldMarshal)
		text += L"Has Field Marshal, ";
	if ((attr & fdReservedMask) == fdHasDefault)
		text += L"Has Default, ";
	if ((attr & fdReservedMask) == fdHasFieldRVA)
		text += L"Has Field RVA, ";

	return text.substr(0, text.size() - 2);
}

std::wstring PEStrings::PropertyAttributesToString(CorPropertyAttr attr) {
	std::wstring text;

	if (IsPrSpecialName(attr))
		text += L"Special Name, ";
	if (IsPrRTSpecialName(attr))
		text += L"Runtime Special Name";
	if (IsPrHasDefault(attr))
		text += L"Has Default, ";

	if (!text.empty())
		text = text.substr(0, text.size() - 2);
	return text;
}

std::wstring PEStrings::EventAttributesToString(CorEventAttr attr) {
	std::wstring text;

	if (IsEvSpecialName(attr))
		text += L"Special Name, ";
	if (IsEvRTSpecialName(attr))
		text += L"Runtime Special Name";

	if (!text.empty())
		text = text.substr(0, text.size() - 2);
	return text;
}


PCWSTR PEStrings::GetDataDirectoryName(int index) {
	PCWSTR names[] = {
		L"Export", L"Import", L"Resource", L"Exception", L"Security", L"Base Relocation",
		L"Debug", L"Architecture", L"Global Pointer", L"Thread Local Storage", L"Load Config",
		L"Bound Import", L"IAT", L"Delay Import", L"COM Descriptor (CLR)"
	};
	return index < 0 || index >= _countof(names) ? L"" : names[index];
}

//std::string PEStrings::SectionCharacteristicsToString(DWORD c) {
//	std::string result;
//	auto ch = static_cast<SectionFlags>(c);
//
//	struct {
//		SectionFlags Flags;
//		PCSTR Text;
//	} items[] = {
//		{ SectionFlags::NoPad, "No Pad" },
//		{ SectionFlags::Code, "Code" },
//		{ SectionFlags::Read, "Read" },
//		{ SectionFlags::Write, "Write" },
//		{ SectionFlags::Execute, "Execute" },
//		{ SectionFlags::Shared, "Shared" },
//		{ SectionFlags::InitializedData, "Initialized Data" },
//		{ SectionFlags::UninitializedData, "Uninitialized Data" },
//		{ SectionFlags::Remove, "Remove" },
//		{ SectionFlags::Discardable, "Discardable" },
//		{ SectionFlags::Info, "Info" },
//		{ SectionFlags::Comdat, "Comdat" },
//		{ SectionFlags::GPRel, "GP Relative" },
//		{ SectionFlags::ExtendedReloc, "Extended Reloc" },
//		{ SectionFlags::NotPaged, "Not Paged" },
//		{ SectionFlags::NotCached, "Not Cached" },
//	};
//
//	for (auto& item : items)
//		if ((ch & item.Flags) != SectionFlags::None)
//			result += item.Text + std::string(", ");
//
//	if (!result.empty())
//		result = result.substr(0, result.length() - 2);
//
//	return result;
//}
