#pragma once

class pe_debug_entry {
    uint32_t characteristics;
    uint32_t timestamp;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t type;
    uint32_t size_of_data;
    uint32_t address_of_raw_data;
    uint32_t pointer_to_raw_data;

    std::vector<uint8_t> item_data;
public:
    pe_debug_entry();
    pe_debug_entry(uint32_t characteristics, uint32_t timestamp, uint16_t major_version, uint16_t minor_version,
        uint32_t type, uint32_t size_of_data, uint32_t address_of_raw_data, uint32_t pointer_to_raw_data,
     void * data);

    ~pe_debug_entry() = default;
    pe_debug_entry(const pe_debug_entry&) = default;
    pe_debug_entry& operator=(const pe_debug_entry&) = default;
    pe_debug_entry(pe_debug_entry&&) = default;
    pe_debug_entry& operator=(pe_debug_entry&&) = default;

public:
    void  set_characteristics(uint32_t characteristics);
    void  set_timestamp(uint32_t timestamp);
    void  set_major_version(uint16_t major_version);
    void  set_minor_version(uint16_t minor_version);
    void  set_type(uint32_t type);
    void  set_size_of_data(uint32_t size_of_data);
    void  set_address_of_raw_data(uint32_t address_of_raw_data);
    void  set_pointer_to_raw_data(uint32_t pointer_to_raw_data);
public:
    uint32_t get_characteristics() const;
    uint32_t get_timestamp() const;
    uint16_t get_major_version() const;
    uint16_t get_minor_version() const;
    uint32_t get_type() const;
    uint32_t get_size_of_data() const;
    uint32_t get_address_of_raw_data() const;
    uint32_t get_pointer_to_raw_data() const;

    const std::vector<uint8_t>& get_item_data() const;
    std::vector<uint8_t>& get_item_data();
};

class pe_debug_directory {

    std::vector<pe_debug_entry> entries;
public:
    pe_debug_directory();
    pe_debug_directory(const pe_debug_directory& debug);
    ~pe_debug_directory();

    pe_debug_directory& operator=(const pe_debug_directory& debug);
public:
    void add_entry(const pe_debug_entry& entry);
    void clear();
public:
    size_t size() const;

    const std::vector<pe_debug_entry>& get_entries() const;
    std::vector<pe_debug_entry>& get_entries();
};

pe_directory_code get_debug_directory(_In_ const pe_image &image, _Out_ pe_debug_directory& debug);
pe_directory_code get_placement_debug_directory(_In_ const pe_image &image, _Inout_ pe_placement& placement);