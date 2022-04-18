#pragma once

struct pe_relocation_entry {
    uint32_t relative_virtual_address;
    uint32_t relocation_id;
    uint64_t data;
    uint8_t type;
};


class pe_relocations_directory {
    std::vector<pe_relocation_entry> relocations;
public:
    pe_relocations_directory() = default;
    ~pe_relocations_directory() = default;

    pe_relocations_directory(const pe_relocations_directory&) = default;
    pe_relocations_directory& operator=(const pe_relocations_directory&) = default;
    pe_relocations_directory(pe_relocations_directory&&) = default;
    pe_relocations_directory& operator=(pe_relocations_directory&&) = default;
public:
    void add_relocation(uint32_t rva, uint32_t relocation_id, uint8_t type);

    bool erase_item(uint32_t rva);
    bool erase_first_item_by_id(uint32_t relocation_id);
    bool erase_all_items_by_id(uint32_t relocation_id);
    
    void get_items_by_relocation_id(std::vector<pe_relocation_entry*>& found_relocs, uint32_t relocation_id);

    void get_items_in_segment(std::vector<pe_relocation_entry>& relocs, uint32_t segment_rva, size_t segment_size) const;
    bool erase_items_in_segment(uint32_t segment_rva, size_t segment_size);


    void clear();
    void sort();  
public:
    size_t size() const;

    bool has_item(uint32_t rva) const;
    bool has_item_id(uint32_t relocation_id) const;
    
    bool get_item_id(uint32_t rva, uint32_t& relocation_id) const;

    const std::vector<pe_relocation_entry>& get_entries() const;
    std::vector<pe_relocation_entry>& get_entries();
};




pe_directory_code get_relocation_directory(_In_ const pe_image& image,
    _Out_ pe_relocations_directory& relocs, bool ignore_absolute = true);
bool build_relocation_directory(_Inout_ pe_image &image, _Inout_ pe_section& section,
    _In_ const pe_relocations_directory& relocs);
pe_directory_code get_placement_relocation_directory(_In_ const pe_image &image, _Inout_ pe_placement& placement);
