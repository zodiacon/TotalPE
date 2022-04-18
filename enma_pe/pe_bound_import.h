#pragma once

class pe_bound_imported_ref{
    std::string ref_name;
    uint32_t timestamp;
    
public:
    pe_bound_imported_ref();
    pe_bound_imported_ref(const std::string& ref_name, uint32_t  timestamp);
    ~pe_bound_imported_ref() = default;

    pe_bound_imported_ref(const pe_bound_imported_ref&) = default;
    pe_bound_imported_ref& operator=(const pe_bound_imported_ref&) = default;
    pe_bound_imported_ref(pe_bound_imported_ref&&) = default;
    pe_bound_imported_ref& operator=(pe_bound_imported_ref&&) = default;

public:
    void set_ref_name(const std::string& ref_name);
    void set_timestamp(uint32_t  timestamp);

public:
    std::string get_ref_name() const;
    uint32_t  get_timestamp() const;
};

class pe_bound_library {
    std::string library_name;
    uint32_t  timestamp;
    std::vector<pe_bound_imported_ref> refs;
public:
    pe_bound_library();
    ~pe_bound_library() = default;

    pe_bound_library(const pe_bound_library&) = default;
    pe_bound_library& operator=(const pe_bound_library&) = default;
    pe_bound_library(pe_bound_library&&) = default;
    pe_bound_library& operator=(pe_bound_library&&) = default;

public:
    void set_library_name(const std::string& library_name);
    void set_timestamp(uint32_t  timestamp);

    void add_ref(const pe_bound_imported_ref& ref);
public:
    std::string get_library_name() const;
    uint32_t  get_timestamp() const;
    size_t get_number_of_forwarder_refs() const;

    const std::vector<pe_bound_imported_ref>& get_refs() const;
    std::vector<pe_bound_imported_ref>& get_refs();
};

class pe_bound_import_directory {
    std::vector<pe_bound_library> libraries;
public:
    pe_bound_import_directory() = default;
    ~pe_bound_import_directory() = default;

    pe_bound_import_directory(const pe_bound_import_directory&) = default;
    pe_bound_import_directory& operator=(const pe_bound_import_directory&) = default;
    pe_bound_import_directory(pe_bound_import_directory&&) = default;
    pe_bound_import_directory& operator=(pe_bound_import_directory&&) = default;

public:
    void add_library(const pe_bound_library& lib);
    void clear();
public:
    size_t size() const;

    bool has_library(const std::string& library_name, uint32_t timestamp) const;
    bool has_library(const std::string& library_name) const;

    const std::vector<pe_bound_library>& get_libraries() const;

    std::vector<pe_bound_library>& get_libraries();
};



pe_directory_code get_bound_import_directory(_In_ const pe_image &image, _Out_ pe_bound_import_directory& imports);
bool build_bound_import_directory(_Inout_ pe_image &image, _Inout_ pe_section& section,
    _In_ pe_bound_import_directory& imports);
pe_directory_code get_placement_bound_import_directory(_In_ const pe_image &image, _Inout_ pe_placement& placement);
