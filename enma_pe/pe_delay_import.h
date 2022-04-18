#pragma once


class pe_delay_library {
    std::string library_name;

    bool is_bound;

    uint32_t attributes;   
    uint32_t dll_name_rva;
    uint32_t module_handle_rva;
    uint32_t iat_rva;
    uint32_t names_table_rva;
    uint32_t iat_bound_table_rva;
    uint32_t unload_info_table_rva;
    uint32_t timestamp;

    std::vector<pe_import_function> functions;
public:
    pe_delay_library();
    ~pe_delay_library() = default;

    pe_delay_library(const pe_delay_library&) = default;
    pe_delay_library& operator=(const pe_delay_library&) = default;
    pe_delay_library(pe_delay_library&&) = default;
    pe_delay_library& operator=(pe_delay_library&&) = default;

public:
    pe_delay_library& set_library_name(const std::string& library_name);

    pe_delay_library& set_attributes(uint32_t  attributes);
    pe_delay_library& set_dll_name_rva(uint32_t  dll_name_rva);
    pe_delay_library& set_module_handle_rva(uint32_t  rva);
    pe_delay_library& set_iat_rva(uint32_t  rva);
    pe_delay_library& set_names_table_rva(uint32_t  rva);
    pe_delay_library& set_bound_table_iat_rva(uint32_t  rva);
    pe_delay_library& set_unload_info_table_rva(uint32_t  rva);
    pe_delay_library& set_timestamp(uint32_t  timestamp);
    pe_delay_library& set_bound_library(bool is_bound);

    pe_delay_library& add_function(const pe_import_function& function);
public:
    size_t size() const;

    std::string get_library_name() const;

    uint32_t get_attributes() const;
    uint32_t get_dll_name_rva() const;
    uint32_t get_module_handle_rva() const;
    uint32_t get_iat_rva() const;
    uint32_t get_names_table_rva() const;
    uint32_t get_bound_table_iat_rva() const;
    uint32_t get_unload_info_table_rva() const;
    uint32_t get_timestamp() const;

    bool is_bound_library() const;

    const std::vector<pe_import_function>& get_functions() const;
    std::vector<pe_import_function>& get_functions();

    pe_import_library convert_to_pe_import_library() const;
};

class pe_delay_import_directory {
    std::vector<pe_delay_library> libraries;
public:
    pe_delay_import_directory() = default;
    ~pe_delay_import_directory() = default;

    pe_delay_import_directory(const pe_delay_import_directory&) = default;
    pe_delay_import_directory& operator=(const pe_delay_import_directory&) = default;
    pe_delay_import_directory(pe_delay_import_directory&&) = default;
    pe_delay_import_directory& operator=(pe_delay_import_directory&&) = default;

public:
    void add_library(const pe_delay_library& lib);
    void clear();
public:
    size_t size() const;

    pe_import_directory convert_to_import_table() const;
    const std::vector<pe_delay_library>& get_libraries() const;

    std::vector<pe_delay_library>& get_libraries();
};



pe_directory_code get_delay_import_directory(_In_ const pe_image &image, _Out_ pe_delay_import_directory& imports,
    _In_ const pe_bound_import_directory& bound_imports);
pe_directory_code get_placement_delay_import_directory(_In_ const pe_image &image, _Inout_ pe_placement& placement,
    _In_ const pe_bound_import_directory& bound_imports);
