#include "pch.h"
#include "ResourceHelper.h"
#include "PEStrings.h"

ResourceHelper::ResourceHelper(pe_image_full const& pe) : m_pe(pe) {
}

std::vector<ResourceItem> ResourceHelper::GetFlatResources(PCWSTR type) const {
	pe_resource_directory_entry const* theDir{ nullptr };
	for (auto& dir : m_pe.get_resources().get_entry_list()) {
		auto name = PEStrings::ResourceTypeToString(dir.get_id());
		if (name == type) {
			theDir = &dir;
			break;
		}
	}
	if (theDir == nullptr)
		return {};

	std::vector<ResourceItem> items;
	items.reserve(8);
	for (auto& dir : theDir->get_resource_directory().get_entry_list()) {
		auto name = dir.is_named() ? dir.get_name() : std::format(L"#{}", dir.get_id());
		for (auto& item : dir.get_resource_directory().get_entry_list()) {
			items.emplace_back(name, &item.get_data_entry());
		}
	}
	return items;
}

