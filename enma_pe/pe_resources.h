#pragma once

class pe_resource_directory;
class pe_resource_directory_entry;

class pe_resource_data_entry {
    uint32_t codepage;
    std::vector<uint8_t> data;
public:

    pe_resource_data_entry();
    pe_resource_data_entry(const void * data, uint32_t data_size, uint32_t codepage);
    ~pe_resource_data_entry()= default;

    pe_resource_data_entry(const pe_resource_data_entry&) = default;
    pe_resource_data_entry& operator=(const pe_resource_data_entry&) = default;
    pe_resource_data_entry(pe_resource_data_entry&&) = default;
    pe_resource_data_entry& operator=(pe_resource_data_entry&&) = default;

public:
    void set_codepage(uint32_t codepage);
    void set_data(const void * data, uint32_t data_size);
public:
    uint32_t        get_codepage() const;
    const std::vector<uint8_t>&    get_data() const;
    std::vector<uint8_t>&    get_data();
};

class pe_resource_directory_entry {
    uint32_t id;
    std::wstring name;

    union includes {
        includes();
        pe_resource_data_entry* data;
        pe_resource_directory * dir;
    }_ptr;

    bool includes_data;
    bool named;
public:
    pe_resource_directory_entry();
    pe_resource_directory_entry(const pe_resource_directory_entry& other);
    ~pe_resource_directory_entry();

    pe_resource_directory_entry& operator=(const pe_resource_directory_entry& other);
    void release();
public:
    void set_name(const std::wstring& name);
    void set_id(uint32_t id);

    void add_data_entry(const pe_resource_data_entry& entry);
    void add_resource_directory(const pe_resource_directory& dir);
public:
    bool     is_named() const;
    uint32_t get_id() const;
    const std::wstring& get_name() const;
    bool    is_includes_data() const;

    const pe_resource_directory& get_resource_directory() const;
    const pe_resource_data_entry& get_data_entry() const;

    pe_resource_directory& get_resource_directory();
    pe_resource_data_entry& get_data_entry();
};


struct entry_sorter
{
public:
    bool operator()(pe_resource_directory_entry& entry1, pe_resource_directory_entry& entry2);
};

class pe_resource_directory {
    uint32_t characteristics;
    uint32_t timestamp;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t number_of_named_entries;
    uint32_t number_of_id_entries;
    std::vector<pe_resource_directory_entry> entries;
public:
    pe_resource_directory();
    pe_resource_directory(const pe_resource_directory& resource_dir);
    pe_resource_directory(const image_resource_directory& res_dir);
    ~pe_resource_directory();
    pe_resource_directory& operator=(const pe_resource_directory& resource_dir);

    bool entry_by_id(const pe_resource_directory_entry * &entry, uint32_t id);
    bool entry_by_name(const pe_resource_directory_entry * &entry, const std::wstring& name);

    void set_characteristics(uint32_t characteristics);
    void set_timestamp(uint32_t timestamp);
    void set_number_of_named_entries(uint32_t number);
    void set_number_of_id_entries(uint32_t number);
    void set_major_version(uint16_t major_version);
    void set_minor_version(uint16_t minor_version);

    void add_resource_directory_entry(const pe_resource_directory_entry& entry);
    void clear_resource_directory_entry_list();
public:
    size_t size() const;
    uint32_t get_characteristics() const;
    uint32_t get_timestamp() const;
    uint32_t get_number_of_named_entries() const;
    uint32_t get_number_of_id_entries() const;
    uint16_t get_major_version() const;
    uint16_t get_minor_version() const;
    const std::vector<pe_resource_directory_entry>& get_entry_list() const;

    std::vector<pe_resource_directory_entry>& get_entry_list();
};


pe_directory_code get_resources_directory(_In_ const pe_image &image,
    _Out_ pe_resource_directory& resources);                        
bool build_resources_directory(_Inout_ pe_image &image, _Inout_ pe_section& section,
    _Inout_ pe_resource_directory& resources);
pe_directory_code get_placement_resources_directory(_In_ const pe_image &image, _Inout_ pe_placement& placement);
