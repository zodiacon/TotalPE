#pragma once

class pe_import_function {
    uint32_t iat_rva;
    uint32_t oft_rva;

    uint16_t hint;
    std::string func_name;

    uint16_t ordinal;
    uint64_t iat_item;
    uint64_t oft_item;

    bool b_import_by_name;
public:
    pe_import_function();
    pe_import_function(uint32_t iat_rva, uint32_t oft_rva, const std::string& func_name, uint16_t hint, uint64_t iat_item = 0, uint64_t oft_item = 0);
    pe_import_function(uint32_t iat_rva, uint32_t oft_rva, uint16_t ordinal, uint64_t iat_item = 0, uint64_t oft_item = 0);

    ~pe_import_function() = default;

    pe_import_function(const pe_import_function&) = default;
    pe_import_function& operator=(const pe_import_function&) = default;
    pe_import_function(pe_import_function&&) = default;
    pe_import_function& operator=(pe_import_function&&) = default;

public:
    pe_import_function& set_func_name(const std::string& func_name);
    pe_import_function& set_hint(uint16_t hint);
    pe_import_function& set_ordinal(uint16_t ordinal);
    pe_import_function& set_iat_rva(uint32_t rva);
    pe_import_function& set_iat_item(uint64_t iat_item);
    pe_import_function& set_oft_rva(uint32_t rva);
    pe_import_function& set_oft_item(uint64_t oft_item);
    pe_import_function& set_import_by_name(bool b);
public:
    std::string get_func_name() const;
    uint16_t get_hint() const;
    uint16_t get_ordinal() const;
    uint32_t get_iat_rva() const;
    uint64_t get_iat_item()const;
    uint32_t get_oft_rva() const;
    uint64_t get_oft_item()const;

    bool  is_import_by_name() const;
};

class pe_import_library {
    std::string library_name;
    
    bool is_bound;

    uint32_t timestamp;
    uint32_t iat_rva;
    uint32_t oft_rva;
    uint32_t library_name_rva;

    std::vector<pe_import_function> functions;
public:
    pe_import_library();
    pe_import_library(const std::string& library_name);
    ~pe_import_library() = default;

    pe_import_library(const pe_import_library&) = default;
    pe_import_library& operator=(const pe_import_library&) = default;
    pe_import_library(pe_import_library&&) = default;
    pe_import_library& operator=(pe_import_library&&) = default;

public:
    pe_import_library& set_library_name(const std::string& library_name);
    pe_import_library& set_timestamp(uint32_t timestamp);
    pe_import_library& set_rva_iat(uint32_t rva);
    pe_import_library& set_rva_oft(uint32_t rva);
    pe_import_library& set_rva_library_name(uint32_t rva);
    pe_import_library& set_bound_library(bool is_bound);
    pe_import_library& add_function(const pe_import_function& function);

    void clear();
public:
    size_t size();

    std::string get_library_name() const;
    uint32_t get_timestamp() const;
    uint32_t get_rva_iat() const;
    uint32_t get_rva_oft() const;
    uint32_t get_rva_library_name() const;
    bool is_bound_library() const;

    const std::vector<pe_import_function>& get_functions() const;
    std::vector<pe_import_function>& get_functions();
};

class pe_import_directory {
    std::vector<pe_import_library> libraries;
public:
    pe_import_directory() = default;
    ~pe_import_directory() = default;

    pe_import_directory(const pe_import_directory&) = default;
    pe_import_directory& operator=(const pe_import_directory&) = default;
    pe_import_directory(pe_import_directory&&) = default;
    pe_import_directory& operator=(pe_import_directory&&) = default;

public:
    void add_library(const pe_import_library& lib);
    void clear();
public:
    size_t size();

    std::vector<pe_import_library>& get_libraries();
    const std::vector<pe_import_library>& get_libraries() const;
    bool get_imported_lib(const std::string& lib_name, pe_import_library * &lib);
    bool get_imported_func(const std::string& lib_name, const std::string& func_name, pe_import_library * &lib, pe_import_function * &func);
    bool get_imported_func(const std::string& lib_name, uint16_t ordinal, pe_import_library * &lib, pe_import_function * &func);
};


enum import_table_build_id {
    import_table_build_iat          = 1 << 1,
    import_table_build_oft          = 1 << 2,
    import_table_build_library_name = 1 << 3,
};


pe_directory_code get_import_directory(_In_ const pe_image &image,
    _Out_ pe_import_directory& imports,
    _In_ const pe_bound_import_directory& bound_imports);

bool build_internal_import_directory_data(_Inout_ pe_image &image,
    _Inout_ pe_section& section, _Inout_ pe_import_directory& imports,
    _In_ uint32_t build_items_ids/*import_table_build_id*/,
    uint32_t iat_rva = -1, uint32_t oft_rva = -1, uint32_t names_rva = -1,
    _In_opt_ const pe_bound_import_directory* bound_imports = 0);
bool build_import_directory_only(_Inout_ pe_image &image,
    _Inout_ pe_section& section, _Inout_ pe_import_directory& imports,
    uint32_t desc_rva = -1);

bool build_import_directory_full(_Inout_ pe_image &image,
    _Inout_ pe_section& section, _Inout_ pe_import_directory& imports,
    uint32_t iat_rva = -1, uint32_t oft_rva = -1, uint32_t names_rva = -1, uint32_t desc_rva = -1,
    _In_opt_ const pe_bound_import_directory* bound_imports = 0);

pe_directory_code get_placement_import_directory(_In_ const pe_image &image,_Inout_ pe_placement& placement,
    _In_ const pe_bound_import_directory& bound_imports);
