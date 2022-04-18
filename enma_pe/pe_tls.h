#pragma once

class pe_tls_directory {
    uint32_t start_address_raw_data;
    uint32_t end_address_raw_data;
    uint32_t address_of_index;
    uint32_t address_of_callbacks;
    uint32_t size_of_zero_fill;
    uint32_t  characteristics;

    struct tls_callback{
        uint32_t rva_callback;
        bool use_relocation;
    };

    std::vector<uint8_t> raw_data;
    std::vector<tls_callback> callbacks;
public:
    pe_tls_directory();
    ~pe_tls_directory() = default;

    pe_tls_directory(const pe_tls_directory&) = default;
    pe_tls_directory& operator=(const pe_tls_directory&) = default;
    pe_tls_directory(pe_tls_directory&&) = default;
    pe_tls_directory& operator=(pe_tls_directory&&) = default;

public:
    void set_start_address_raw_data(uint32_t start_address_raw_data);
    void set_end_address_raw_data(uint32_t end_address_raw_data);
    void set_address_of_index(uint32_t address_of_index);
    void set_address_of_callbacks(uint32_t address_of_callbacks);
    void set_size_of_zero_fill(uint32_t size_of_zero_fill);
    void set_characteristics(uint32_t characteristics);
public:
    uint32_t get_start_address_raw_data() const;
    uint32_t get_end_address_raw_data() const;
    uint32_t get_address_of_index() const;
    uint32_t get_address_of_callbacks() const;
    uint32_t get_size_of_zero_fill() const;
    uint32_t get_characteristics() const;

    const std::vector<uint8_t>& get_raw_data() const;
    const std::vector<tls_callback>& get_callbacks() const;

    std::vector<uint8_t>& get_raw_data();
    std::vector<tls_callback>& get_callbacks();
};

enum tls_table_build_id {
    tls_table_build_raw_data             = 1 << 1,
    tls_table_build_address_of_index     = 1 << 2,
    tls_table_build_callbacks            = 1 << 3,
};



pe_directory_code get_tls_directory(_In_ const pe_image &image,_Out_ pe_tls_directory& tls);


bool build_internal_tls_directory_data(_In_ const pe_image &image, _Inout_ pe_section& section,
    _Out_ pe_tls_directory& tls, _Out_ pe_relocations_directory& relocs,
    _In_ uint32_t build_items_ids/*tls_table_build_id*/);
bool build_tls_directory_only(_Inout_ pe_image &image, _Inout_ pe_section& section,
    _Inout_ pe_tls_directory& tls, _Out_ pe_relocations_directory& relocs);
bool build_tls_directory_full(_Inout_ pe_image &image, _Inout_ pe_section& section,
    _Inout_ pe_tls_directory& tls, _Out_ pe_relocations_directory& relocs);

pe_directory_code get_placement_tls_directory(_In_ const pe_image &image, _Inout_ pe_placement& placement);
