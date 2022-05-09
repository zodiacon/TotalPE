#pragma once

struct ResourceItem {
	std::wstring Name;
	pe_resource_data_entry const* Entry;
};

class ResourceHelper {
public:
	explicit ResourceHelper(pe_image_full const& pe);
	std::vector<ResourceItem> GetFlatResources(PCWSTR type) const;

private:
	pe_image_full const& m_pe;
};
