#pragma once

class pe_dotnet_directory {
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t flags;

    union {
        uint32_t entry_point_token;
        uint32_t entry_point_rva;
    };

    image_data_directory meta_data;
    image_data_directory resources;
    image_data_directory strong_name_signature;
    image_data_directory code_manager_table;
    image_data_directory vtable_fixups;
    image_data_directory export_address_table_jumps;
public:
    pe_dotnet_directory();
    ~pe_dotnet_directory() = default;

    pe_dotnet_directory(const pe_dotnet_directory&) = default;
    pe_dotnet_directory& operator=(const pe_dotnet_directory&) = default;
    pe_dotnet_directory(pe_dotnet_directory&&) = default;
    pe_dotnet_directory& operator=(pe_dotnet_directory&&) = default;
public:
    void set_major_version(uint16_t major_version);
    void set_minor_version(uint16_t minor_version);

    void set_flags(uint32_t flags);
    void set_entry_point(uint32_t entry_point);

    void set_meta_data(image_data_directory meta_data);
    void set_resources(image_data_directory resources);
    void set_strong_name_signature(image_data_directory strong_name_signature);
    void set_code_manager_table(image_data_directory code_manager_table);
    void set_vtable_fixups(image_data_directory vtable_fixups);
    void set_export_address_table_jumps(image_data_directory export_address_table_jumps);
public:
    uint16_t get_major_version() const;
    uint16_t get_minor_version() const;

    uint32_t get_flags() const;
    uint32_t get_entry_point() const;

    image_data_directory get_meta_data() const;
    image_data_directory get_resources() const;
    image_data_directory get_strong_name_signature() const;
    image_data_directory get_code_manager_table() const;
    image_data_directory get_vtable_fixups() const;
    image_data_directory get_export_address_table_jumps() const;
};

pe_directory_code get_dotnet_directory(_In_ const pe_image &image, _Out_ pe_dotnet_directory& dotnet);